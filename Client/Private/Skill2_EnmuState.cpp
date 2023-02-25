#include "stdafx.h"
#include "..\Public\Skill2_EnmuState.h"
#include "GameInstance.h"
#include "EnmuIdleState.h"

using namespace Enmu;

CSkill2_EnmuState::CSkill2_EnmuState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CSkill2_EnmuState::HandleInput(CEnmu * pEnmu)
{
	return nullptr;
}

CEnmuState * CSkill2_EnmuState::Tick(CEnmu * pEnmu, _float fTimeDelta)
{
	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			return new CSkill2_EnmuState(TYPE_LOOP);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			return new CSkill2_EnmuState(TYPE_END);
			break;
		case Client::CEnmuState::TYPE_END:
			return new CSkill2_EnmuState(TYPE_DEFAULT);
			break;
		case Client::CEnmuState::TYPE_DEFAULT:
			return new CIdleState();
			break;
		case Client::CEnmuState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}


	return nullptr;
}

CEnmuState * CSkill2_EnmuState::Late_Tick(CEnmu * pEnmu, _float fTimeDelta)
{
	pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill2_EnmuState::Enter(CEnmu * pEnmu)
{
	m_eStateId = STATE_SKILL2;

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_2_0);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_2_0);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_2_0, 0.01f);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_2_0);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_2_1);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_2_1);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_2_1, 0.01f);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_2_1);
		break;
	case Client::CEnmuState::TYPE_END:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_2_2);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_2_2);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_2_2, 0.01f);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_2_2);
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_2_3);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_2_3);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_2_3, 0.01f);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_2_3);
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		break;
	default:
		break;
	}



}

void CSkill2_EnmuState::Exit(CEnmu * pEnmu)
{
}
