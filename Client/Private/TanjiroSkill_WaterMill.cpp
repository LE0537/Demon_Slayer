#include "stdafx.h"
#include "TanjiroSkill_WaterMill.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CSkill_WaterMillState::CSkill_WaterMillState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * CSkill_WaterMillState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CTanjiroState * CSkill_WaterMillState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CSkill_WaterMillState(CTanjiroState::TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CSkill_WaterMillState(CTanjiroState::TYPE_END);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}


	return nullptr;
}

CTanjiroState * CSkill_WaterMillState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_WaterMillState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_WATERMILL;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_0);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_WATERMILL_0, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_0);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_1);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_WATERMILL_1, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_1);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_2);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_WATERMILL_2, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_2);
		break;
	}

}

void CSkill_WaterMillState::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
}

