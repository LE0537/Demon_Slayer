#include "stdafx.h"
#include "RuiGuardHitState.h"
#include "RuiIdleState.h"
#include "RuiGuardState.h"
#include "GameInstance.h"
#include "Layer.h"

using namespace Rui;


CGuardHitState::CGuardHitState(STATE_TYPE eState)
{
	m_eStateType = eState;
}

CRuiState * CGuardHitState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CRuiState * CGuardHitState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_bGuard(true);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_GUARD_HIT_0);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_GUARD_HIT_1);
	pRui->Get_Model()->Set_LinearTime(CRui::ANIM_GUARD_HIT_0, 0.2f);
	pRui->Get_Model()->Set_LinearTime(CRui::ANIM_GUARD_HIT_1, 0.2f);

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		default:
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	return nullptr;






	return nullptr;
}

CRuiState * CGuardHitState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Transform()->Go_Backward(fTimeDelta * 0.3f, pRui->Get_NavigationCom());
		break;
	case Client::CRuiState::TYPE_LOOP:
		if (m_fTime < 0.3f)
		{
			pRui->Get_Transform()->Go_Backward(fTimeDelta * 0.6f, pRui->Get_NavigationCom());
		}
		break;
	default:
		break;
	}
	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CGuardHitState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_GUARD_HIT;


	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_GUARD_HIT_0);
		pRui->Set_AnimIndex(CRui::ANIM_GUARD_HIT_0);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_GUARD_HIT_1);
		pRui->Set_AnimIndex(CRui::ANIM_GUARD_HIT_1);
		break;
	}



}

void CGuardHitState::Exit(CRui* pRui)
{
	//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_ADV);
}

