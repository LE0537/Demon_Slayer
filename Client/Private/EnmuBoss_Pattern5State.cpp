#include "stdafx.h"
#include "..\Public\EnmuBoss_Pattern5State.h"
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

CEnmuBoss_Pattern5State::CEnmuBoss_Pattern5State(STATE_TYPE eType, CEnmuBoss::PARTS eParts)
{
	m_eParts = eParts;
	m_eStateType = eType;
}

CEnmuBossState * CEnmuBoss_Pattern5State::HandleInput(CEnmuBoss * pEnmuBoss)
{
	if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_PlayerInfo().iHp <= 0)
	{
		return new CEnmuBoss_Hit(CEnmuBossState::STATE_TYPE::TYPE_START, CEnmuBoss::PARTS::PARTS_SHIELD);
	}
	return nullptr;
}

CEnmuBossState * CEnmuBoss_Pattern5State::Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_0))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_0);
				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_0))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_0);
				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_1))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_1);
				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_1))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_1);
				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_END, m_eParts);
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_END:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_2))
			{
				dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND])->Set_CollBox(false);

				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_2);
				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_DEFAULT, m_eParts);
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_2))
			{
				dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND])->Set_CollBox(false);

				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_2);
				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_DEFAULT, m_eParts);
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_DEFAULT:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_3))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_3);
				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_CHANGE, m_eParts);
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_3))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_3);
				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_CHANGE, m_eParts);
			}
		}
		break;
	case Client::CEnmuBossState::TYPE_CHANGE:
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_4))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_IDLE, 0.1f);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_4);

				_vector vRightHandLook = dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND])->GET_RightHandOriginLook();
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Transform()->Set_State(CTransform::STATE_LOOK, vRightHandLook);

				return new CEnmuBoss_Pattern5State(STATE_TYPE::TYPE_START, CEnmuBoss::PARTS::PARTS_LEFT_HAND);

				//return new CIdleState();
			}
		}
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_4))
			{
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_4);

				_vector vLeftHandLook = dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND])->GET_LeftHandOriginLook();
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Transform()->Set_State(CTransform::STATE_LOOK, vLeftHandLook);

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

CEnmuBossState * CEnmuBoss_Pattern5State::Late_Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{
	if (m_eStateType == CEnmuBossState::TYPE_START)
	{
		if (m_iHit == 0)
		{
			++m_iHit;
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		/*	if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
			{
				pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT5_HAND, dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]));
				pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT5_WIND, dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]));
			}
			else if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
			{
				pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT5_HAND, dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]));
				pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT5_WIND, dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]));
			}*/

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
		if (m_iHit == 0)
		{
			CCharacters* m_pTarget = pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_BattleTarget();

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
			{
				dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND])->Set_CollBox(true);
		//		pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT5_WIND, dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]));
		//		pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT5_GROUND2, dynamic_cast<CEnmu_Left_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]));
			}
			else if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
			{
				dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND])->Set_CollBox(true);
		//		pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT5_WIND, dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]));
		//		pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT5_GROUND2, dynamic_cast<CEnmu_Right_Hand*>(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]));
			}
			RELEASE_INSTANCE(CEffect_Manager);
			++m_iHit;
		}
	
	}
	return nullptr;
}

void CEnmuBoss_Pattern5State::Enter(CEnmuBoss * pEnmuBoss)
{
	m_eStateId = STATE_ATK_1;


	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_0, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_0, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_1, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_1);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_1, 0.1f);
		}

		break;
	case Client::CEnmuBossState::TYPE_END:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			_vector vTargetPos = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(LEVEL_BOSSENMU, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Transform()->Set_PlayerLookAt(vTargetPos);

			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_2, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			_vector vTargetPos = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(LEVEL_BOSSENMU, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Transform()->Set_PlayerLookAt(vTargetPos);

			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_2, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_DEFAULT:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_3, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_3);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_3, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_CHANGE:
		// RIGHT_HAND
		if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_4, 0.1f);
		}
		// LEFT_HAND
		else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Reset_Anim(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN5_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_4);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN5_4, 0.1f);
		}
		break;
	case Client::CEnmuBossState::TYPE_ADD:
		break;
	default:
		break;
	}



}

void CEnmuBoss_Pattern5State::Exit(CEnmuBoss * pEnmuBoss)
{

}

