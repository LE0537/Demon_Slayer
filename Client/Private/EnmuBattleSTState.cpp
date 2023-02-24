#include "stdafx.h"
#include "EnmuBattleSTState.h"
#include "EnmuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "GameObj.h"

using namespace Enmu;


CBattleStartState::CBattleStartState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);

}

CEnmuState * CBattleStartState::HandleInput(CEnmu* pEnmu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	return nullptr;
}

CEnmuState * CBattleStartState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	
	if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_BATTLE_START))
	{
		pEnmu->Get_Model()->Set_End(CEnmu::ANIM_BATTLE_START);
		return new CIdleState();
	}

	return nullptr;
}

CEnmuState * CBattleStartState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{

		pEnmu->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CBattleStartState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_BATTLE_START);
	pEnmu->Set_AnimIndex(CEnmu::ANIM_BATTLE_START);
	pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
	pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
}

void CBattleStartState::Exit(CEnmu* pEnmu)
{

}

