#include "stdafx.h"
#include "KyoujuroSkill_DoubleUpper.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CSkill_DoubleUpperState::CSkill_DoubleUpperState()
{

}

CKyoujuroState * CSkill_DoubleUpperState::HandleInput(CKyoujuro * pKyojuro)
{

	return nullptr;
}

CKyoujuroState * CSkill_DoubleUpperState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	if (pKyojuro->Get_Model()->Get_End(pKyojuro->Get_AnimIndex()))
	{
		pKyojuro->Get_Model()->Set_End(pKyojuro->Get_AnimIndex());
		return new CIdleState();
	}


	return nullptr;
}

CKyoujuroState * CSkill_DoubleUpperState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{


	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_DoubleUpperState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_DOUBLEUPPER;

	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_DOUBLEUPPER);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_DOUBLEUPPER);

}

void CSkill_DoubleUpperState::Exit(CKyoujuro * pKyojuro)
{

}

