#include "stdafx.h"
#include "..\Public\EnmuBoss_Pattern3State.h"
#include "EnmuBossIdleState.h"
#include "GameInstance.h"

#include "Enmu_Chaos_Head.h"
#include "Enmu_Shield.h"
#include "Enmu_Right_Hand.h"
#include "Enmu_Left_Hand.h"
#include "Enmu_Chok.h"

#include "Tanjiro.h"
#include "Layer.h"
#include "EnmuBoss_Hit.h"
#include "Effect_Manager.h"
using namespace EnmuBoss;

CEnmuBoss_Pattern3State::CEnmuBoss_Pattern3State(STATE_TYPE eType, CEnmuBoss::PARTS eParts)
{
	m_eParts = eParts;
	m_eStateType = eType;
}

CEnmuBossState * CEnmuBoss_Pattern3State::HandleInput(CEnmuBoss * pEnmuBoss)
{

	if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_PlayerInfo().iHp <= 0)
	{
		return new CEnmuBoss_Hit(CEnmuBossState::STATE_TYPE::TYPE_START, CEnmuBoss::PARTS::PARTS_SHIELD);
	}


	return nullptr;
}

CEnmuBossState * CEnmuBoss_Pattern3State::Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		//if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_0) &&
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_0))
			{

				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_0);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_0);
				return new CEnmuBoss_Pattern3State(STATE_TYPE::TYPE_LOOP, m_eParts);
			}
		}
		//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		//{
		//	if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_0))
		//	{
		//		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_0);
		//		return new CEnmuBoss_Pattern3State(STATE_TYPE::TYPE_LOOP, m_eParts);
		//	}
		//}
		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_1) &&
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_1))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_1);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_1);
				return new CEnmuBoss_Pattern3State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		//{
		//	if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_1))
		//	{
		//		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_1);
		//		return new CEnmuBoss_Pattern3State(STATE_TYPE::TYPE_END, m_eParts);
		//	}
		//}
		break;
	case Client::CEnmuBossState::TYPE_END:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_2) &&
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_2))
			{
			
				dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND])->Set_CollBox(false);
				dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND])->Set_CollBox(false);

				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_2);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_2);
				//pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
				//pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
				//pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_IDLE, true);
				//pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_IDLE, 0.1f);

				return new CIdleState();
			}
		}
		//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		//{
		//	if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_2))
		//	{
		//		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_2);
		//		return new CIdleState();
		//	}
		//}
		break;
	case Client::CEnmuBossState::TYPE_DEFAULT:
		break;
	case Client::CEnmuBossState::TYPE_CHANGE:
		break;
	case Client::CEnmuBossState::TYPE_ADD:
		break;
	default:
		break;
	}


	return nullptr;
}

CEnmuBossState * CEnmuBoss_Pattern3State::Late_Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{
	if (m_eStateType == CEnmuBossState::TYPE_START)
	{
		if (m_iHit == 0)
		{
			++m_iHit;
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
			pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT3_FLASH, dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND])->Get_WeaponWorld());
			else if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
			pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT3_FLASH, dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND])->Get_WeaponWorld());
			

			pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT3_GROUND, dynamic_cast<CEnmu_Chaos_Head*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]));

			RELEASE_INSTANCE(CEffect_Manager);
		}
	}
	for (_uint i = 0; i < pEnmuBoss->Get_EnmuPartsList().size(); ++i)
	{
		pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Play_Animation(fTimeDelta);
	}

	if (m_eStateType == CEnmuBossState::TYPE_END)
	{
		m_fDelay += fTimeDelta;
		if (m_fDelay > 0.3f && m_iHit == 0)
		{
			CCharacters* m_pTarget = pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_BattleTarget();

			dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND])->Set_CollBox(true);
			dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND])->Set_CollBox(true);

			++m_iHit;
		}
	
	}

	return nullptr;
}

void CEnmuBoss_Pattern3State::Enter(CEnmuBoss * pEnmuBoss)
{
	
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		// RIGHT_HAND
		//if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_0, 0.1f);
		}
		// LEFT_HAND
		//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_0, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		// RIGHT_HAND

		//if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_1, 0.1f);
		}
		// LEFT_HAND
		//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_1, 0.1f);
		}

		break;
	case Client::CEnmuBossState::TYPE_END:
		// RIGHT_HAND
		//if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_2, 0.1f);
		}
		// LEFT_HAND
		//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN3_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN3_2, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_DEFAULT:
		break;
	case Client::CEnmuBossState::TYPE_CHANGE:
		break;
	case Client::CEnmuBossState::TYPE_ADD:
		break;
	default:
		break;
	}



}

void CEnmuBoss_Pattern3State::Exit(CEnmuBoss * pEnmuBoss)
{

}

void CEnmuBoss_Pattern3State::Create_Chok()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_vector vTargetPos = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(LEVEL_BOSSENMU, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pGameInstance->Add_GameObject(L"Prototype_GameObject_Enmu_ChokChok", g_iLevel, L"Layer_ChokChok", &vTargetPos);
}

