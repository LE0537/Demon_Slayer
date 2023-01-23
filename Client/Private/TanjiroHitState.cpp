#include "stdafx.h"
#include "TanjiroHitState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"

using namespace Tanjiro;

CHitState::CHitState(_float _fPow)
	:m_fPow(_fPow)
{
}

CTanjiroState * CHitState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CHitState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (!m_bReset)
	{
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_HIT);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_HIT);
		m_bReset = true;
	}
	fHitTime += fTimeDelta * 60.f;


	if (fHitTime <= 20.f)
		pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

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



