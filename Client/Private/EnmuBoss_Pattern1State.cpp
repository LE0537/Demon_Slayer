#include "stdafx.h"
#include "..\Public\EnmuBoss_Pattern1State.h"
#include "EnmuBossIdleState.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Enmu_Chaos_Head.h"
#include "Enmu_Shield.h"
#include "Enmu_Right_Hand.h"
#include "Enmu_Left_Hand.h"
#include "Enmu_Chok.h"
#include "RuiBigBall.h"
#include "EnmuBoss.h"
#include "EnmuBoss_Hit.h"
using namespace EnmuBoss;

CEnmuBoss_Pattern1State::CEnmuBoss_Pattern1State(STATE_TYPE eType, CEnmuBoss::PARTS eParts)
{
	m_eParts = eParts;
	m_eStateType = eType;
}

CEnmuBossState * CEnmuBoss_Pattern1State::HandleInput(CEnmuBoss * pEnmuBoss)
{

	if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_PlayerInfo().iHp <= 0)
	{
		  return new CEnmuBoss_Hit(CEnmuBossState::STATE_TYPE::TYPE_START, CEnmuBoss::PARTS::PARTS_SHIELD);
	}

	return nullptr;
}

CEnmuBossState * CEnmuBoss_Pattern1State::Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_0))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_0);
				return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_0))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_0);
				return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_1))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_1);
				return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_1))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_1);
				return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_END:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_2))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_2);
				return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_DEFAULT, m_eParts);
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_2))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_2);
				return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_DEFAULT, m_eParts);
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_DEFAULT:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_3))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_3);
				return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_CHANGE, m_eParts);
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_3))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_3);
				return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_CHANGE, m_eParts);
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_CHANGE:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_4))
			{
				//pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
				//pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
				//pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
				//pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_IDLE, 0.1f);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_4);
				//return new CEnmuBoss_Pattern1State(STATE_TYPE::TYPE_START, CEnmuBoss::PARTS::PARTS_LEFT_HAND);

				return new CIdleState();
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_4))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_4);
				return new CIdleState();
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_ADD:
		break;
	default:
		break;
	}


	return nullptr;
}

CEnmuBossState * CEnmuBoss_Pattern1State::Late_Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{

	for (_uint i = 0; i < pEnmuBoss->Get_EnmuPartsList().size(); ++i)
	{
		pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Play_Animation(fTimeDelta);
	}
	if (m_eStateType == CEnmuBossState::TYPE_START)
	{
		if (m_iHit == 0)
		{
			++m_iHit;
			_vector vEffectPos;
			if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
				vEffectPos = XMLoadFloat4(&dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND])->Check_CollPos());
			else if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
				vEffectPos = XMLoadFloat4(&dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND])->Check_CollPos());

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			
			pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT1_FLASH,&vEffectPos);

			RELEASE_INSTANCE(CEffect_Manager);
		}
	}
	if (m_eStateType == CEnmuBossState::TYPE_DEFAULT)
	{
		if (m_iHit == 0)
		{
			CCharacters* m_pTarget = nullptr;
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_BattleTarget()->Get_PlayerInfo().iHp > 0)
				m_pTarget = pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_BattleTarget();
			else if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_BattleTarget()->Get_PlayerInfo().iHp <= 0)
				m_pTarget = pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_BattleTarget();

			CRuiBigBall::RUIBIGBALLINFO	tInfo;

			tInfo.pTarget = m_pTarget;
			tInfo.iType = 1;
			if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
				tInfo.vMyPos = dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND])->Check_CollPos();
			else if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
				tInfo.vMyPos = dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND])->Check_CollPos();


			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiBigBall"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);

		
			m_fDelay = 0.f;
			++m_iHit;
		}
	}
	return nullptr;
}

void CEnmuBoss_Pattern1State::Enter(CEnmuBoss * pEnmuBoss)
{
	
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_0, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_0, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_1, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_1, 0.1f);
		}

		break;
	case Client::CEnmuBossState::TYPE_END:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_2, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_2, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_DEFAULT:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_3, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_3, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_CHANGE:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN1_4, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN1_4, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_ADD:
		break;
	default:
		break;
	}



}

void CEnmuBoss_Pattern1State::Exit(CEnmuBoss * pEnmuBoss)
{

}

