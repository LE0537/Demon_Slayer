#include "stdafx.h"
#include "RuiBattleSTState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "GameObj.h"

using namespace Rui;


CBattleStartState::CBattleStartState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);

}

CRuiState * CBattleStartState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	return nullptr;
}

CRuiState * CBattleStartState::Tick(CRui* pRui, _float fTimeDelta)
{
	
	if (pRui->Get_Model()->Get_End(CRui::ANIM_BATTLE_START))
	{
		pRui->Get_Model()->Set_End(CRui::ANIM_BATTLE_START);
		return new CIdleState();
	}

	return nullptr;
}

CRuiState * CBattleStartState::Late_Tick(CRui* pRui, _float fTimeDelta)
{

		pRui->Get_Model()->Play_Animation2(fTimeDelta);
	
	return nullptr;
}

void CBattleStartState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_BATTLE_START;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_BATTLE_START);
	pRui->Set_AnimIndex(CRui::ANIM_BATTLE_START);

}

void CBattleStartState::Exit(CRui* pRui)
{

}

