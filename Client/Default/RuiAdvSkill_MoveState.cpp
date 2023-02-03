#include "stdafx.h"
#include "RuiAdvSkill_MoveState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"

using namespace Rui;

CAdvSkill_MoveState::CAdvSkill_MoveState()
{
}

CRuiState * CAdvSkill_MoveState::HandleInput(CRui * pRui)
{
	return nullptr;
}

CRuiState * CAdvSkill_MoveState::Tick(CRui * pRui, _float fTimeDelta)
{
	m_fBackStepTime += fTimeDelta;

	if (m_fBackStepTime <= 0.2f)
		pRui->Get_Transform()->Go_Backward(fTimeDelta *  1.2f, pRui->Get_NavigationCom());


	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
		return new CIdleState();
	}

	return nullptr;
}

CRuiState * CAdvSkill_MoveState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation(fTimeDelta * 0.8f);

	return nullptr;
}

void CAdvSkill_MoveState::Enter(CRui * pRui)
{
	m_eStateId = STATE_ID::STATE_ADVSKILL_COMMON;

	pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_ADVSKILL_MOVE);
	pRui->Set_AnimIndex(CRui::ANIM_ADVSKILL_MOVE);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_ADVSKILL_MOVE);
	pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CAdvSkill_MoveState::Exit(CRui * pRui)
{
}


