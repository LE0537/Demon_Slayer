#include "stdafx.h"
#include "NezukoGuardHitState.h"
#include "NezukoIdleState.h"
#include "AkazaGuardState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "NezukoGuardState.h"
using namespace Nezuko;


CGuardHitState::CGuardHitState(STATE_TYPE eState)
{
	m_eStateType = eState;
}

CNezukoState * CGuardHitState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CNezukoState * CGuardHitState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_bGuard(true);
	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_GUARD_HIT_0);
	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_GUARD_HIT_1);
	//pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_GUARD_HIT_0, 0.2f);
	//pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_GUARD_HIT_1, 0.2f);

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CGuardState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CGuardState(TYPE_LOOP);
			break;
		default:
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	return nullptr;






	return nullptr;
}

CNezukoState * CGuardHitState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.3f, pNezuko->Get_NavigationCom());
		break;
	case Client::CNezukoState::TYPE_LOOP:
		if (m_fTime < 0.3f)
		{
			pNezuko->Get_Transform()->Go_Backward(fTimeDelta * 0.6f, pNezuko->Get_NavigationCom());
		}
		break;
	default:
		break;
	}
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardHitState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_GUARD_HIT;


	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_GUARD_HIT_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_GUARD_HIT_0);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_GUARD_HIT_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_GUARD_HIT_0, 0.01f);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_GUARD_HIT_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_GUARD_HIT_1);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_GUARD_HIT_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_GUARD_HIT_1, 0.01f);
		break;
	}



}

void CGuardHitState::Exit(CNezuko* pNezuko)
{

	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

