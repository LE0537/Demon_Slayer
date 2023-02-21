#include "stdafx.h"
#include "NezukoIdleState.h"
#include "Nezuko.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "NezukoAtk_1_State.h"
#include "NezukoMoveState.h"
#include "NezukoGuardState.h"
#include "NezukojumpState.h"
#include "NezukoSkill_Common.h"
#include "NezukoSkill_FallCut.h"
#include "NezukoChangeState.h"
#include "NezukoDashState.h"
#include "NezukoTargetRushState.h"
#include "UI_Manager.h"
#include "Nezuko_CinemaState.h"
using namespace Nezuko;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CNezukoState * CIdleState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pNezuko->Set_bGuard(false);
	if (!pNezuko->Get_PlayerInfo().bChange)
	{
		switch (pNezuko->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E))
			{
				return new CNezuko_CinemaState(CNezuko_CinemaState::SCENE_START);
			}
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
			else if (pGameInstance->Key_Down(DIK_Q) && pNezuko->Get_PlayerInfo().iUnicCount > 0 && pNezuko->Get_PlayerInfo().iPowerIndex < 2)
			{
				pNezuko->Set_UnicCount(-1);
				if (pNezuko->Get_PlayerInfo().iPowerIndex == 0)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UltUseCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pNezuko->Set_PowerIndex(1);
					pNezuko->Set_PowerUp(1.5f);
					pNezuko->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pNezuko);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pNezuko);
					RELEASE_INSTANCE(CEffect_Manager);
				}
				else if (pNezuko->Get_PlayerInfo().iPowerIndex == 1)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UltUseCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pNezuko->Set_PowerIndex(2);
					pNezuko->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pNezuko);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pNezuko);
					RELEASE_INSTANCE(CEffect_Manager);
				}
			}
			else if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();
			else if (pGameInstance->Key_Down(DIK_SPACE)) // а║га
			{
				_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else if (pGameInstance->Key_Pressing(DIK_O) && pNezuko->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_L))
				return new CTargetRushState(STATE_TYPE::TYPE_START);

			else if (pGameInstance->Key_Down(DIK_I))
			{
				if (pGameInstance->Key_Down(DIK_O))
				{
					if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_FallCutState(STATE_TYPE::TYPE_START);
					}
				}
				else
				{
					if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_CommonState(STATE_TYPE::TYPE_START);
					}
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
			else if (pGameInstance->Key_Down(DIK_RCONTROL) && pNezuko->Get_PlayerInfo().iUnicCount > 0 && pNezuko->Get_PlayerInfo().iPowerIndex < 2)
			{
				pNezuko->Set_UnicCount(-1);
				if (pNezuko->Get_PlayerInfo().iPowerIndex == 0)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UltUseCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pNezuko->Set_PowerIndex(1);
					pNezuko->Set_PowerUp(1.5f);
					pNezuko->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pNezuko);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pNezuko);
					RELEASE_INSTANCE(CEffect_Manager);
				}
				else if (pNezuko->Get_PlayerInfo().iPowerIndex == 1)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UltUseCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pNezuko->Set_PowerIndex(2);
					pNezuko->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pNezuko);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pNezuko);
					RELEASE_INSTANCE(CEffect_Manager);
				}
			}
			else if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			else if (pGameInstance->Key_Down(DIK_LCONTROL)) // а║га
			{
				_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}

			else if (pGameInstance->Key_Pressing(DIK_C) && pNezuko->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CTargetRushState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_X))
			{
				if (pGameInstance->Key_Down(DIK_C))
				{
					if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_FallCutState(STATE_TYPE::TYPE_START);
					}
				}
				else
				{
					if (200 <= pNezuko->Get_PlayerInfo().iSkBar)
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pNezuko->Set_SkillBar(-200);
						return new CSkill_CommonState(STATE_TYPE::TYPE_START);
					}
				}
			}

		}
	}
	return nullptr;
}

CNezukoState * CIdleState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (pNezuko->Get_PlayerInfo().bChange)
	{
		return new CChangeState(STATE_TYPE::TYPE_LOOP);
	}
	if (pNezuko->Get_PlayerInfo().bSub)
	{
		return new CChangeState(STATE_TYPE::TYPE_START);
	}

	if (m_ePreState == STATE_MOVE)
	{
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState(STATE_IDLE);
		}
	}

	return nullptr;
}

CNezukoState * CIdleState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pNezuko->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pNezuko->Get_NavigationHeight().y)
	{
		pNezuko->Set_PlayerOriginPosY(fTimeDelta);
	}
	if (pNezuko->Get_PlayerInfo().iGuard < pNezuko->Get_PlayerInfo().iMaxGuard)
	{
		pNezuko->Set_GuardHp(1);
		if (pNezuko->Get_PlayerInfo().iGuard > pNezuko->Get_PlayerInfo().iMaxGuard)
			pNezuko->Set_ResetGuardHp();
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	if (!pNezuko->Get_PlayerInfo().bChange)
	{
		m_fSkillBarTime += fTimeDelta;
		switch (pNezuko->Get_i1P())
		{
		case 1:
			if (pNezuko->Get_PlayerInfo().iUnicCount < 3 && pNezuko->Get_PlayerInfo().iUnicBar < pNezuko->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_H))
			{
				pNezuko->Set_UnicBar(10);
				if (pNezuko->Get_PlayerInfo().iUnicBar >= pNezuko->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pNezuko->Get_PlayerInfo().iUnicCount < 3)
					{
						pNezuko->Reset_UnicBar();
						pNezuko->Set_UnicCount(1);
					}
					else
						pNezuko->Set_UnicBar(pNezuko->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pNezuko);
					m_fSkillBarTime = 0.f;
				}
			}
			break;
		case 2:
			if (pNezuko->Get_PlayerInfo().iUnicCount < 3 && pNezuko->Get_PlayerInfo().iUnicBar < pNezuko->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_B))
			{
				pNezuko->Set_UnicBar(10);
				if (pNezuko->Get_PlayerInfo().iUnicBar >= pNezuko->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pNezuko->Get_PlayerInfo().iUnicCount < 3)
					{
						pNezuko->Reset_UnicBar();
						pNezuko->Set_UnicCount(1);
					}
					else
						pNezuko->Set_UnicBar(pNezuko->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pNezuko);
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

void CIdleState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_IDLE;


	if (m_ePreState == STATE_MOVE)
	{
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_MOVE_END);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_MOVE_END);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_MOVE_END, 0.01f);
	}
	else
	{
		if (m_ePreState == STATE_HIT)
			m_ePreState = STATE_IDLE;

		pNezuko->Set_GodMode(false);
		//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_IDLE);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_IDLE);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_IDLE);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_IDLE, 0.05f);
		pNezuko->Get_Model()->Set_FrameNum(pNezuko->Get_AnimIndex(), 100);
	}
}

void CIdleState::Exit(CNezuko* pNezuko)
{
}
