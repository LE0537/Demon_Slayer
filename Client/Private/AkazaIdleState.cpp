#include "stdafx.h"
#include "AkazaIdleState.h"
#include "AkazaMoveState.h"
#include "GameInstance.h"
#include "AkazaAtk_1_State.h"
#include "AkazaJumpState.h"
#include "AkazaGuardState.h"
#include "AkazaHitState.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaAdvSkill_Common.h"
#include "AkazaTargetRushState.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"
using namespace Akaza;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CAkazaState * CIdleState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pAkaza->Set_bGuard(false);
	
	switch (pAkaza->Get_i1P())
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
		else if (pGameInstance->Key_Down(DIK_Q) && pAkaza->Get_PlayerInfo().iUnicCount > 0 && pAkaza->Get_PlayerInfo().iPowerIndex < 2)
		{
			pAkaza->Set_UnicCount(-1);
			if (pAkaza->Get_PlayerInfo().iPowerIndex == 0)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_UltUseCount(1, 0);
				RELEASE_INSTANCE(CUI_Manager);
				pAkaza->Set_PowerIndex(1);
				pAkaza->Set_PowerUp(1.5f);
				pAkaza->Set_PowerUpTime(6.f);
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pAkaza);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pAkaza);
				RELEASE_INSTANCE(CEffect_Manager);
			}
			else if (pAkaza->Get_PlayerInfo().iPowerIndex == 1)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_UltUseCount(1, 0);
				RELEASE_INSTANCE(CUI_Manager);
				pAkaza->Set_PowerIndex(2);
				pAkaza->Set_PowerUpTime(6.f);
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pAkaza);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pAkaza);
				RELEASE_INSTANCE(CEffect_Manager);
			}
		}
		else if (pGameInstance->Key_Down(DIK_J))
			return new CAtk_1_State();
		else if (pGameInstance->Key_Down(DIK_SPACE)) // ����
		{
			_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}
		else if (pGameInstance->Key_Pressing(DIK_O) && pAkaza->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_L))
			return new CTargetRushState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_I))
		{
			if (pGameInstance->Key_Down(DIK_O))
			{
				if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
					return new CSkill_DestoryState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
					return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_U))
		{
			if (500 <= pAkaza->Get_PlayerInfo().iFriendBar)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_FriendUseCount(1, 0);
				RELEASE_INSTANCE(CUI_Manager);
				pAkaza->Set_FriendSkillBar(-500.f);
				return new CAdvSkill_CommmonState(STATE_TYPE::TYPE_START);
			}
		}


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
		else if (pGameInstance->Key_Down(DIK_RCONTROL) && pAkaza->Get_PlayerInfo().iUnicCount > 0 && pAkaza->Get_PlayerInfo().iPowerIndex < 2)
		{
			pAkaza->Set_UnicCount(-1);
			if (pAkaza->Get_PlayerInfo().iPowerIndex == 0)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_UltUseCount(1, 1);
				RELEASE_INSTANCE(CUI_Manager);
				pAkaza->Set_PowerIndex(1);
				pAkaza->Set_PowerUp(1.5f);
				pAkaza->Set_PowerUpTime(6.f);
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pAkaza);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER, pAkaza);
				RELEASE_INSTANCE(CEffect_Manager);
			}
			else if (pAkaza->Get_PlayerInfo().iPowerIndex == 1)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_UltUseCount(1, 1);
				RELEASE_INSTANCE(CUI_Manager);
				pAkaza->Set_PowerIndex(2);
				pAkaza->Set_PowerUpTime(6.f);
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP, pAkaza);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT, pAkaza);
				RELEASE_INSTANCE(CEffect_Manager);
			}
		}
		else if (pGameInstance->Key_Down(DIK_Z))
			return new CAtk_1_State();

		else if (pGameInstance->Key_Down(DIK_LCONTROL)) // ����
		{
			_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CJumpState(STATE_TYPE::TYPE_START, fPositionY, 0.f);
		}

		else if (pGameInstance->Key_Pressing(DIK_C) && pAkaza->Get_PlayerInfo().fGuardTime <= 0.f)
			return new CGuardState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CTargetRushState(STATE_TYPE::TYPE_START);
		else if (pGameInstance->Key_Down(DIK_X))
		{
			if (pGameInstance->Key_Down(DIK_C))
			{
				if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
					return new CSkill_DestoryState(STATE_TYPE::TYPE_START);
				}
			}
			else
			{
				if (200 <= pAkaza->Get_PlayerInfo().iSkBar)
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
					return new CSkill_ShootState(STATE_TYPE::TYPE_START);
				}
			}	
		}
		else if (pGameInstance->Key_Pressing(DIK_V))
		{
			if (500 <= pAkaza->Get_PlayerInfo().iFriendBar)
			{
				CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
				pUI_Manager->Set_FriendUseCount(1, 1);
				RELEASE_INSTANCE(CUI_Manager);
				pAkaza->Set_FriendSkillBar(-500.f);
				return new CAdvSkill_CommmonState(STATE_TYPE::TYPE_START);
			}
		}

	}

	return nullptr;
}

CAkazaState * CIdleState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (pAkaza->Get_PlayerInfo().bSub)
	{
		pAkaza->Set_Change(true, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
	}


	return nullptr;
}

CAkazaState * CIdleState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	_vector vPlayerY = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pAkaza->Set_NavigationHeight(vPlayerY);
	if (vPlayerY.m128_f32[1] > pAkaza->Get_NavigationHeight().y)
	{
		pAkaza->Set_PlayerOriginPosY(fTimeDelta);
	}
	if (pAkaza->Get_PlayerInfo().iGuard < pAkaza->Get_PlayerInfo().iMaxGuard)
	{
		pAkaza->Set_GuardHp(1);
		if (pAkaza->Get_PlayerInfo().iGuard > pAkaza->Get_PlayerInfo().iMaxGuard)
			pAkaza->Set_ResetGuardHp();
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	if (!pAkaza->Get_PlayerInfo().bChange)
	{
		m_fSkillBarTime += fTimeDelta;
		switch (pAkaza->Get_i1P())
		{
		case 1:
			if (pAkaza->Get_PlayerInfo().iUnicCount < 3 && pAkaza->Get_PlayerInfo().iUnicBar < pAkaza->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_H))
			{
				pAkaza->Set_UnicBar(10);
				if (pAkaza->Get_PlayerInfo().iUnicBar >= pAkaza->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pAkaza->Get_PlayerInfo().iUnicCount < 3)
					{
						pAkaza->Reset_UnicBar();
						pAkaza->Set_UnicCount(1);
					}
					else
						pAkaza->Set_UnicBar(pAkaza->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pAkaza);
					m_fSkillBarTime = 0.f;
				}
			}
			break;
		case 2:
			if (pAkaza->Get_PlayerInfo().iUnicCount < 3 && pAkaza->Get_PlayerInfo().iUnicBar < pAkaza->Get_PlayerInfo().iUnicMaxBar && pGameInstance->Key_Pressing(DIK_B))
			{
				pAkaza->Set_UnicBar(10);
				if (pAkaza->Get_PlayerInfo().iUnicBar >= pAkaza->Get_PlayerInfo().iUnicMaxBar)
				{
					if (pAkaza->Get_PlayerInfo().iUnicCount < 3)
					{
						pAkaza->Reset_UnicBar();
						pAkaza->Set_UnicCount(1);
					}
					else
						pAkaza->Set_UnicBar(pAkaza->Get_PlayerInfo().iUnicMaxBar);
				}
				if (m_fSkillBarTime > 0.3f)
				{
					pEffectManger->Create_Effect(CEffect_Manager::EFF_POWER_CHARGING, pAkaza);
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

void CIdleState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_IDLE);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_IDLE);
	pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_IDLE, 0.05f);
}

void CIdleState::Exit(CAkaza* pAkaza)
{
}
