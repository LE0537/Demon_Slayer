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
#include "Effect_Manager.h"


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
		else if (pGameInstance->Key_Down(DIK_Q) && pRui->Get_PlayerInfo().iUnicCount > 0 && pRui->Get_PlayerInfo().iPowerIndex < 2)
		{
			pRui->Set_UnicCount(-1);
			if (pRui->Get_PlayerInfo().iPowerIndex == 0)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_UltUseCount(1, 0);
				RELEASE_INSTANCE(CUI_Manager);
				pRui->Set_PowerIndex(1);
				pRui->Set_PowerUp(1.5f);
				pRui->Set_PowerUpTime(6.f);
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pRui);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pRui);
				RELEASE_INSTANCE(CEffect_Manager);
			}
			else if (pRui->Get_PlayerInfo().iPowerIndex == 1)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_UltUseCount(1, 0);
				RELEASE_INSTANCE(CUI_Manager);
				pRui->Set_PowerIndex(2);
				pRui->Set_PowerUpTime(6.f);
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pRui);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pRui);
				RELEASE_INSTANCE(CEffect_Manager);
			}
		}
		else if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_O) && pRui->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_L))
			return new CTargetRushState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pRui->Set_SkillBar(-200);
					return new CSkill_SphereState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pRui->Set_SkillBar(-200);
					return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Down(DIK_U))
		{
			if (pRui->Get_PlayerInfo().iFriendBar >= 500)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_FriendUseCount(1, 0);
				RELEASE_INSTANCE(CUI_Manager);
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
		else if (pGameInstance->Key_Down(DIK_RCONTROL) && pRui->Get_PlayerInfo().iUnicCount > 0 && pRui->Get_PlayerInfo().iPowerIndex < 2)
		{
			pRui->Set_UnicCount(-1);
			if (pRui->Get_PlayerInfo().iPowerIndex == 0)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_UltUseCount(1, 1);
				RELEASE_INSTANCE(CUI_Manager);
				pRui->Set_PowerIndex(1);
				pRui->Set_PowerUp(1.5f);
				pRui->Set_PowerUpTime(6.f);
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pRui);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pRui);
				RELEASE_INSTANCE(CEffect_Manager);
			}
			else if (pRui->Get_PlayerInfo().iPowerIndex == 1)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_UltUseCount(1, 1);
				RELEASE_INSTANCE(CUI_Manager);
				pRui->Set_PowerIndex(2);
				pRui->Set_PowerUpTime(6.f);
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pRui);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pRui);
				RELEASE_INSTANCE(CEffect_Manager);
			}
		}
		else if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Pressing(DIK_C) && pRui->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT)) 
			return new CTargetRushState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pRui->Set_SkillBar(-200);
					return new CSkill_SphereState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pRui->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pRui->Set_SkillBar(-200);
					return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Down(DIK_V))
		{
			if (pRui->Get_PlayerInfo().iFriendBar >= 500)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_FriendUseCount(1, 1);
				RELEASE_INSTANCE(CUI_Manager);
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

	pRui->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pRui->Get_NavigationHeight().y)
	{
		pRui->Set_PlayerOriginPosY(fTimeDelta);
	}
	if (pRui->Get_PlayerInfo().iGuard < pRui->Get_PlayerInfo().iMaxGuard)
	{
		pRui->Set_GuardHp(1);
		if (pRui->Get_PlayerInfo().iGuard > pRui->Get_PlayerInfo().iMaxGuard)
			pRui->Set_ResetGuardHp();
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	if (!pRui->Get_PlayerInfo().bChange)
	{
		m_fSkillBarTime += fTimeDelta;
		switch (pRui->Get_i1P())
		{
		case 1:
			if (pRui->Get_PlayerInfo().iUnicCount < 3 && pRui->Get_PlayerInfo().iUnicBar < pRui->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_H))
			{
				pRui->Set_UnicBar(10);
				if (pRui->Get_PlayerInfo().iUnicBar >= pRui->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pRui->Get_PlayerInfo().iUnicCount < 3)
					{
						pRui->Reset_UnicBar();
						pRui->Set_UnicCount(1);
					}
					else
						pRui->Set_UnicBar(pRui->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pRui);
					m_fSkillBarTime = 0.f;
				}
			}
			break;
		case 2:
			if (pRui->Get_PlayerInfo().iUnicCount < 3 && pRui->Get_PlayerInfo().iUnicBar < pRui->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_B))
			{
				pRui->Set_UnicBar(10);
				if (pRui->Get_PlayerInfo().iUnicBar >= pRui->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pRui->Get_PlayerInfo().iUnicCount < 3)
					{
						pRui->Reset_UnicBar();
						pRui->Set_UnicCount(1);
					}
					else
						pRui->Set_UnicBar(pRui->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pRui);
					m_fSkillBarTime = 0.f;
				}
			}
			break;
		}
	}
	RELEASE_INSTANCE(CEffect_Manager);
	RELEASE_INSTANCE(CGameInstance);
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
