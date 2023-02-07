#include "stdafx.h"
#include "AkazaBattleSTState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "GameObj.h"

using namespace Akaza;


CBattleStartState::CBattleStartState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);

}

CAkazaState * CBattleStartState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	return nullptr;
}

CAkazaState * CBattleStartState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	
		if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_BATTLE_START))
		{
			pAkaza->Get_Model()->Set_End(CAkaza::ANIM_BATTLE_START);
			return new CIdleState();
		}
	
	return nullptr;
}

CAkazaState * CBattleStartState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

		pAkaza->Get_Model()->Play_Animation2(fTimeDelta);
	
	return nullptr;
}

void CBattleStartState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_BATTLE_START;

	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_BATTLE_START);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_BATTLE_START);
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
}

void CBattleStartState::Exit(CAkaza* pAkaza)
{

}

