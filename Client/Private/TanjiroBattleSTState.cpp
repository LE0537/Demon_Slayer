#include "stdafx.h"
#include "TanjiroBattleSTState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "GameObj.h"

using namespace Tanjiro;


CBattleStartState::CBattleStartState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);

}

CTanjiroState * CBattleStartState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	return nullptr;
}

CTanjiroState * CBattleStartState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	
	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_BATTLE_START))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_BATTLE_START);
		return new CIdleState();
	}

	return nullptr;
}

CTanjiroState * CBattleStartState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CBattleStartState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_BATTLE_START;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_BATTLE_START);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_BATTLE_START);
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
	pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);

}

void CBattleStartState::Exit(CTanjiro * pTanjiro)
{

}

