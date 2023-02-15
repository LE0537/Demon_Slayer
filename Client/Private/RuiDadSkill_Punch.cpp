#include "stdafx.h"
#include "RuiDadSkill_Punch.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace RuiDad;


CSkill_PunchState::CSkill_PunchState(STATE_TYPE eType)
{
	m_eStateType = eType;

}

CRuiDadState * CSkill_PunchState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	


	return nullptr;
}

CRuiDadState * CSkill_PunchState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	pRuiDad->Get_Model()->Set_Loop(pRuiDad->Get_AnimIndex());

	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
		return new CIdleState();
	}

	return nullptr;
}

CRuiDadState * CSkill_PunchState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_PunchState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL1);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL1, 0.01f);
	pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL1);
	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL1);

}

void CSkill_PunchState::Exit(CRuiDad* pRuiDad)
{

}

