#include "stdafx.h"
#include "AkazaSkill_Punch.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Akaza;


CSkill_PunchState::CSkill_PunchState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	


	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CAkazaState * CSkill_PunchState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CAkazaState * CSkill_PunchState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CSkill_PunchState(CAkazaState::TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CSkill_PunchState(CAkazaState::TYPE_END);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	return nullptr;
}

CAkazaState * CSkill_PunchState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_PunchState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_SKILL_PUNCH;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_PUNCH_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_PUNCH_0, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_PUNCH_0);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_PUNCH_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_PUNCH_1, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_PUNCH_1);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_PUNCH_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_PUNCH_2, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_PUNCH_2);
		break;
	}

}

void CSkill_PunchState::Exit(CAkaza* pAkaza)
{

}

