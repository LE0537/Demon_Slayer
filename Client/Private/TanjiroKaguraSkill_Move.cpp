#include "stdafx.h"
#include "TanjiroKaguraSkill_Move.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CKaguraSkill_MoveState::CKaguraSkill_MoveState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_WindMill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CTanjiroState * CKaguraSkill_MoveState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CKaguraSkill_MoveState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CKaguraSkill_MoveState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CKaguraSkill_MoveState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_KAGURA_MOVE;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_MOVE);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_KAGURA_MOVE);

}

void CKaguraSkill_MoveState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead();
	pTanjiro->Reset_SkillHit();
}

