#include "stdafx.h"
#include "EnmuBossIdleState.h"
#include "GameInstance.h"

using namespace EnmuBoss;

CIdleState::CIdleState()
{

}

CEnmuBossState * CIdleState::HandleInput(CEnmuBoss* pEnmuBoss)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CEnmuBossState * CIdleState::Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta)
{

	return nullptr;
}

CEnmuBossState * CIdleState::Late_Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta)
{
	for (auto& iter : pEnmuBoss->Get_EnmuPartsList())
	{
		iter->Get_Model()->Play_Animation(fTimeDelta);
	}


	return nullptr;
}

void CIdleState::Enter(CEnmuBoss* pEnmuBoss)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	
}

void CIdleState::Exit(CEnmuBoss* pEnmuBoss)
{
}
