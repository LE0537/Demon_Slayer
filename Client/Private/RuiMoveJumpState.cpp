#include "stdafx.h"
#include "RuiMoveJumpState.h"
#include "RuiMoveState.h"
#include "RuiIdleState.h"
#include "RuiJumpState.h"
#include "Layer.h"
#include "GameInstance.h"
#include "RuiJumpMoveAttackState.h"
#include "RuiJumpSkill_Move.h"
#include "Effect_Manager.h"
using namespace Rui;


CMoveJumpState::CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eDirection = eDir;
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CRuiState * CMoveJumpState::HandleInput(CRui* pRui)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pRui->Get_i1P())
	{
	case 1:
		if (m_eStateType != CRuiState::TYPE_END)
		{
			if (pGameInstance->Key_Down(DIK_J))
				return new CJumpMoveAttackState(TYPE_START);
			else if (pGameInstance->Key_Down(DIK_I))
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CJumpSkill_MoveState(TYPE_START);
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
	
		if (m_eStateType != CRuiState::TYPE_END)
		{
			if (pGameInstance->Key_Down(DIK_Z))
			{
				int a = 0;
				return new CJumpMoveAttackState(TYPE_START);
			}
			else if (pGameInstance->Key_Down(DIK_X))
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CJumpSkill_MoveState(TYPE_START);
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
	}

	return nullptr;
}

CRuiState * CMoveJumpState::Tick(CRui* pRui, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÃÑ ½Ã°£
	_float fCurrentTime = 0.f; // ¾Ö´Ï¸ÞÀÌ¼Ç ÇöÀç ½Ã°£

	pRui->Get_Model()->Set_Loop(CRui::ANIM_JUMP_START);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_JUMP_LOOP);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_JUMP_END);

	if (m_eStateType == CRuiState::TYPE_CHANGE)
	{
		//pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
		return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_START);
	}
	

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			return new CJumpState(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			//return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState(STATE_ID::STATE_JUMP);
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}


	return nullptr;
}

CRuiState * CMoveJumpState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	m_fJumpTime += 0.05f;

	if(m_eStateType != STATE_TYPE::TYPE_END)
		Jump(pRui, fTimeDelta + m_fJumpTime);

	if(m_bMove == true)
		Move(pRui, fTimeDelta);

	pRui->Get_Model()->Play_Animation(fTimeDelta);
	if (m_eStateType == TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pRui);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	return nullptr;
}

void CMoveJumpState::Enter(CRui* pRui)
{

	m_eStateId = CRuiState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_START);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_START, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_START);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_LOOP);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_LOOP, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_LOOP);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_END);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_END, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_END);
		break;
	}


}

void CMoveJumpState::Exit(CRui* pRui)
{
}

void CMoveJumpState::Move(CRui* pRui, _float fTimeDelta)
{
	_float fCamAngle = pRui->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pRui->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		pRui->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		pRui->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		pRui->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		pRui->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		pRui->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		pRui->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		pRui->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP && m_bMove == true)
	{
		pRui->Get_Transform()->Go_Straight(fTimeDelta, pRui->Get_NavigationCom());

		CCharacters* m_pTarget = pRui->Get_BattleTarget();
		CCollider*	pMyCollider = pRui->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{

			_float fSpeed = pRui->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pRui->Get_NavigationCom()->Cheak_Cell(vPos))
				pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pRui->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRui->Get_NavigationCom());

		}
	}
}

CRuiState*  CMoveJumpState::Jump(CRui* pRui, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	

	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);		


		if (m_bMove == false)
		{
			m_eStateType = TYPE_END;
			pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_JUMP_END);
			pRui->Get_Model()->Set_LinearTime(CRui::ANIM_JUMP_END, 0.01f);
			pRui->Set_AnimIndex(CRui::ANIM_JUMP_END);
		}
		else
		{
			m_eStateType = STATE_TYPE::TYPE_CHANGE;
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pRui);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	} 

	pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

