#include "stdafx.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "RuiMoveState.h"
#include "RuiAtk_1_State.h"
#include "RuiJumpState.h"
#include "RuiGuardState.h"
#include "RuiSkill_Shoot.h"
#include "RuiSkill_Sphere.h"
#include "RuiAdvSkill_CommonState.h"
#include "RuiTargetRushState.h"
#include "AiState.h"
using namespace Rui;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CRuiState * CIdleState::HandleInput(CRui * pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pRui->Set_bGuard(false);
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
		else if (pGameInstance->Key_Pressing(DIK_O))
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_L))
			return new CTargetRushState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_SphereState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Down(DIK_U))
		{
			if (pRui->Get_PlayerInfo().iFriendBar >= 500)
			{
				pRui->Get_Model()->Reset_Anim(CRui::ANIM_ADVSKILL_COMMON);
				pRui->Set_FriendSkillBar(-500.f);
				return new CAdvSkill_CommonState();
			}
		}

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
		else if (pGameInstance->Key_Pressing(DIK_C))
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT)) 
			return new CTargetRushState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_SphereState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					pRui->Set_SkillBar(-200);
					return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Down(DIK_V))
		{
			if (pRui->Get_PlayerInfo().iFriendBar >= 500)
			{
				pRui->Get_Model()->Reset_Anim(CRui::ANIM_ADVSKILL_COMMON);
				pRui->Set_FriendSkillBar(-500.f);
				return new CAdvSkill_CommonState();
			}
		}

		break;
	}

	return nullptr;
}

CRuiState * CIdleState::Tick(CRui * pRui, _float fTimeDelta)
{
	if (pRui->Get_PlayerInfo().bSub)
	{
		pRui->Set_Change(true, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
	}

	if (pRui->Get_IsAIMode() == true)
		return new CAiState();
	else
		return nullptr;
}

CRuiState * CIdleState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	if (vPlayerY.m128_f32[1] > 0.f)
	{
		vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
		if (vPlayerY.m128_f32[1] < 0)
			vPlayerY.m128_f32[1] = 0;

		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	}

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
