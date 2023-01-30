#include "stdafx.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroMoveState.h"
#include "KyoujuroJumpState.h"
#include "GameInstance.h"
#include "KyoujuroAtk_1_State.h"
#include "KyoujuroGuardState.h"
#include "KyoujuroGuardHitState.h"
#include "KyoujuroSkill_Common.h"
#include "KyoujuroSkill_DoubleUpper.h"

using namespace Kyoujuro;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CKyoujuroState * CIdleState::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pKyoujuro->Set_bGuard(false);

	switch (pKyoujuro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_W)) // 菊
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // 谅
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			else
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
		}

		else if (pGameInstance->Key_Pressing(DIK_S)) // 第
		{
			if (pGameInstance->Key_Pressing(DIK_A)) // 谅
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_D)) // 快 
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			else
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
		}


		else if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_SPACE))
		{
			_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_LOOP, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_O))
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{
				if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
				{
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DoubleUpperState();
				}
			}
			else
			{
				if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
				{
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_CommonState();
				}
			}
		}
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_UP)) // 菊
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
				return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
				return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
			else
				return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
		}

		else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 第
		{
			if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
				return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快 
				return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
			else
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
		}


		else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
			return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_LCONTROL))
		{
			_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_LOOP, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_C))
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{
				if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
				{
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DoubleUpperState();
				}
			}
			else
			{
				if (200 <= pKyoujuro->Get_PlayerInfo().iSkBar)
				{
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_CommonState();
				}
			}
		}
		break;
	default:
		break;
	}

	return nullptr;
}

CKyoujuroState * CIdleState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	if (pKyoujuro->Get_PlayerInfo().bSub)
	{
		pKyoujuro->Set_Change(true, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
	}


	return nullptr;
}

CKyoujuroState * CIdleState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	if (m_ePreState == CKyoujuroState::STATE_JUMP)
	{
		pKyoujuro->Get_Model()->Play_Animation2(fTimeDelta);
	}
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CIdleState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_IDLE);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_IDLE);
}

void CIdleState::Exit(CKyoujuro * pKyoujuro)
{
}
