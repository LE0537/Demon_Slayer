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
#include "KyoujuroChangeState.h"
#include "KyoujuroTargetRushState.h"
#include "Effect_Manager.h"
#include "Kyoujuro_CinemaState.h"
using namespace Kyoujuro;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CKyoujuroState * CIdleState::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pKyoujuro->Set_bGuard(false);
	if (!pKyoujuro->Get_PlayerInfo().bChange)
	{
		switch (pKyoujuro->Get_i1P())
		{
		case 1:

			if (pGameInstance->Key_Pressing(DIK_E))
			{
				return new CKyoujuro_CinemaState(CKyoujuro_CinemaState::CINEMASCENE::SCENE_START);
			}

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
			else if (pGameInstance->Key_Down(DIK_Q) && pKyoujuro->Get_PlayerInfo().iUnicCount > 0 && pKyoujuro->Get_PlayerInfo().iPowerIndex < 2)
			{
				pKyoujuro->Set_UnicCount(-1);
				if (pKyoujuro->Get_PlayerInfo().iPowerIndex == 0)
				{
					pKyoujuro->Set_PowerIndex(1);
					pKyoujuro->Set_PowerUp(1.5f);
					pKyoujuro->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pKyoujuro);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pKyoujuro);
					RELEASE_INSTANCE(CEffect_Manager);
				}
				else if (pKyoujuro->Get_PlayerInfo().iPowerIndex == 1)
				{
					pKyoujuro->Set_PowerIndex(2);
					pKyoujuro->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pKyoujuro);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pKyoujuro);
					RELEASE_INSTANCE(CEffect_Manager);
				}
			}
			else if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();
			else if (pGameInstance->Key_Pressing(DIK_O) && pKyoujuro->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_L))
				return new CTargetRushState(STATE_TYPE::TYPE_START);
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
			else if (pGameInstance->Key_Down(DIK_RCONTROL) && pKyoujuro->Get_PlayerInfo().iUnicCount > 0 && pKyoujuro->Get_PlayerInfo().iPowerIndex < 2)
			{
				pKyoujuro->Set_UnicCount(-1);
				if (pKyoujuro->Get_PlayerInfo().iPowerIndex == 0)
				{
					pKyoujuro->Set_PowerIndex(1);
					pKyoujuro->Set_PowerUp(1.5f);
					pKyoujuro->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pKyoujuro);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pKyoujuro);
					RELEASE_INSTANCE(CEffect_Manager);
				}
				else if (pKyoujuro->Get_PlayerInfo().iPowerIndex == 1)
				{
					pKyoujuro->Set_PowerIndex(2);
					pKyoujuro->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pKyoujuro);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pKyoujuro);
					RELEASE_INSTANCE(CEffect_Manager);
				}
			}
			else if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();
			else if (pGameInstance->Key_Pressing(DIK_C) && pKyoujuro->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CTargetRushState(STATE_TYPE::TYPE_START);
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
	}
	return nullptr;
}

CKyoujuroState * CIdleState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	if (pKyoujuro->Get_PlayerInfo().bChange)
	{
		return new CChangeState(STATE_TYPE::TYPE_LOOP);
	}
	if (pKyoujuro->Get_PlayerInfo().bSub)
	{
		return new CChangeState(STATE_TYPE::TYPE_START);
	}

	if (m_ePreState == STATE_MOVE)
	{
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState(STATE_IDLE);
		}
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

	_vector vPlayerY = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pKyoujuro->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pKyoujuro->Get_NavigationHeight().y)
	{
		pKyoujuro->Set_PlayerOriginPosY(fTimeDelta);
	}
	if (pKyoujuro->Get_PlayerInfo().iGuard < pKyoujuro->Get_PlayerInfo().iMaxGuard)
	{
		pKyoujuro->Set_GuardHp(1);
		if (pKyoujuro->Get_PlayerInfo().iGuard > pKyoujuro->Get_PlayerInfo().iMaxGuard)
			pKyoujuro->Set_ResetGuardHp();
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	if (!pKyoujuro->Get_PlayerInfo().bChange)
	{
		m_fSkillBarTime += fTimeDelta;
		switch (pKyoujuro->Get_i1P())
		{
		case 1:
			if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3 && pKyoujuro->Get_PlayerInfo().iUnicBar < pKyoujuro->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_H))
			{
				pKyoujuro->Set_UnicBar(10);
				if (pKyoujuro->Get_PlayerInfo().iUnicBar >= pKyoujuro->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3)
					{
						pKyoujuro->Reset_UnicBar();
						pKyoujuro->Set_UnicCount(1);
					}
					else
						pKyoujuro->Set_UnicBar(pKyoujuro->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pKyoujuro);
					m_fSkillBarTime = 0.f;
				}
			}
			break;
		case 2:
			if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3 && pKyoujuro->Get_PlayerInfo().iUnicBar < pKyoujuro->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_B))
			{
				pKyoujuro->Set_UnicBar(10);
				if (pKyoujuro->Get_PlayerInfo().iUnicBar >= pKyoujuro->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3)
					{
						pKyoujuro->Reset_UnicBar();
						pKyoujuro->Set_UnicCount(1);
					}
					else
						pKyoujuro->Set_UnicBar(pKyoujuro->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pKyoujuro);
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

void CIdleState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	if (m_ePreState == STATE_MOVE)
	{
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_MOVE_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_MOVE_END);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_MOVE_END, 0.01f);
	}
	else
	{
		if (m_ePreState == STATE_HIT)
			m_ePreState = STATE_IDLE;

		pKyoujuro->Set_GodMode(false);
		//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_IDLE);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_IDLE);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_IDLE);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_IDLE, 0.05f);
		pKyoujuro->Get_Model()->Set_FrameNum(pKyoujuro->Get_AnimIndex(), 100);
	}
}

void CIdleState::Exit(CKyoujuro * pKyoujuro)
{
}
