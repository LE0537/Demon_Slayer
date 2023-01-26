#include "stdafx.h"
#include "RuiMoveState.h"
#include "RuiIdleState.h"
#include "Rui.h"
#include "GameInstance.h"
#include "Characters.h"
#include "Layer.h"
#include "RuiDashState.h"

using namespace Rui;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CRuiState * CMoveState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pRui->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_W)) // 菊
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LF);
				else
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RF);
				else
					return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_STRAIGHT);
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // 第
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LB);
				else
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快 
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RB);
				else
					return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_BACK);
				else
					return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}

		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // 谅
		{
			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_LEFT);
			else
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快
		{
			if (pGameInstance->Key_Pressing(DIK_L))
				return new CDashState(DIR_RIGHT);
			else
				return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else
			return new CIdleState();
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_UP)) // 菊
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LF);
				else
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RF);
				else
					return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_STRAIGHT);
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 第
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LB);
				else
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快 
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RB);
				else
					return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_BACK);
				else
					return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_LEFT);
			else
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDashState(DIR_RIGHT);
			else
				return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else
			return new CIdleState();

		break;
	}




	return nullptr;
}

CRuiState * CMoveState::Tick(CRui* pRui, _float fTimeDelta)
{

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			m_eStateType = CRuiState::TYPE_LOOP;
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CRuiState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	return nullptr;


}

CRuiState * CMoveState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	Move(pRui, fTimeDelta);
	pRui->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CMoveState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_MOVE_START);
		pRui->Set_AnimIndex(CRui::ANIM_MOVE_START);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_MOVE_END);
		pRui->Set_AnimIndex(CRui::ANIM_MOVE_END);
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CRui* pRui)
{
}

void CMoveState::Move(CRui* pRui, _float fTimeDelta)
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

	if (m_eDirection != DIR_STOP)
		pRui->Get_Transform()->Go_StraightNoNavi(fTimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	CCollider*	pMyCollider = pRui->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{
		float fSpeed = pRui->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;
		
		_vector vPow = XMVector3Dot(pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);
		
		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
	}
	
	RELEASE_INSTANCE(CGameInstance);
}
