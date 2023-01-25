#include "stdafx.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"

using namespace Akaza;

CIdleState::CIdleState(STATE_ID eState)
{
	m_ePreState = eState;
}

CAkazaState * CIdleState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return nullptr;
}

CAkazaState * CIdleState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{



	return nullptr;
}

CAkazaState * CIdleState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{


	return nullptr;
}

void CIdleState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_IDLE;

}

void CIdleState::Exit(CAkaza* pAkaza)
{
}
