#include "stdafx.h"
#include "RuiIdleState.h"
#include "GameInstance.h"

using namespace Rui;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CRuiState * CIdleState::HandleInput(CRui * pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	


	return nullptr;
}

CRuiState * CIdleState::Tick(CRui * pRui, _float fTimeDelta)
{



	return nullptr;
}

CRuiState * CIdleState::Late_Tick(CRui * pRui, _float fTimeDelta)
{



	return nullptr;
}

void CIdleState::Enter(CRui * pRui)
{
	m_eStateId = STATE_ID::STATE_IDLE;


}

void CIdleState::Exit(CRui * pRui)
{
}
