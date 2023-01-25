#include "stdafx.h"
#include "KyoujuroSkill_DashSlash.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CSkill_DashSlashState::CSkill_DashSlashState()
{

}

CKyoujuroState * CSkill_DashSlashState::HandleInput(CKyoujuro * pKyojuro)
{

	return nullptr;
}

CKyoujuroState * CSkill_DashSlashState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	if (pKyojuro->Get_Model()->Get_End(pKyojuro->Get_AnimIndex()))
	{
		pKyojuro->Get_Model()->Set_End(pKyojuro->Get_AnimIndex());
		return new CIdleState();
	}


	return nullptr;
}

CKyoujuroState * CSkill_DashSlashState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{


	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_DashSlashState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_DASHSLASH;

	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_DASHSLASH);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_DASHSLASH);

}

void CSkill_DashSlashState::Exit(CKyoujuro * pKyojuro)
{

}

