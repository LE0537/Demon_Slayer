#include "stdafx.h"
#include "ShinobuMoveJumpState.h"
#include "ShinobuMoveState.h"
#include "ShinobuIdlestate.h"
#include "GameInstance.h"
#include "ShinobuJumpState.h"
#include "ShinobuJumpMoveAttackState.h"
#include "ShinobuJumpSkill_Move.h"
#include "Effect_Manager.h"

using namespace Shinobu;

CMoveJumpState::CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eDirection = eDir;
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CShinobuState * CMoveJumpState::HandleInput(CShinobu* pShinobu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pShinobu->Get_i1P())
	{
	case 1:
		if (m_eStateType != STATE_TYPE::TYPE_DEFAULT)
		{
			if (pGameInstance->Key_Pressing(DIK_J)) // ¾Õ
			{
				return new CJumpMoveAttackState(TYPE_START);
			}
			else if (pGameInstance->Key_Down(DIK_I))
			{
				if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pShinobu->Set_SkillBar(-200);
					return new CJumpMoveSkillState(TYPE_START);
				}
			}

			if (pGameInstance->Key_Pressing(DIK_W)) // ¾Õ
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LF;
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_RF;
				}
				else
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_STRAIGHT;
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_S)) // µÚ
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LB;
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì 
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_RB;
				}
				else
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_BACK;
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_A)) // ÁÂ
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_LEFT;
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ¿ì
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_RIGHT;
			}
			else
			{
				m_bMove = false;
				m_eNextDir = OBJDIR::DIR_STOP;
			}
		}
		else
		{
			m_bMove = false;
			m_eNextDir = OBJDIR::DIR_STOP;
		}
		break;
	case 2:
		if (m_eStateType != STATE_TYPE::TYPE_DEFAULT)
		{
			if (pGameInstance->Key_Pressing(DIK_Z)) // ¾Õ
			{
				return new CJumpMoveAttackState(TYPE_START);
			}
			else if (pGameInstance->Key_Down(DIK_X))
			{
				if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pShinobu->Set_SkillBar(-200);
					return new CJumpMoveSkillState(STATE_TYPE::TYPE_START);
				}
			}

			if (pGameInstance->Key_Pressing(DIK_UP)) // ¾Õ
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LF;
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_RF;
				}
				else
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_STRAIGHT;
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // µÚ
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LB;
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì 
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_RB;
				}
				else
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_BACK;
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ÁÂ
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_LEFT;
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ¿ì
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_RIGHT;
			}
			else
			{
				m_bMove = false;
				m_eNextDir = OBJDIR::DIR_STOP;
			}
		}
		else
		{
			m_bMove = false;
			m_eNextDir = OBJDIR::DIR_STOP;
		}
		break;
	default:
		break;
	}



	return nullptr;
}

CShinobuState * CMoveJumpState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÃÑ ½Ã°£
	_float fCurrentTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÇöÀç ½Ã°£

	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_JUMP_START);
	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_JUMP_LOOP);
	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_JUMP_END);

	if (m_eStateType == CShinobuState::TYPE_CHANGE)
	{
		//pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
		return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_START);
	}


	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			return new CJumpstate(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CShinobuState::TYPE_LOOP:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			//return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}


	return nullptr;
}

CShinobuState * CMoveJumpState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	m_fJumpTime += 0.035f;

	if (m_eStateType != STATE_TYPE::TYPE_END)
		Jump(pShinobu, fTimeDelta + m_fJumpTime);

	if (m_bMove == true)
		Move(pShinobu, fTimeDelta);

	pShinobu->Get_Model()->Play_Animation(fTimeDelta);
	if (m_eStateType == CShinobuState::TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pShinobu);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	return nullptr;
}

void CMoveJumpState::Enter(CShinobu* pShinobu)
{

	m_eStateId = CShinobuState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_JUMP_START);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_JUMP_START, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_START);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_JUMP_LOOP);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_JUMP_LOOP, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_LOOP);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_JUMP_END);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_JUMP_END, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_END);
		break;
	}



}

void CMoveJumpState::Exit(CShinobu* pShinobu)
{
}

void CMoveJumpState::Move(CShinobu* pShinobu, _float fTimeDelta)
{
	_float fCamAngle = pShinobu->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pShinobu->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		pShinobu->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		pShinobu->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		pShinobu->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		pShinobu->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		pShinobu->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		pShinobu->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		pShinobu->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP && m_bMove == true)
	{
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta, pShinobu->Get_NavigationCom());

		CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
		CCollider*	pMyCollider = pShinobu->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{

			_float fSpeed = pShinobu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPos))
				pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pShinobu->Get_NavigationCom());

		}
	}
}

CShinobuState*  CMoveJumpState::Jump(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pShinobu->Get_NavigationHeight().y;
	pShinobu->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 30.f;


	_vector      vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);




	if (y <= fEndHeight)
	{
		pShinobu->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


		if (m_bMove == false)
		{
			m_eStateType = TYPE_END;
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_JUMP_END);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_JUMP_END, 0.01f);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_END);
		}
		else
		{
			m_eStateType = STATE_TYPE::TYPE_CHANGE;
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pShinobu);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}

	pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}



