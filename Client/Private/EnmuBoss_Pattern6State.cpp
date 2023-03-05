#include "stdafx.h"
#include "..\Public\EnmuBoss_Pattern6State.h"
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
using namespace EnmuBoss;

CEnmuBoss_Pattern6State::CEnmuBoss_Pattern6State(STATE_TYPE eType, CEnmuBoss::PARTS eParts)
{
	m_eParts = eParts;
	m_eStateType = eType;
}

CEnmuBossState * CEnmuBoss_Pattern6State::HandleInput(CEnmuBoss * pEnmuBoss)
{

	if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_PlayerInfo().iHp <= 0)
	{
		return new CEnmuBoss_Hit(CEnmuBossState::STATE_TYPE::TYPE_START, CEnmuBoss::PARTS::PARTS_SHIELD);
	}
	return nullptr;
}

CEnmuBossState * CEnmuBoss_Pattern6State::Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:

		if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_0) 
			&& pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_0))
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_0);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_0);
			return new CEnmuBoss_Pattern6State(STATE_TYPE::TYPE_LOOP, m_eParts);
		}

		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		m_fDuration += fTimeDelta;
		m_fCurrentDuration += fTimeDelta;
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.3f)
		{
			Create_TargetChok();

			m_fTime = 0.f;
		}


		if (m_fCurrentDuration >= 0.5f)
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			_vector vTargetPos = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(LEVEL_BOSSENMU, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			std::random_device RandomDevice;
			std::mt19937 gen(RandomDevice());
			std::uniform_int_distribution<int> RandomPattern(3, 10);
			int iRandom = RandomPattern(gen);

			vTargetPos += XMVectorSet(iRandom, 0.f, iRandom, 0.f);

			Create_Chok(vTargetPos);

			m_fCurrentDuration = 0.f;
		}


		if(m_fDuration >= 7.f)
			return new CEnmuBoss_Pattern6State(STATE_TYPE::TYPE_END, m_eParts);

		//if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_1)
		//	&& pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_1))
		//{
		//	pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_1);
		//	pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_1);
		//	return new CEnmuBoss_Pattern6State(STATE_TYPE::TYPE_END, m_eParts);
		//}


		break;
	case Client::CEnmuBossState::TYPE_END:

		if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_2)
			&& pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_2))
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_2);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_2);
			return new CIdleState();
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


	return nullptr;
}

CEnmuBossState * CEnmuBoss_Pattern6State::Late_Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{

	for (_uint i = 0; i < pEnmuBoss->Get_EnmuPartsList().size(); ++i)
	{
		pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Play_Animation(fTimeDelta);
	}

	return nullptr;
}

void CEnmuBoss_Pattern6State::Enter(CEnmuBoss * pEnmuBoss)
{

	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		// RIGHT_HAND
		//if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
	{
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_0);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_0);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_0);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_0, 0.1f);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("BossEnmu_Pattern_5.wav"), g_fVoice);
	}
	// LEFT_HAND
	//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
	{
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_0);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_0);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_0);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_0, 0.1f);
	}
	break;
	case Client::CEnmuBossState::TYPE_LOOP:
		// RIGHT_HAND
		//Create_Chok();

		//if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
	{
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_1);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_1);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_1, true);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_1, 0.1f);
	}
	// LEFT_HAND
	//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
	{
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_1);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_1);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_1, true);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_1, 0.1f);
	}
	CSoundMgr::Get_Instance()->PlayEffect(TEXT("BossEnmu_SE_Pattern_01.wav"), g_fEffect);
	break;
	case Client::CEnmuBossState::TYPE_END:
		// RIGHT_HAND
		//if (m_eParts == CEnmuBoss::PARTS::PARTS_RIGHT_HAND)
	{
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_2);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_2);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_2);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_PATTERN2_2, 0.1f);
	}
	// LEFT_HAND
	//else if (m_eParts == CEnmuBoss::PARTS::PARTS_LEFT_HAND)
	{
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_2);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_2);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_2);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_PATTERN2_2, 0.1f);
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

void CEnmuBoss_Pattern6State::Exit(CEnmuBoss * pEnmuBoss)
{

}

void CEnmuBoss_Pattern6State::Create_Chok(_vector vPosition)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	pGameInstance->Add_GameObject(L"Prototype_GameObject_Enmu_ChokChok", g_iLevel, L"Layer_ChokChok", &vPosition);
}

void CEnmuBoss_Pattern6State::Create_TargetChok()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_vector vTargetPos = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(LEVEL_BOSSENMU, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pGameInstance->Add_GameObject(L"Prototype_GameObject_Enmu_ChokChok", g_iLevel, L"Layer_ChokChok", &vTargetPos);
}

