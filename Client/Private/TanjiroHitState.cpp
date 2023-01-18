#include "stdafx.h"
#include "TanjiroHitState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"

using namespace Tanjiro;

CHitState::CHitState()
{
}

CTanjiroState * CHitState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Down(DIK_M))
		pTanjiro->Take_Damage();

	return nullptr;
}

CTanjiroState * CHitState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_HIT);

	fHitTime += fTimeDelta * 60.f;
	printf("HitTime : %f \n", (float)fHitTime);

	if (fHitTime >= 35.f)
		return new CIdleState();

	return nullptr;
}

CTanjiroState * CHitState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.1f);

	return nullptr;
}

void CHitState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT);
}

void CHitState::Exit(CTanjiro * pTanjiro)
{
	
}



