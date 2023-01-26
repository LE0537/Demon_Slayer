#include "stdafx.h"
#include "AkazaToolState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Akaza;


CToolState::CToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, STATE_TYPE eType, _bool bIsContinue)
{
	m_iAnimIndex = iAnimIndex;
	m_iAnimIndex_Second = iAnimIndex_2;
	m_iAnimIndex_Third = iAnimIndex_3;
	m_eStateType = eType;
	m_bIsContinue = bIsContinue;
}

CAkazaState * CToolState::HandleInput(CAkaza* pAkaza)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return nullptr;
}

CAkazaState * CToolState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());


	if (m_bIsContinue == false)
	{
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
		}
	}
	else if (m_bIsContinue == true)
	{
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CAkazaState::TYPE_START:
				pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
				if (m_iAnimIndex == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_LOOP, true);
				break;
			case Client::CAkazaState::TYPE_LOOP:
				pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
				if (m_iAnimIndex_Second == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_END, true);
				break;
			case Client::CAkazaState::TYPE_END:
				pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
				if (m_iAnimIndex_Third == -1)
					return new CIdleState();
				else
					return new CIdleState();
				break;
			}
		}
	}



	return nullptr;
}

CAkazaState * CToolState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CToolState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_END;



	if (m_iAnimIndex == -1)
		m_iAnimIndex = CAkaza::ANIM_IDLE;
	else if (m_iAnimIndex_Second == -1)
		m_iAnimIndex_Second = CAkaza::ANIM_IDLE;
	else if (m_iAnimIndex_Third == -1)
		m_iAnimIndex_Third = CAkaza::ANIM_IDLE;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex);
		pAkaza->Get_Model()->Set_LinearTime(m_iAnimIndex, 0.2f);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(m_iAnimIndex));
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Second);
		pAkaza->Get_Model()->Set_LinearTime(m_iAnimIndex_Second, 0.2f);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(m_iAnimIndex_Second));
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Third);
		pAkaza->Get_Model()->Set_LinearTime(m_iAnimIndex_Third, 0.2f);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(m_iAnimIndex_Third));
		break;
	}

}

void CToolState::Exit(CAkaza* pAkaza)
{

}

