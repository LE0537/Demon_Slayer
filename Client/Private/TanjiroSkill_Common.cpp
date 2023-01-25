#include "stdafx.h"
#include "TanjiroSkill_Common.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CSkill_CommonState::CSkill_CommonState()
{

}

CTanjiroState * CSkill_CommonState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CSkill_CommonState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CSkill_CommonState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_CommonState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_COMMON);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_COMMON);

}

void CSkill_CommonState::Exit(CTanjiro * pTanjiro)
{

}

