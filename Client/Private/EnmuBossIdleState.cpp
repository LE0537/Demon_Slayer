#include "stdafx.h"
#include "EnmuBossIdleState.h"
#include "GameInstance.h"

#include "Enmu_Chaos_Head.h"
#include "Enmu_Shield.h"
#include "Enmu_Right_Hand.h"
#include "Enmu_Left_Hand.h"
#include "Enmu_Chok.h"

#include "EnmuBoss_Pattern1State.h"
#include "EnmuBoss_Pattern2State.h"
#include "EnmuBoss_Pattern3State.h"
#include "EnmuBoss_Pattern4State.h"
#include "EnmuBoss_Pattern5State.h"
#include "EnmuBoss_Pattern6State.h"
#include "Tanjiro.h"
#include "Layer.h"

#include "ImGuiManager.h"

using namespace EnmuBoss;

CIdleState::CIdleState()
{

}

CEnmuBossState * CIdleState::HandleInput(CEnmuBoss* pEnmuBoss)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	
	if (pGameInstance->Get_Instance()->Key_Down(DIK_1)) // 파동 날리기 왼손 오른손 랜덤 필요
		return new CEnmuBoss_Pattern1State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
	if (pGameInstance->Get_Instance()->Key_Down(DIK_2)) // 촉수 두번 속도 빠르게 해야될듯
		return new CEnmuBoss_Pattern2State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
	if (pGameInstance->Get_Instance()->Key_Down(DIK_3)) // 범위 넓은 공격 
		return new CEnmuBoss_Pattern3State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
	if (pGameInstance->Get_Instance()->Key_Down(DIK_4)) // 주먹 
		return new CEnmuBoss_Pattern4State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
	if (pGameInstance->Get_Instance()->Key_Down(DIK_5)) // 주먹 내려찍기
		return new CEnmuBoss_Pattern5State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
	if (pGameInstance->Get_Instance()->Key_Down(DIK_6)) // 촉수 7초
		return new CEnmuBoss_Pattern6State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);


	if (pEnmuBoss->Get_Start() == false)
	{
		if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Get_End(46) &&
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Get_End(45))
		{

			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Set_AnimIndex(5);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_CurrentAnimIndex(5);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_Loop(5, false);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_LinearTime(5, 0.1f);

			if(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Get_End(5))
			{ 
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_End(46);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_End(45);
				pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_End(5);

				pEnmuBoss->Set_Start();

				return new CIdleState();
			}

		}

		return nullptr;
	}
		



	CCharacters* pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(LEVEL_BOSSENMU, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	_vector vTargetPosition = pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	CImGuiManager::Get_Instance()->Set_Distance(fDistance);

	if (fDistance <= 25.f) { m_eRange = RANGE_IN; }
	else { m_eRange = RANGE_OUT; }


	switch (m_eRange)
	{
	case Client::EnmuBoss::CIdleState::RANGE_IN:
		Update_AI_Near(pEnmuBoss);
		break;
	case Client::EnmuBoss::CIdleState::RANGE_OUT:
		Update_AI_Out(pEnmuBoss);
		break;
	case Client::EnmuBoss::CIdleState::RANGE_END:
		break;
	default:
		break;
	}




	if (m_fDelay >= 1.f)
	{
		m_fDelay = 0.f;
		return Return_AIState(pEnmuBoss);
	}
	else
	{
		return nullptr;
	}


}

CEnmuBossState * CIdleState::Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta)
{
	m_fDelay += fTimeDelta;

	return nullptr;
}

CEnmuBossState * CIdleState::Late_Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta)
{
	for (_uint i = 0; i < pEnmuBoss->Get_EnmuPartsList().size(); ++i)
	{
		pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Play_Animation(fTimeDelta);
	}


	return nullptr;
}

void CIdleState::Enter(CEnmuBoss* pEnmuBoss)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	if (pEnmuBoss->Get_Start() == false)
	{
		// HEAD
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Set_AnimIndex(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_Loop(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE, true);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_LinearTime(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE, 0.1f);

		// SHIELD

		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Set_AnimIndex(4);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_CurrentAnimIndex(4);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_Loop(4, true);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_LinearTime(4, 0.1f);

		// RIGHT_HAND



		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(46);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(46);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(46);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(46, 0.1f);

		// LEFT_HAND


		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(45);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(45);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(45);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(45, 0.1f);

		// CHOK

		for (_uint i = CEnmuBoss::PARTS::PARTS_CHOCK1; i < CEnmuBoss::PARTS::PARTS_END; ++i)
		{
			std::random_device RandomDevice;
			std::mt19937 gen(RandomDevice());
			std::uniform_int_distribution<int> RandomPattern(1, 3);
			int iRandom = RandomPattern(gen);

			switch (iRandom)
			{
			case 1:
				pEnmuBoss->Get_EnmuPartsList()[i]->Set_AnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_0);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_0);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_Loop(CEnmu_Chok::ANIMID::ANIM_IDLE_0, true);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_LinearTime(CEnmu_Chok::ANIMID::ANIM_IDLE_0, 0.1f);
				break;
			case 2:
				pEnmuBoss->Get_EnmuPartsList()[i]->Set_AnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_1);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_1);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_Loop(CEnmu_Chok::ANIMID::ANIM_IDLE_1, true);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_LinearTime(CEnmu_Chok::ANIMID::ANIM_IDLE_1, 0.1f);
				break;
			case 3:
				pEnmuBoss->Get_EnmuPartsList()[i]->Set_AnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_2);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_2);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_Loop(CEnmu_Chok::ANIMID::ANIM_IDLE_2, true);
				pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_LinearTime(CEnmu_Chok::ANIMID::ANIM_IDLE_2, 0.1f);
				break;
			}
		}
		//pEnmuBoss->Set_Start();
	}

	else
	{
		if (!pEnmuBoss->Get_EnmuPartsList().empty())
		{
			// HEAD
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Set_AnimIndex(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_Loop(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE, true);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_LinearTime(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE, 0.1f);

			// SHIELD

			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Set_AnimIndex(CEnmu_Shield::ANIMID::ANIM_IDLE);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Shield::ANIMID::ANIM_IDLE);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_Loop(CEnmu_Shield::ANIMID::ANIM_IDLE, true);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_LinearTime(CEnmu_Shield::ANIMID::ANIM_IDLE, 0.1f);

			// RIGHT_HAND



			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Right_Hand::ANIMID::ANIM_IDLE);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(CEnmu_Right_Hand::ANIMID::ANIM_IDLE, true);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Right_Hand::ANIMID::ANIM_IDLE, 0.1f);

			// LEFT_HAND


			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_IDLE);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Left_Hand::ANIMID::ANIM_IDLE);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(CEnmu_Left_Hand::ANIMID::ANIM_IDLE, true);
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(CEnmu_Left_Hand::ANIMID::ANIM_IDLE, 0.1f);

			// CHOK

			for (_uint i = CEnmuBoss::PARTS::PARTS_CHOCK1; i < CEnmuBoss::PARTS::PARTS_END; ++i)
			{
				std::random_device RandomDevice;
				std::mt19937 gen(RandomDevice());
				std::uniform_int_distribution<int> RandomPattern(1, 3);
				int iRandom = RandomPattern(gen);

				switch (iRandom)
				{
				case 1:
					pEnmuBoss->Get_EnmuPartsList()[i]->Set_AnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_0);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_0);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_Loop(CEnmu_Chok::ANIMID::ANIM_IDLE_0, true);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_LinearTime(CEnmu_Chok::ANIMID::ANIM_IDLE_0, 0.1f);
					break;
				case 2:
					pEnmuBoss->Get_EnmuPartsList()[i]->Set_AnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_1);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_1);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_Loop(CEnmu_Chok::ANIMID::ANIM_IDLE_1, true);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_LinearTime(CEnmu_Chok::ANIMID::ANIM_IDLE_1, 0.1f);
					break;
				case 3:
					pEnmuBoss->Get_EnmuPartsList()[i]->Set_AnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_2);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chok::ANIMID::ANIM_IDLE_2);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_Loop(CEnmu_Chok::ANIMID::ANIM_IDLE_2, true);
					pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Set_LinearTime(CEnmu_Chok::ANIMID::ANIM_IDLE_2, 0.1f);
					break;
				}
			}
		}
	}


}

void CIdleState::Exit(CEnmuBoss* pEnmuBoss)
{
}

void CIdleState::Update_AI_Near(CEnmuBoss * pEnmuBoss)
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_ePattern = AI_PATTERN::PATTERN_1;
		break;
	case 2:
		m_ePattern = AI_PATTERN::PATTERN_1;
		break;
	case 3:
		m_ePattern = AI_PATTERN::PATTERN_1;
		break;
	case 4:
		m_ePattern = AI_PATTERN::PATTERN_1;
		break;
	case 5:
		m_ePattern = AI_PATTERN::PATTERN_2;
		break;
	case 6:
		m_ePattern = AI_PATTERN::PATTERN_2;
		break;
	case 7:
		m_ePattern = AI_PATTERN::PATTERN_2;
		break;
	case 8:
		m_ePattern = AI_PATTERN::PATTERN_2;
		break;
	case 9:
		m_ePattern = AI_PATTERN::PATTERN_3;
		break;
	case 10:
		m_ePattern = AI_PATTERN::PATTERN_3;
		break;
	}
}

void CIdleState::Update_AI_Out(CEnmuBoss * pEnmuBoss)
{
	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 10);
	int iRandom = RandomPattern(gen);

	switch (iRandom)
	{
	case 1:
		m_ePattern = AI_PATTERN::PATTERN_3;
		break;
	case 2:
		m_ePattern = AI_PATTERN::PATTERN_3;
		break;
	case 3:
		m_ePattern = AI_PATTERN::PATTERN_4;
		break;
	case 4:
		m_ePattern = AI_PATTERN::PATTERN_4;
		break;
	case 5:
		m_ePattern = AI_PATTERN::PATTERN_5;
		break;
	case 6:
		m_ePattern = AI_PATTERN::PATTERN_5;
		break;
	case 7:
		m_ePattern = AI_PATTERN::PATTERN_1;
		break;
	case 8:
		m_ePattern = AI_PATTERN::PATTERN_1;
		break;
	case 9:
		m_ePattern = AI_PATTERN::PATTERN_2;
		break;
	case 10:
		m_ePattern = AI_PATTERN::PATTERN_6;
		break;
	}



}

CEnmuBossState * CIdleState::Return_AIState(CEnmuBoss * pEnmuBoss)
{

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 2);
	int iRandom = RandomPattern(gen);

	switch (m_ePattern)
	{
	case Client::EnmuBoss::CIdleState::PATTERN_1:
		if (iRandom == 1)
			return new CEnmuBoss_Pattern1State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
		else
			return new CEnmuBoss_Pattern1State(TYPE_START, CEnmuBoss::PARTS::PARTS_LEFT_HAND);
		break;
	case Client::EnmuBoss::CIdleState::PATTERN_2:
		return new CEnmuBoss_Pattern2State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
		break;
	case Client::EnmuBoss::CIdleState::PATTERN_3:
		return new CEnmuBoss_Pattern3State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
		break;
	case Client::EnmuBoss::CIdleState::PATTERN_4:
		return new CEnmuBoss_Pattern4State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
		break;
	case Client::EnmuBoss::CIdleState::PATTERN_5:
		return new CEnmuBoss_Pattern5State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
		break;
	case Client::EnmuBoss::CIdleState::PATTERN_6:
		return new CEnmuBoss_Pattern6State(TYPE_START, CEnmuBoss::PARTS::PARTS_RIGHT_HAND);
		break;
	case Client::EnmuBoss::CIdleState::PATTERN_END:
		break;
	default:
		break;
	}




	return nullptr;
}
