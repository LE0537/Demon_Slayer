#include "stdafx.h"
#include "..\Public\RuiSplSkrStartState.h"
#include "GameInstance.h"
#include "Rui_CinemaState.h"
#include "RuiIdleState.h"
#include "RuiSplColl.h"
using namespace Rui;



CSplSkrStartState::CSplSkrStartState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiState * CSplSkrStartState::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CSplSkrStartState::Tick(CRui* pRui, _float fTimeDelta)
{
	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			if (pRui->Get_HitSpl())
			{
				pRui->Set_HitSpl(false);
				pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				pRui->Get_BattleTarget()->Play_Scene();
				return new CRui_CinemaState(CRui_CinemaState::SCENE_START);
			}
			else
			{
				pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				return new CIdleState();
			}
			break;
		case Client::CRuiState::TYPE_LOOP:

			break;
		case Client::CRuiState::TYPE_END:
			break;
		case Client::CRuiState::TYPE_DEFAULT:
			break;
		case Client::CRuiState::TYPE_CHANGE:
			break;
		default:
			break;
		}

		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

	}


	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		break;
	case Client::CRuiState::TYPE_LOOP:
		break;
	case Client::CRuiState::TYPE_END:
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	case Client::CRuiState::TYPE_CHANGE:
		break;
	default:
		break;
	}



	return nullptr;
}

CRuiState * CSplSkrStartState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pRui->Get_Transform()->Set_PlayerLookAt(vLooAt);

	if (m_eStateType == CRuiState::TYPE_START)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_fMove += fTimeDelta;

		CRuiSplColl::RUISPLINFO	tInfo;
		tInfo.pPlayer = pRui;
		tInfo.pTarget = m_pTarget;

		if (m_iHit < 1)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			tInfo.iIndex = 0;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiSplColl"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;


			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}

		RELEASE_INSTANCE(CGameInstance);
	}

	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSplSkrStartState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ATK_1;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_SPLSKL_START_0);
		pRui->Set_AnimIndex(CRui::ANIM_SPLSKL_START_0);
		pRui->Get_Model()->Set_Loop(CRui::ANIM_SPLSKL_START_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SPLSKL_START_0, 0.01f);
		break;
	case Client::CRuiState::TYPE_END:
		break;
	case Client::CRuiState::TYPE_DEFAULT:
		break;
	case Client::CRuiState::TYPE_CHANGE:
		break;
	default:
		break;
	}


}

void CSplSkrStartState::Exit(CRui* pRui)
{
}

void CSplSkrStartState::Move(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
}
