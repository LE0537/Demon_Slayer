#include "stdafx.h"
#include "TanjiroSkill_WindMill.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
#include "TanjiroDashState.h"
using namespace Tanjiro;


CSkill_WindMillState::CSkill_WindMillState()
{

}

CTanjiroState * CSkill_WindMillState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CTanjiroState * CSkill_WindMillState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());


	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		return new CIdleState();
	}
	

	return nullptr;
}

CTanjiroState * CSkill_WindMillState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_WindMillState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_WINDMILL;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_WINDMILL);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_SKILL_WINDMILL, 0.2f);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_WINDMILL);

}

void CSkill_WindMillState::Exit(CTanjiro * pTanjiro)
{
}

