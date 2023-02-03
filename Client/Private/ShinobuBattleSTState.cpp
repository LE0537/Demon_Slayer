#include "stdafx.h"
#include "ShinobuBattleSTState.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "GameObj.h"

using namespace Shinobu;


CBattleStartState::CBattleStartState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);

}

CShinobuState * CBattleStartState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	return nullptr;
}

CShinobuState * CBattleStartState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	
	if (pShinobu->Get_Model()->Get_End(CShinobu::ANIM_BATTLE_START))
	{
		pShinobu->Get_Model()->Set_End(CShinobu::ANIM_BATTLE_START);
		return new CIdleState();
	}

	return nullptr;
}

CShinobuState * CBattleStartState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CBattleStartState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_BATTLE_START;

	pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_BATTLE_START);
	pShinobu->Set_AnimIndex(CShinobu::ANIM_BATTLE_START);

}

void CBattleStartState::Exit(CShinobu* pShinobu)
{

}

