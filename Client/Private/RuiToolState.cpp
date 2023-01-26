#include "stdafx.h"
#include "RuiToolState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Rui;


CToolState::CToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, STATE_TYPE eType, _bool bIsContinue)
{
	m_iAnimIndex = iAnimIndex;
	m_iAnimIndex_Second = iAnimIndex_2;
	m_iAnimIndex_Third = iAnimIndex_3;
	m_eStateType = eType;
	m_bIsContinue = bIsContinue;
}

CRuiState * CToolState::HandleInput(CRui* pRui)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Set_OriginalFrame(pRui);

	return nullptr;
}

CRuiState * CToolState::Tick(CRui* pRui, _float fTimeDelta)
{
	
	pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());


	if (m_bIsContinue == false)
	{
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
		}
	}
	else if (m_bIsContinue == true)
	{
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CRuiState::TYPE_START:
				pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				if (m_iAnimIndex == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_LOOP, true);
				break;
			case Client::CRuiState::TYPE_LOOP:
				pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				if (m_iAnimIndex_Second == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_END, true);
				break;
			case Client::CRuiState::TYPE_END:
				if (m_iAnimIndex_Third == -1)
					return new CIdleState();
				else
					pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
				return new CIdleState();
				break;
			}
		}
	}



	return nullptr;
}

CRuiState * CToolState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CToolState::Enter(CRui * pRui)
{
	m_eStateId = STATE_END;


	if (m_iAnimIndex == -1)
		m_iAnimIndex = CRui::ANIM_IDLE;
	else if (m_iAnimIndex_Second == -1)
		m_iAnimIndex_Second = CRui::ANIM_IDLE;
	else if (m_iAnimIndex_Third == -1)
		m_iAnimIndex_Third = CRui::ANIM_IDLE;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex);
		pRui->Get_Model()->Set_LinearTime(m_iAnimIndex, 0.2f);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(m_iAnimIndex));
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Second);
		pRui->Get_Model()->Set_LinearTime(m_iAnimIndex_Second, 0.2f);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(m_iAnimIndex_Second));
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Third);
		pRui->Get_Model()->Set_LinearTime(m_iAnimIndex_Third, 0.2f);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(m_iAnimIndex_Third));
		break;
	}

}

void CToolState::Exit(CRui* pRui)
{

}

