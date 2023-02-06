#include "stdafx.h"
#include "KyoujuroBattleSTState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "GameObj.h"

using namespace Kyoujuro;


CBattleStartState::CBattleStartState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);

}

CKyoujuroState * CBattleStartState::HandleInput(CKyoujuro* pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	return nullptr;
}

CKyoujuroState * CBattleStartState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	
	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_BATTLE_START))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_BATTLE_START);
		return new CIdleState();
	}

	return nullptr;
}

CKyoujuroState * CBattleStartState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

		pKyoujuro->Get_Model()->Play_Animation2(fTimeDelta);
	
	return nullptr;
}

void CBattleStartState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_BATTLE_START;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_BATTLE_START);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_BATTLE_START);

}

void CBattleStartState::Exit(CKyoujuro* pKyoujuro)
{

}

