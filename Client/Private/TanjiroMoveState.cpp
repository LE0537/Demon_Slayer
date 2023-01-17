#include "stdafx.h"
#include "TanjiroMoveState.h"
#include "TanjiroIdlestate.h"
#include "TanjiroMoveJumpState.h"
#include "GameInstance.h"

using namespace Tanjiro;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CTanjiroState * CMoveState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_W)) // ��
	{
		 if (pGameInstance->Key_Pressing(DIK_A)) // ��
		{
			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_LF, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // ��
		{
			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_RF, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_LOOP);
		}
		else
		{
			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}

			return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_LOOP);
		}
	}

	else if (pGameInstance->Key_Pressing(DIK_S)) // ��
	{
		 if (pGameInstance->Key_Pressing(DIK_A)) // ��
		{
			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_LB, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // �� 
		{
			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_RB, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_LOOP);
		}
		else
		{
			if (pGameInstance->Key_Down(DIK_SPACE))
			{
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_BACK, CTanjiroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_LOOP);
		}
	}


	else if (pGameInstance->Key_Pressing(DIK_A)) // ��
	{
		if (pGameInstance->Key_Down(DIK_SPACE))
		{
			_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CMoveJumpState(OBJDIR::DIR_LEFT, CTanjiroState::TYPE_START, fPositionY, 0.f);
		}
		return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
	}
	else if (pGameInstance->Key_Pressing(DIK_D)) // ��
	{
		if (pGameInstance->Key_Down(DIK_SPACE))
		{
			_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CMoveJumpState(OBJDIR::DIR_RIGHT, CTanjiroState::TYPE_START, fPositionY, 0.f);
		}
		return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_LOOP);
	}
	else
		return new CIdleState();



	return nullptr;
}

CTanjiroState * CMoveState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			m_eStateType = CTanjiroState::TYPE_LOOP;
			break;
			//case Client::CTanjiroState::TYPE_LOOP:
			//	m_eStateType = CTanjiroState::TYPE_LOOP;
			//	break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}

	return nullptr;
}

CTanjiroState * CMoveState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	Move(pTanjiro, fTimeDelta);
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);


	return nullptr;
}

void CMoveState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_START);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_START);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_LOOP);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_LOOP);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_END);
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CTanjiro * pTanjiro)
{
}

void CMoveState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pTanjiro->Get_Transform()->Set_RotationY(0.f);
		break;
	case Client::DIR_LEFT:
		pTanjiro->Get_Transform()->Set_RotationY(270.f);
		break;
	case Client::DIR_RIGHT:
		pTanjiro->Get_Transform()->Set_RotationY(90.f);
		break;
	case Client::DIR_BACK:
		pTanjiro->Get_Transform()->Set_RotationY(180.f);
		break;
	case Client::DIR_LF:
		pTanjiro->Get_Transform()->Set_RotationY(305.f);
		break;
	case Client::DIR_RF:
		pTanjiro->Get_Transform()->Set_RotationY(45.f);
		break;
	case Client::DIR_LB:
		pTanjiro->Get_Transform()->Set_RotationY(225.f);
		break;
	case Client::DIR_RB:
		pTanjiro->Get_Transform()->Set_RotationY(135.f);
		break;
	case Client::DIR_STOP:
		break;
	}

	if(m_eDirection != DIR_STOP)
		pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);
}


