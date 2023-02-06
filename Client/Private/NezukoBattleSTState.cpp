#include "stdafx.h"
#include "NezukoBattleSTState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "GameObj.h"

using namespace Nezuko;


CBattleStartState::CBattleStartState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);

}

CNezukoState * CBattleStartState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	return nullptr;
}

CNezukoState * CBattleStartState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	
	if (pNezuko->Get_Model()->Get_End(CNezuko::ANIM_BATTLE_START))
	{
		pNezuko->Get_Model()->Set_End(CNezuko::ANIM_BATTLE_START);
		return new CIdleState();
	}

	return nullptr;
}

CNezukoState * CBattleStartState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{

		pNezuko->Get_Model()->Play_Animation2(fTimeDelta);
	
	return nullptr;
}

void CBattleStartState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_BATTLE_START;

	pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_BATTLE_START);
	pNezuko->Set_AnimIndex(CNezuko::ANIM_BATTLE_START);

}

void CBattleStartState::Exit(CNezuko* pNezuko)
{

}

