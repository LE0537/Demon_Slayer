#include "stdafx.h"
#include "..\Public\EnmuBoss_Hit.h"
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

using namespace EnmuBoss;


CEnmuBoss_Hit::CEnmuBoss_Hit(STATE_TYPE eType, CEnmuBoss::PARTS eParts)
{
	m_eParts = eParts;
	m_eStateType = eType;
}

CEnmuBossState * CEnmuBoss_Hit::HandleInput(CEnmuBoss * pEnmuBoss)
{
	

	return nullptr;
}

CEnmuBossState * CEnmuBoss_Hit::Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:


		if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Get_End(3))
			
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_End(3);
			return new CEnmuBoss_Hit(STATE_TYPE::TYPE_LOOP, m_eParts);
		}

		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		m_fDuration += fTimeDelta;

		if(m_fDuration >= 6.f)
			return new CEnmuBoss_Hit(STATE_TYPE::TYPE_END, m_eParts);
		break;
	case Client::CEnmuBossState::TYPE_END:
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Set_GodMode(true);

		m_fDuration += fTimeDelta;

		if(m_fDuration >= 3.f)
			return new CIdleState();

		if (pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Get_End(5))
		{
			pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_End(5);

			if(pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_PlayerInfo().iHp >= 0)
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

CEnmuBossState * CEnmuBoss_Hit::Late_Tick(CEnmuBoss * pEnmuBoss, _float fTimeDelta)
{
	for (_uint i = 0; i < pEnmuBoss->Get_EnmuPartsList().size(); ++i)
	{
		pEnmuBoss->Get_EnmuPartsList()[i]->Get_Model()->Play_Animation(fTimeDelta);
	}

	return nullptr;
}

void CEnmuBoss_Hit::Enter(CEnmuBoss * pEnmuBoss)
{

	pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Set_GodMode(true);


	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		// HEAD
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Set_AnimIndex(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_Loop(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE, true);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_LinearTime(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE, 0.1f);

		// SHIELD

		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Set_AnimIndex(3);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_CurrentAnimIndex(3);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_Loop(3);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_LinearTime(3, 0.1f);

		

		// RIGHT_HAND



		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(42);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(42);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(42);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(42, 0.1f);

		// LEFT_HAND


		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(41);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(41);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(41);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(41, 0.1f);

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

		break;
	case Client::CEnmuBossState::TYPE_LOOP:


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



		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(43);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(43);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(43,true);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(43, 0.1f);

		// LEFT_HAND


		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(42);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(42);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(42, true);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(42, 0.1f);

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

		break;
	case Client::CEnmuBossState::TYPE_END:


		// HEAD
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Set_AnimIndex(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_CurrentAnimIndex(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_Loop(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE, true);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_HEAD]->Get_Model()->Set_LinearTime(CEnmu_Chaos_Head::ANIMID::ANIM_IDLE, 0.1f);

		// SHIELD

		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Set_AnimIndex(5);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_CurrentAnimIndex(5);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_Loop(5);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_SHIELD]->Get_Model()->Set_LinearTime(5, 0.1f);

		// RIGHT_HAND



		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Set_AnimIndex(44);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_CurrentAnimIndex(44);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_Loop(44);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_RIGHT_HAND]->Get_Model()->Set_LinearTime(44, 0.1f);

		// LEFT_HAND


		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Set_AnimIndex(43);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_CurrentAnimIndex(43);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_Loop(43);
		pEnmuBoss->Get_EnmuPartsList()[CEnmuBoss::PARTS::PARTS_LEFT_HAND]->Get_Model()->Set_LinearTime(43, 0.1f);

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

void CEnmuBoss_Hit::Exit(CEnmuBoss * pEnmuBoss)
{
}
