#include "stdafx.h"
#include "TanjiroGuardHitState.h"
#include "TanjiroIdleState.h"
#include "TanjiroGuardState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
using namespace Tanjiro;


CGuardHitState::CGuardHitState(STATE_TYPE eState)
{
	m_eStateType = eState;
}

CTanjiroState * CGuardHitState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CTanjiroState * CGuardHitState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_GUARD_HIT_0);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_GUARD_HIT_1);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_GUARD_HIT_0, 0.2f);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_GUARD_HIT_1, 0.2f);

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		default:
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}

	return nullptr;






	return nullptr;
}

CTanjiroState * CGuardHitState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardHitState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_GUARD_HIT;


	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_GUARD_HIT_0);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_GUARD_HIT_0);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_GUARD_HIT_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_GUARD_HIT_1);
		break;
	}



}

void CGuardHitState::Exit(CTanjiro * pTanjiro)
{
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

