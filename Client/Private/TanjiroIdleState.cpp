#include "stdafx.h"
#include "TanjiroIdlestate.h"
#include "TanjiroMoveState.h"
#include "TanjiroJumpstate.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "TanjiroAtk_1_State.h"
#include "TanjiroKaguraAtk_1_State.h"
#include "TanjiroGuardState.h"
#include "TanjiroGuardHitState.h"
#include "TanjiroSkill_WaterMill.h"
#include "TanjiroSkill_Common.h"
#include "TanjiroSkill_WindMill.h"
#include "TanjiroKaguraSkill_Common.h"
#include "TanjiroKaguraSkill_Sphere.h"
#include "TanjiroChangeState.h"
#include "TanjiroTargetRushState.h"
#include "HinoCami_CinemaState.h"
#include "ImGuiManager.h"
#include "TanjiroSplSkrStartState.h"
using namespace Tanjiro;



CIdleState::CIdleState(STATE_ID eState)
	: ePreState(eState)
{
}

CTanjiroState * CIdleState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pTanjiro->Set_bGuard(false);
	pTanjiro->Set_SplSkl(false);

	//if (pGameInstance->Key_Down(DIK_2) && pTanjiro->Get_StoryKey())
	//{
	//	_vector vPos = { -335.479f,42.501f,-328.243f,1.f };
	//	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
	//	pTanjiro->Set_NavigationHeight(vPos);
	//	pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos);
	//}


	if (!pTanjiro->Get_PlayerInfo().bChange && g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		switch (pTanjiro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E) && !pTanjiro->Get_StoryKey())
			{
			//	pTanjiro->Get_BattleTarget()->Play_Scene();
				return new CSplSkrStartState(TYPE_START);
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
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else if (pGameInstance->Key_Down(DIK_Q) && pTanjiro->Get_PlayerInfo().iUnicCount > 0 && pTanjiro->Get_PlayerInfo().iPowerIndex < 2)
			{
				pTanjiro->Set_KaguraMode(true);
				pTanjiro->Set_UnicCount(-1);
				if (pTanjiro->Get_PlayerInfo().iPowerIndex == 0)
				{
					pTanjiro->Set_PowerIndex(1);
					pTanjiro->Set_PowerUp(1.5f);
					pTanjiro->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pTanjiro);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pTanjiro);
					RELEASE_INSTANCE(CEffect_Manager);
				}
				else if (pTanjiro->Get_PlayerInfo().iPowerIndex == 1)
				{
					pTanjiro->Set_PowerIndex(2);
					pTanjiro->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pTanjiro);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pTanjiro);
					RELEASE_INSTANCE(CEffect_Manager);
				}
			}
			else if (pGameInstance->Key_Down(DIK_J))
			{
				if (pTanjiro->Get_KaguraMode())
				{
					return new CAtk_1_KaguraState();
				}
				else
					return new CAtk_1_State();
			}
			else if (pGameInstance->Key_Pressing(DIK_O) && pTanjiro->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_L))
				return new CTargetRushState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_I))
			{
				if (pGameInstance->Key_Down(DIK_O))
				{
					if (pTanjiro->Get_KaguraMode())
					{
						if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
						{
							pTanjiro->Set_SkillBar(-200);
							return new CKaguraSkill_SphereState();
						}
					}
					else
					{
						//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL);
						if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
						{
							pTanjiro->Set_SkillBar(-200);
							pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_0);
							pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_1);
							pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_2);
			
							return new CSkill_WindMillState(TYPE_START);
						}
					}
				}
				else
				{
					if (pTanjiro->Get_KaguraMode())
					{
						pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_KAGURA_COMMON);
						if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
						{
							pTanjiro->Set_SkillBar(-200);
							return new CKaguraSkill_CommonState();
						}
					}
					else
					{
						if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
						{
							pTanjiro->Set_SkillBar(-200);
							return new CSkill_CommonState();
						}
					}
				}
			}
			break;
		case 2:
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
				_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
			}
			else if (pGameInstance->Key_Down(DIK_RCONTROL) && pTanjiro->Get_PlayerInfo().iUnicCount > 0 && pTanjiro->Get_PlayerInfo().iPowerIndex < 2)
			{
				pTanjiro->Set_KaguraMode(true);
				pTanjiro->Set_UnicCount(-1);
				if (pTanjiro->Get_PlayerInfo().iPowerIndex == 0)
				{
					pTanjiro->Set_PowerIndex(1);
					pTanjiro->Set_PowerUp(1.5f);
					pTanjiro->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pTanjiro);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pTanjiro);
					RELEASE_INSTANCE(CEffect_Manager);
				}
				else if (pTanjiro->Get_PlayerInfo().iPowerIndex == 1)
				{
					pTanjiro->Set_PowerIndex(2);
					pTanjiro->Set_PowerUpTime(10.f);
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pTanjiro);
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pTanjiro);
					RELEASE_INSTANCE(CEffect_Manager);
				}
			}
			else if (pGameInstance->Key_Down(DIK_Z))
			{
				if (pTanjiro->Get_KaguraMode())
					return new CAtk_1_KaguraState();
				else
					return new CAtk_1_State();
			}
			else if (pGameInstance->Key_Pressing(DIK_C) && pTanjiro->Get_PlayerInfo().fGuardTime <= 0.f)
				return new CGuardState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CTargetRushState(STATE_TYPE::TYPE_START);
			else if (pGameInstance->Key_Down(DIK_X))
			{
				if (pGameInstance->Key_Down(DIK_C))
				{
					if (pTanjiro->Get_KaguraMode())
					{
						if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
						{
							pTanjiro->Set_SkillBar(-200);
							return new CKaguraSkill_SphereState();
						}
					}
					else
					{
						//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL);
						if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
						{
							pTanjiro->Set_SkillBar(-200);
							pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_0);
							pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_1);
							pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_WINDMILL_2);
							return new CSkill_WindMillState(TYPE_START);
						}
					}
				}
				else
				{
					if (pTanjiro->Get_KaguraMode())
					{
						if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
						{
							pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_SKILL_KAGURA_COMMON);
							pTanjiro->Set_SkillBar(-200);
							return new CKaguraSkill_CommonState();
						}
					}
					else
					{
						if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
						{
							pTanjiro->Set_SkillBar(-200);
							return new CSkill_CommonState();
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else
	{

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
			_vector vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpstate(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}
		
	}
	return nullptr;
}

CTanjiroState * CIdleState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (pTanjiro->Get_PlayerInfo().bChange)
	{
		return new CChangeState(STATE_TYPE::TYPE_LOOP);
	}
	if (pTanjiro->Get_PlayerInfo().bSub)
	{
		return new CChangeState(STATE_TYPE::TYPE_START);
	}

	if (ePreState == STATE_HIT || ePreState == STATE_MOVE)
	{
		if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
		{
			if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
			{
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				return new CIdleState(STATE_IDLE);
			}
		}
		else
		{
			if (pTanjiro->Get_ModelADV()->Get_End(pTanjiro->Get_ADVAnimIndex()))
			{
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
				return new CIdleState(STATE_IDLE);
			}
		}
	}

	_float3 vPosition; XMStoreFloat3(&vPosition, pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	CImGuiManager::Get_Instance()->Set_CurrentPosition(vPosition);


	return nullptr;
}

CTanjiroState * CIdleState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	//if (ePreState == CTanjiroState::STATE_JUMP)
	//{
	//	pTanjiro->Get_Model()->Play_Animation2(fTimeDelta);
	//}
	//else
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	else
		pTanjiro->Get_ModelADV()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pTanjiro->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pTanjiro->Get_NavigationHeight().y + 0.1f)
	{
		pTanjiro->Set_PlayerOriginPosY(fTimeDelta);
	}

	//	vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
	//	if (vPlayerY.m128_f32[1] < pTanjiro->Get_NavigationHeight().y)
	//		vPlayerY.m128_f32[1] = pTanjiro->Get_NavigationHeight().y;

	//	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	//}
	if (pTanjiro->Get_PlayerInfo().iGuard < pTanjiro->Get_PlayerInfo().iMaxGuard)
	{
		pTanjiro->Set_GuardHp(1);
		if (pTanjiro->Get_PlayerInfo().iGuard > pTanjiro->Get_PlayerInfo().iMaxGuard)
			pTanjiro->Set_ResetGuardHp();
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	if (!pTanjiro->Get_PlayerInfo().bChange)
	{
		m_fSkillBarTime += fTimeDelta;
		switch (pTanjiro->Get_i1P())
		{
		case 1:
			if (pTanjiro->Get_PlayerInfo().iUnicCount < 3 && pTanjiro->Get_PlayerInfo().iUnicBar < pTanjiro->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_H))
			{
				pTanjiro->Set_UnicBar(10);
				if (pTanjiro->Get_PlayerInfo().iUnicBar >= pTanjiro->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pTanjiro->Get_PlayerInfo().iUnicCount < 3)
					{
						pTanjiro->Reset_UnicBar();
						pTanjiro->Set_UnicCount(1);
					}
					else
						pTanjiro->Set_UnicBar(pTanjiro->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pTanjiro);
					m_fSkillBarTime = 0.f;
				}
			}
			break;
		case 2:
			if (pTanjiro->Get_PlayerInfo().iUnicCount < 3 && pTanjiro->Get_PlayerInfo().iUnicBar < pTanjiro->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_B))
			{
				pTanjiro->Set_UnicBar(10);
				if (pTanjiro->Get_PlayerInfo().iUnicBar >= pTanjiro->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pTanjiro->Get_PlayerInfo().iUnicCount < 3)
					{
						pTanjiro->Reset_UnicBar();
						pTanjiro->Set_UnicCount(1);
					}
					else
						pTanjiro->Set_UnicBar(pTanjiro->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pTanjiro);
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

void CIdleState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_IDLE;
	
	//if (ePreState == STATE_HIT)
	//{
	//	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN);
	//	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_RETURN);
	//	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN);
	//	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN, 0.01f);
	//}
	//else
	if (ePreState == STATE_MOVE)
	{
		if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
		{
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_END);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_END);
			pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_MOVE_END, 0.01f);
		}
		else
		{
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(6);
			pTanjiro->Set_ADVAnimIndex(6);
			pTanjiro->Get_ModelADV()->Set_Loop(pTanjiro->Get_ADVAnimIndex());
			pTanjiro->Get_ModelADV()->Set_LinearTime(6, 0.01f);
		}
	}
	else
	{
		if (ePreState == STATE_HIT)
			ePreState = STATE_IDLE;

		pTanjiro->Set_GodMode(false);
		//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_IDLE);
		if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
		{
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_IDLE);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_IDLE);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_IDLE, 0.05f);
			pTanjiro->Get_Model()->Set_FrameNum(pTanjiro->Get_AnimIndex(), 100);
		}
		else
		{
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(4);
			pTanjiro->Set_ADVAnimIndex(4);
			pTanjiro->Get_ModelADV()->Set_LinearTime(4, 0.05f);
			pTanjiro->Get_ModelADV()->Set_FrameNum(pTanjiro->Get_ADVAnimIndex(), 100);
		}
	}

}

void CIdleState::Exit(CTanjiro * pTanjiro)
{
}

