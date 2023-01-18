#include "stdafx.h"
#include "TanjiroGuardAdvState.h"
#include "TanjiroIdleState.h"
#include "TanjiroGuardState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
using namespace Tanjiro;


CGuardAdvState::CGuardAdvState()
{
}

CTanjiroState * CGuardAdvState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_GUARD_ADV))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_GUARD_ADV);

		if (pGameInstance->Key_Pressing(DIK_O))
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
	}


	return nullptr;
}

CTanjiroState * CGuardAdvState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_GUARD_ADV);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_GUARD_ADV, 0.02f);


	printf_s("type : %d state : %d anim : %d \n", (int)m_eStateType, (int)m_eStateId, (int)pTanjiro->Get_AnimIndex());

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();





	return nullptr;
}

CTanjiroState * CGuardAdvState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardAdvState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_GUARD_ADV;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_GUARD_ADV);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_GUARD_ADV);

}

void CGuardAdvState::Exit(CTanjiro * pTanjiro)
{
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

