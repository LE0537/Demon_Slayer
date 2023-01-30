#include "stdafx.h"
#include "NezukoToolState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Nezuko;


CToolState::CToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, STATE_TYPE eType, _bool bIsContinue)
{
	m_iAnimIndex = iAnimIndex;
	m_iAnimIndex_Second = iAnimIndex_2;
	m_iAnimIndex_Third = iAnimIndex_3;
	m_eStateType = eType;
	m_bIsContinue = bIsContinue;
}

CNezukoState * CToolState::HandleInput(CNezuko* pNezuko)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return nullptr;
}

CNezukoState * CToolState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	
	pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());


	if (m_bIsContinue == false)
	{
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
		}
	}
	else if (m_bIsContinue == true)
	{
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CNezukoState::TYPE_START:
				pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
				if (m_iAnimIndex == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_LOOP, true);
				break;
			case Client::CNezukoState::TYPE_LOOP:
				pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
				if (m_iAnimIndex_Second == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_END, true);
				break;
			case Client::CNezukoState::TYPE_END:
				if (m_iAnimIndex_Third == -1)
					return new CIdleState();
				else
					pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
				return new CIdleState();
				break;
			}
		}
	}



	return nullptr;
}

CNezukoState * CToolState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CToolState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_END;

	if (m_iAnimIndex == -1)
		m_iAnimIndex = CNezuko::ANIM_IDLE;
	else if (m_iAnimIndex_Second == -1)
		m_iAnimIndex_Second = CNezuko::ANIM_IDLE;
	else if (m_iAnimIndex_Third == -1)
		m_iAnimIndex_Third = CNezuko::ANIM_IDLE;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex);
		pNezuko->Get_Model()->Set_LinearTime(m_iAnimIndex, 0.2f);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(m_iAnimIndex));
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Second);
		pNezuko->Get_Model()->Set_LinearTime(m_iAnimIndex_Second, 0.2f);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(m_iAnimIndex_Second));
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Third);
		pNezuko->Get_Model()->Set_LinearTime(m_iAnimIndex_Third, 0.2f);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(m_iAnimIndex_Third));
		break;
	}

}

void CToolState::Exit(CNezuko* pNezuko)
{

}



