#include "stdafx.h"
#include "KyoujuroMoveJumpState.h"
#include "KyoujuroMoveState.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroJumpState.h"
#include "Layer.h"
#include "GameInstance.h"
#include "KyoujuroJumpSkill_Common.h"
#include "KyoujuroJumpSkill_Move.h"
#include "KyoujuroJumpMoveAttackState.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CMoveJumpState::CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eDirection = eDir;
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CKyoujuroState * CMoveJumpState::HandleInput(CKyoujuro * pKyoujuro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pKyoujuro->Get_i1P())
	{
	case 1:
		if (m_eStateType != CKyoujuroState::TYPE_DEFAULT)
		{
			if (pGameInstance->Key_Pressing(DIK_J)) // ¾Õ
			{
				return new CJumpMoveAttackState(TYPE_START);
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

				if (pGameInstance->Key_Down(DIK_I))
				{
					if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pKyoujuro->Set_SkillBar(-200);
						return new CJumpSkill_CommonState(m_fCurrentPosY);
					}
				}

				m_bMove = false;
				m_eNextDir = OBJDIR::DIR_STOP;
			}
		}
		else
		{
			m_bMove = false;
			m_eNextDir = OBJDIR::DIR_STOP;
		}

		if (m_bMove == true)
		{
			if (pGameInstance->Key_Down(DIK_I))
			{
				if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CJumpSkill_MoveState(m_fCurrentPosY);
				}
			}
		}
		break;
	case 2:
		if (m_eStateType != CKyoujuroState::TYPE_DEFAULT)
		{
			if (pGameInstance->Key_Pressing(DIK_Z)) // ¾Õ
			{
				return new CJumpMoveAttackState(TYPE_START);
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
			if (pGameInstance->Key_Down(DIK_X))
			{
				if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CJumpSkill_CommonState(m_fCurrentPosY);
				}
			}
			m_bMove = false;
			m_eNextDir = OBJDIR::DIR_STOP;
		}

		if (m_bMove == true)
		{
			if (pGameInstance->Key_Down(DIK_X))
			{
				if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CJumpSkill_MoveState(m_fCurrentPosY);
				}
			}
		}

		break;
	default:
		break;
	}

	return nullptr;
}

CKyoujuroState * CMoveJumpState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÃÑ ½Ã°£
	_float fCurrentTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÇöÀç ½Ã°£

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_START);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_LOOP_START);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_LOOP_END);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_JUMP_END);


	if (m_eStateType == CKyoujuroState::TYPE_CHANGE)
	{
		//pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
		return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_START);
	}



	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			printf_s("Start Jump \n");
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CMoveJumpState(m_eNextDir, STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			printf_s("Loop Jump \n");
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			return new CMoveJumpState(m_eNextDir, STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_END:
			printf_s("End jump \n");
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());

			//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CKyoujuroState::TYPE_DEFAULT:
			printf_s("Default Jump \n");
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}


	return nullptr;
}

CKyoujuroState * CMoveJumpState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	if (m_eStateType == TYPE_START)
	{
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pKyoujuro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	else if (m_eStateType == TYPE_LOOP)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else if (m_eStateType == TYPE_DEFAULT)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.035f;


	if (m_eStateType != TYPE_DEFAULT)
		Jump(pKyoujuro, fTimeDelta + m_fJumpTime);

	if (m_bMove == true)
		Move(pKyoujuro, fTimeDelta);

	return nullptr;
}

void CMoveJumpState::Enter(CKyoujuro * pKyoujuro)
{

	m_eStateId = CKyoujuroState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_START, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_START);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_LOOP_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_LOOP_START, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_LOOP_START);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_LOOP_END);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_JUMP_LOOP_END, 0.01f);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_LOOP_END);
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_END);
		break;
	case Client::CKyoujuroState::TYPE_CHANGE:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_MOVE_START);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_MOVE_START);
		break;
	}


}

void CMoveJumpState::Exit(CKyoujuro * pKyoujuro)
{
	pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
}

void CMoveJumpState::Move(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	//_float fCamAngle = pKyoujuro->Get_CamAngle();

	//switch (m_eDirection)
	//{
	//case Client::DIR_STRAIGHT:
	//	pKyoujuro->Get_Transform()->Set_RotationY(0.f + fCamAngle);
	//	break;
	//case Client::DIR_LEFT:
	//	pKyoujuro->Get_Transform()->Set_RotationY(270.f + fCamAngle);
	//	break;
	//case Client::DIR_RIGHT:
	//	pKyoujuro->Get_Transform()->Set_RotationY(90.f + fCamAngle);
	//	break;
	//case Client::DIR_BACK:
	//	pKyoujuro->Get_Transform()->Set_RotationY(180.f + fCamAngle);
	//	break;
	//case Client::DIR_LF:
	//	pKyoujuro->Get_Transform()->Set_RotationY(305.f + fCamAngle);
	//	break;
	//case Client::DIR_RF:
	//	pKyoujuro->Get_Transform()->Set_RotationY(45.f + fCamAngle);
	//	break;
	//case Client::DIR_LB:
	//	pKyoujuro->Get_Transform()->Set_RotationY(225.f + fCamAngle);
	//	break;
	//case Client::DIR_RB:
	//	pKyoujuro->Get_Transform()->Set_RotationY(135.f + fCamAngle);
	//	break;
	//case Client::DIR_STOP:
	//	break;
	//}

	if (m_eDirection != DIR_STOP && m_bMove == true)
	{
		pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta, pKyoujuro->Get_NavigationCom());

		CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();
		CCollider*	pMyCollider = pKyoujuro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{

			_float fSpeed = pKyoujuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pKyoujuro->Get_NavigationCom()->Cheak_Cell(vPos))
				pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyoujuro->Get_NavigationCom());

		}
	}
}

CKyoujuroState*  CMoveJumpState::Jump(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pKyoujuro->Get_NavigationHeight().y;
	pKyoujuro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 30.f;


	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	fStartHeight = pKyoujuro->Get_NavigationHeight().y;
	fEndHeight = pKyoujuro->Get_NavigationHeight().y;


	_vector      vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);




	if (y <= fEndHeight)
	{
		pKyoujuro->Get_Transform()->Set_Jump(false); 
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		if (m_eNextDir == OBJDIR::DIR_STOP)
		{
			m_eStateType = CKyoujuroState::TYPE_DEFAULT;
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_JUMP_END);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_END);
		}
		else
		{
			m_eStateType = CKyoujuroState::TYPE_CHANGE;
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pKyoujuro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}

	pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

