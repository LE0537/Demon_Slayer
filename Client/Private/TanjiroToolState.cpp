#include "stdafx.h"
#include "TanjiroToolState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CToolState::CToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, STATE_TYPE eType, _bool bIsContinue)
{
	m_iAnimIndex = iAnimIndex;
	m_iAnimIndex_Second = iAnimIndex_2;
	m_iAnimIndex_Third = iAnimIndex_3;
	m_eStateType = eType;
	m_bIsContinue = bIsContinue;
}

CTanjiroState * CToolState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return nullptr;
}

CTanjiroState * CToolState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());


	if (m_bIsContinue == false)
	{
		if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
		}
	}
	else if (m_bIsContinue == true)
	{
		if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CTanjiroState::TYPE_START:
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				if (m_iAnimIndex == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_LOOP, true);
				break;
			case Client::CTanjiroState::TYPE_LOOP:
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				if (m_iAnimIndex_Second == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_END, true);
				break;
			case Client::CTanjiroState::TYPE_END:
				if (m_iAnimIndex_Third == -1)
					return new CIdleState();
				else
					pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				return new CIdleState();
				break;
			}
		}
	}



	return nullptr;
}

CTanjiroState * CToolState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CToolState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_END;

	if (m_iAnimIndex == -1)
		m_iAnimIndex = CTanjiro::ANIM_IDLE;
	else if (m_iAnimIndex_Second == -1)
		m_iAnimIndex_Second = CTanjiro::ANIM_IDLE;
	else if (m_iAnimIndex_Third == -1)
		m_iAnimIndex_Third = CTanjiro::ANIM_IDLE;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex);
		pTanjiro->Get_Model()->Set_LinearTime(m_iAnimIndex, 0.2f);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(m_iAnimIndex));
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Second);
		pTanjiro->Get_Model()->Set_LinearTime(m_iAnimIndex_Second, 0.2f);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(m_iAnimIndex_Second));
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Third);
		pTanjiro->Get_Model()->Set_LinearTime(m_iAnimIndex_Third, 0.2f);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(m_iAnimIndex_Third));
		break;
	}

}

void CToolState::Exit(CTanjiro * pTanjiro)
{

}



