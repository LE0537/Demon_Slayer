#include "stdafx.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "RuiMoveState.h"
#include "RuiAtk_1_State.h"

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
		if (pGameInstance->Key_Pressing(DIK_W)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // ��
			{
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ��
			{
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // ��
			{
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // �� 
			{
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // ��
		{
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_D)) // ��
		{
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}

		else if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_UP)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
			{
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
			{
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ��
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
			{
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // �� 
			{
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			}
			else
			{
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			}
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
		{
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
		{
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		}

		else if (pGameInstance->Key_Down(DIK_J))
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
