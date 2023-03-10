#include "stdafx.h"
#include "ShinobuIdlestate.h"
#include "ShinobuMoveState.h"
#include "ShinobuJumpstate.h"
#include "GameInstance.h"

#include "ShinobuAtk_1_State.h"

#include "ShinobuGuardState.h"
#include "ShinobuGuardHitState.h"
#include "ShinobuSkill_Common.h"
#include "ShinobuChangeState.h"
#include "ShinobuSkill_Upper.h"
#include "ShinobuTargetRushState.h"
#include "Effect_Manager.h"
#include "Shinobu_CinemaState.h"
#include "ShinobuSplSkrStartState.h"


using namespace Shinobu;



CIdleState::CIdleState(STATE_ID eState)
	: ePreState(eState)
{
	m_ePreState = eState;
}

CShinobuState * CIdleState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pShinobu->Set_bGuard(false);
	pShinobu->Set_SplSkl(false);
	if (!pShinobu->Get_PlayerInfo().bChange)
	{
		switch (pShinobu->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E) && g_iLevel == LEVEL_GAMEPLAY)
			{
				if (pShinobu->Get_PlayerInfo().iUnicCount > 0)
				{
					pShinobu->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Pressing(DIK_W)) // ╬у
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // аб
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
				else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
					return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}

			else if (pGameInstance->Key_Pressing(DIK_D)) // ╣з
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // аб
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
				else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л 
					return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
				else
					return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
			}


			else if (pGameInstance->Key_Pressing(DIK_A)) // аб
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_S)) // ©Л
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_SPACE)) // а║га
			{
				_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else if (pGameInstance->Key_Down(DIK_Q) && pShinobu->Get_PlayerInfo().iUnicCount > 0 && pShinobu->Get_PlayerInfo().iPowerIndex < 2)
			{
				pShinobu->Set_UnicCount(-1);
				if (pShinobu->Get_PlayerInfo().iPowerIndex == 0)
				{
					pShinobu->Set_PowerIndex(1);
					pShinobu->Set_PowerUp(1.5f);
					pShinobu->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pShinobu);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_SI, pShinobu);
					RELEASE_INSTANCE(CEffect_Manager);
				}
				else if (pShinobu->Get_PlayerInfo().iPowerIndex == 1)
				{
					pShinobu->Get_Effect()->Set_Dead();
					pShinobu->Set_PowerIndex(2);
					pShinobu->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pShinobu);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PFT_SI, pShinobu);
					RELEASE_INSTANCE(CEffect_Manager);
				}
			}
			else if (pGameInstance->Key_Down(DIK_J))
			{
				return new CAtk_1_State();
			}
			else if (pGameInstance->Key_Pressing(DIK_O) && pShinobu->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_L))
				return new CTargetRushState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_I))
			{
				if (pGameInstance->Key_Down(DIK_O))
				{

					if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_UpperState(STATE_TYPE::TYPE_START);
					}

				}
				else
				{

					if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_CommonState();
					}

				}
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_RSHIFT) && g_iLevel == LEVEL_GAMEPLAY)
			{
				if (pShinobu->Get_PlayerInfo().iUnicCount > 0)
				{
					pShinobu->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}
			if (pGameInstance->Key_Pressing(DIK_UP)) // ╬у
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
					return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
					return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
				else
					return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
			}

			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ╣з
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
					return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л 
					return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
				else
					return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
			}


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
				return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ©Л
				return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_LCONTROL)) // а║га
			{
				_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else if (pGameInstance->Key_Down(DIK_RCONTROL) && pShinobu->Get_PlayerInfo().iUnicCount > 0 && pShinobu->Get_PlayerInfo().iPowerIndex < 2)
			{
				pShinobu->Set_UnicCount(-1);
				if (pShinobu->Get_PlayerInfo().iPowerIndex == 0)
				{
					pShinobu->Set_PowerIndex(1);
					pShinobu->Set_PowerUp(1.5f);
					pShinobu->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pShinobu);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_SI, pShinobu);
					RELEASE_INSTANCE(CEffect_Manager);
				}
				else if (pShinobu->Get_PlayerInfo().iPowerIndex == 1)
				{
					pShinobu->Get_Effect()->Set_Dead();
					pShinobu->Set_PowerIndex(2);
					pShinobu->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pShinobu);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PFT_SI, pShinobu);
					RELEASE_INSTANCE(CEffect_Manager);
				}
			}
			else if (pGameInstance->Key_Down(DIK_Z))
			{
				return new CAtk_1_State();
			}
			else if (pGameInstance->Key_Pressing(DIK_C) && pShinobu->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CTargetRushState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_X))
			{
				if (pGameInstance->Key_Down(DIK_C))
				{

					if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_UpperState(STATE_TYPE::TYPE_START);
					}

				}
				else
				{

					if (200 <= pShinobu->Get_PlayerInfo().iSkBar)
					{
						pShinobu->Set_SkillBar(-200);
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

CShinobuState * CIdleState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (pShinobu->Get_PlayerInfo().bChange)
	{
		return new CChangeState(STATE_TYPE::TYPE_LOOP);
	}
	if (pShinobu->Get_PlayerInfo().bSub)
	{
		return new CChangeState(STATE_TYPE::TYPE_START);
	}

	if (ePreState == STATE_MOVE)
	{
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState(STATE_IDLE);
		}
	}
	return nullptr;
}

CShinobuState * CIdleState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (ePreState == CShinobuState::STATE_JUMP)
	{
		pShinobu->Get_Model()->Play_Animation2(fTimeDelta);
	}
	else
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pShinobu->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pShinobu->Get_NavigationHeight().y + 0.1f)
	{
		pShinobu->Set_PlayerOriginPosY(fTimeDelta);
	}
	if (pShinobu->Get_PlayerInfo().iGuard < pShinobu->Get_PlayerInfo().iMaxGuard)
	{
		pShinobu->Set_GuardHp(1);
		if (pShinobu->Get_PlayerInfo().iGuard > pShinobu->Get_PlayerInfo().iMaxGuard)
			pShinobu->Set_ResetGuardHp();
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	if (!pShinobu->Get_PlayerInfo().bChange)
	{
		m_fSkillBarTime += fTimeDelta;
		switch (pShinobu->Get_i1P())
		{
		case 1:
			if (pShinobu->Get_PlayerInfo().iUnicCount < 3 && pShinobu->Get_PlayerInfo().iUnicBar < pShinobu->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_H))
			{
				pShinobu->Set_UnicBar(10);
				if (pShinobu->Get_PlayerInfo().iUnicBar >= pShinobu->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pShinobu->Get_PlayerInfo().iUnicCount < 3)
					{
						pShinobu->Reset_UnicBar();
						pShinobu->Set_UnicCount(1);
					}
					else
						pShinobu->Set_UnicBar(pShinobu->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pShinobu);
					m_fSkillBarTime = 0.f;
				}
			}
			break;
		case 2:
			if (pShinobu->Get_PlayerInfo().iUnicCount < 3 && pShinobu->Get_PlayerInfo().iUnicBar < pShinobu->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_B))
			{
				pShinobu->Set_UnicBar(10);
				if (pShinobu->Get_PlayerInfo().iUnicBar >= pShinobu->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pShinobu->Get_PlayerInfo().iUnicCount < 3)
					{
						pShinobu->Reset_UnicBar();
						pShinobu->Set_UnicCount(1);
					}
					else
						pShinobu->Set_UnicBar(pShinobu->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pShinobu);
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

void CIdleState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_IDLE;



	if (m_ePreState == STATE_MOVE)
	{
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_MOVE_END);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_MOVE_END);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_MOVE_END, 0.01f);
	}
	else
	{
		if (m_ePreState == STATE_HIT)
			m_ePreState = STATE_IDLE;

		pShinobu->Set_GodMode(false);
		//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_IDLE);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_IDLE);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_IDLE);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_IDLE, 0.05f);
		pShinobu->Get_Model()->Set_FrameNum(pShinobu->Get_AnimIndex(), 100);
	}
}

void CIdleState::Exit(CShinobu* pShinobu)
{
}

