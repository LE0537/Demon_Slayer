#include "stdafx.h"
#include "RuiGuardState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "RuiGuardAdvState.h"
#include "Layer.h"

using namespace Rui;


CGuardState::CGuardState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiState * CGuardState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pRui->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_O) && m_eStateType != STATE_TYPE::TYPE_END)
		{
			pRui->Set_bGuard(true);
			if (pGameInstance->Key_Down(DIK_A) || pGameInstance->Key_Down(DIK_W) || pGameInstance->Key_Down(DIK_S) || pGameInstance->Key_Down(DIK_D))
				return new CGuardAdvState();
			else
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_C) && m_eStateType != STATE_TYPE::TYPE_END)
		{
			pRui->Set_bGuard(true);
			if (pGameInstance->Key_Down(DIK_LEFT) || pGameInstance->Key_Down(DIK_UP) || pGameInstance->Key_Down(DIK_DOWN) || pGameInstance->Key_Down(DIK_RIGHT))
				return new CGuardAdvState();
			else
				return new CGuardState(STATE_TYPE::TYPE_LOOP);
		}
		else
			return new CGuardState(STATE_TYPE::TYPE_END);
		break;
	default:
		break;
	}
	

	return nullptr;
}

CRuiState * CGuardState::Tick(CRui* pRui, _float fTimeDelta)
{

	pRui->Get_Model()->Set_Loop(CRui::ANIM_GUARD_0);
	//pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_GUARD_1);
	pRui->Get_Model()->Set_Loop(CRui::ANIM_GUARD_2);
	pRui->Get_Model()->Set_LinearTime(CRui::ANIM_GUARD_0, 0.0f);
	pRui->Get_Model()->Set_LinearTime(CRui::ANIM_GUARD_1, 0.01f);
	pRui->Get_Model()->Set_LinearTime(CRui::ANIM_GUARD_2, 0.0f);

	

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{  
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CGuardState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP:
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CRuiState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}


	return nullptr;
}

CRuiState * CGuardState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pRui->Get_Transform()->Set_PlayerLookAt(vLooAt);


	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.2f);

	RELEASE_INSTANCE(CGameInstance);

	return nullptr;
}

void CGuardState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_GUARD;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_GUARD_0);
		pRui->Set_AnimIndex(CRui::ANIM_GUARD_0);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_GUARD_1);
		pRui->Set_AnimIndex(CRui::ANIM_GUARD_1);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_GUARD_2);
		pRui->Set_AnimIndex(CRui::ANIM_GUARD_2);
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	}

}

void CGuardState::Exit(CRui* pRui)
{
}

