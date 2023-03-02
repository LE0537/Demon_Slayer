#include "stdafx.h"
#include "NezukoMoveJumpState.h"
#include "NezukoMoveState.h"
#include "NezukoIdleState.h"
#include "NezukoJumpState.h"
#include "Layer.h"
#include "GameInstance.h"
#include "NezukoJumpMoveAttackState.h"
#include "NezukoJumpSkill_Move.h"
#include "Effect_Manager.h"
using namespace Nezuko;


CMoveJumpState::CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eDirection = eDir;
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CNezukoState * CMoveJumpState::HandleInput(CNezuko* pNezuko)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pNezuko->Get_i1P())
	{
	case 1:
		if (m_eStateType != CNezukoState::TYPE_END)
		{
			if (pGameInstance->Key_Pressing(DIK_J)) // ¾Õ
			{
				return new CJumpMoveAttackState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_I)) // ¾Õ
			{
				return new CJumpSkill_MoveState(TYPE_START);
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
		if (m_eStateType != CNezukoState::TYPE_END)
		{
			if (pGameInstance->Key_Pressing(DIK_Z)) // ¾Õ
			{
				return new CJumpMoveAttackState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_X)) // ¾Õ
			{
				return new CJumpSkill_MoveState(TYPE_START);
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
	}

	return nullptr;
}

CNezukoState * CMoveJumpState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÃÑ ½Ã°£
	_float fCurrentTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÇöÀç ½Ã°£

	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_JUMP_START);
	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_JUMP_LOOP);
	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_JUMP_END);

	if (m_eStateType == CNezukoState::TYPE_CHANGE)
	{
		//pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
		return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_START);
	}
	

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			return new CJumpState(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			//return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}


	return nullptr;
}

CNezukoState * CMoveJumpState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	m_fJumpTime += 0.035f;

	if(m_eStateType != STATE_TYPE::TYPE_END)
		Jump(pNezuko, fTimeDelta + m_fJumpTime);

	if(m_bMove == true)
		Move(pNezuko, fTimeDelta);

	pNezuko->Get_Model()->Play_Animation(fTimeDelta,true);
	if (m_eStateType == CNezukoState::TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pNezuko);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	return nullptr;
}

void CMoveJumpState::Enter(CNezuko* pNezuko)
{

	m_eStateId = CNezukoState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_JUMP_START);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_JUMP_START, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_START);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpStart.wav"), g_fEffect);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_JUMP_LOOP);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_JUMP_LOOP, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_LOOP);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_JUMP_END);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_JUMP_END, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_END);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpEnd.wav"), g_fEffect);
		break;
	}


}

void CMoveJumpState::Exit(CNezuko* pNezuko)
{
}

void CMoveJumpState::Move(CNezuko* CNezuko, _float fTimeDelta)
{
	_float fCamAngle = CNezuko->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		CNezuko->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		CNezuko->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		CNezuko->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		CNezuko->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		CNezuko->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		CNezuko->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		CNezuko->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		CNezuko->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP && m_bMove == true)
{
		CNezuko->Get_Transform()->Go_Straight(fTimeDelta, CNezuko->Get_NavigationCom());
		CCharacters* m_pTarget = CNezuko->Get_BattleTarget();
		CCollider*	pMyCollider = CNezuko->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{

			_float fSpeed = CNezuko->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = CNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(CNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = CNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (CNezuko->Get_NavigationCom()->Cheak_Cell(vPos))
				CNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				CNezuko->Get_Transform()->Go_Backward(fTimeDelta / 2.f, CNezuko->Get_NavigationCom());

		}
	}
}

CNezukoState*  CMoveJumpState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pNezuko->Get_NavigationHeight().y;
	pNezuko->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 30.f;


	_vector      vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	

	if (y <= fEndHeight)
	{
		pNezuko->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


		if (m_bMove == false)
		{
			m_eStateType = TYPE_END;
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_JUMP_END);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_JUMP_END, 0.01f);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_END);
		}
		else
		{
			m_eStateType = STATE_TYPE::TYPE_CHANGE;
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pNezuko);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	} 

	pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

