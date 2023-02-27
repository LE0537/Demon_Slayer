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


	return nullptr;
}

CEnmuBossState * CIdleState::Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta)
{

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

void CIdleState::Exit(CEnmuBoss* pEnmuBoss)
{
}
