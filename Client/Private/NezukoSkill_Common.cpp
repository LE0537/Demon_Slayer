#include "stdafx.h"
#include "NezukoSkill_Common.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Nezuko;


CSkill_CommonState::CSkill_CommonState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CNezukoState * CSkill_CommonState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CNezukoState * CSkill_CommonState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSkill_CommonState(CNezukoState::TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSkill_CommonState(CNezukoState::TYPE_END);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	return nullptr;
}

CNezukoState * CSkill_CommonState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();

	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_CommonState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_COMMON_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_COMMON_0, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_COMMON_0);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_COMMON_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_COMMON_1, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_COMMON_1);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_COMMON_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_COMMON_2, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_COMMON_2);
		break;
	}

}

void CSkill_CommonState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

