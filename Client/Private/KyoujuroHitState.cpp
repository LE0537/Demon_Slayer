#include "stdafx.h"
#include "KyoujuroHitState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"

using namespace Kyoujuro;

CHitState::CHitState()
{
}

CKyoujuroState * CHitState::HandleInput(CKyoujuro* pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Down(DIK_B))
		pKyoujuro->Take_Damage();

	return nullptr;
}

CKyoujuroState * CHitState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_HIT);

	fHitTime += fTimeDelta * 60.f;
	//printf("HitTime : %f \n", (float)fHitTime);

	if (fHitTime >= 38.f)
		return new CIdleState();

	return nullptr;
}

CKyoujuroState * CHitState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.1f);

	return nullptr;
}

void CHitState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT);
}

void CHitState::Exit(CKyoujuro* pKyoujuro)
{
	
}



