#include "stdafx.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "RuiMoveState.h"
#include "RuiAtk_1_State.h"
#include "RuiJumpState.h"
using namespace Rui;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CRuiState * CIdleState::HandleInput(CRui * pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	
	switch (pRui->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_W)) // ╬у
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // аб
			{
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
			{
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // ╣з
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // аб
			{
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л 
			{
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // аб
		{
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
		{
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Down(DIK_SPACE)) // а║га
		{
			_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_UP)) // ╬у
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
			{
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
			{
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ╣з
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
			{
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л 
			{
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
		{
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
		{
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Down(DIK_LCONTROL)) // а║га
		{
			_vector vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();

		break;
	}

	return nullptr;
}

CRuiState * CIdleState::Tick(CRui * pRui, _float fTimeDelta)
{



	return nullptr;
}

CRuiState * CIdleState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CIdleState::Enter(CRui * pRui)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_IDLE);
	pRui->Set_AnimIndex(CRui::ANIM_IDLE);
}

void CIdleState::Exit(CRui * pRui)
{
}
