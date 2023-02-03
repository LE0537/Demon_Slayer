#include "stdafx.h"
#include "ShinobuToolState.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Shinobu;


CToolState::CToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, STATE_TYPE eType, _bool bIsContinue)
{
	m_iAnimIndex = iAnimIndex;
	m_iAnimIndex_Second = iAnimIndex_2;
	m_iAnimIndex_Third = iAnimIndex_3;
	m_eStateType = eType;
	m_bIsContinue = bIsContinue;
}

CShinobuState * CToolState::HandleInput(CShinobu* pShinobu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return nullptr;
}

CShinobuState * CToolState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	
	pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());


	if (m_bIsContinue == false)
	{
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
		}
	}
	else if (m_bIsContinue == true)
	{
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CShinobuState::TYPE_START:
				pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				if (m_iAnimIndex == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_LOOP, true);
				break;
			case Client::CShinobuState::TYPE_LOOP:
				pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				if (m_iAnimIndex_Second == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_END, true);
				break;
			case Client::CShinobuState::TYPE_END:
				if (m_iAnimIndex_Third == -1)
					return new CIdleState();
				else
					pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				return new CIdleState();
				break;
			}
		}
	}



	return nullptr;
}

CShinobuState * CToolState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CToolState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_END;

	if (m_iAnimIndex == -1)
		m_iAnimIndex = CShinobu::ANIM_IDLE;
	else if (m_iAnimIndex_Second == -1)
		m_iAnimIndex_Second = CShinobu::ANIM_IDLE;
	else if (m_iAnimIndex_Third == -1)
		m_iAnimIndex_Third = CShinobu::ANIM_IDLE;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex);
		pShinobu->Get_Model()->Set_LinearTime(m_iAnimIndex, 0.2f);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(m_iAnimIndex));
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Second);
		pShinobu->Get_Model()->Set_LinearTime(m_iAnimIndex_Second, 0.2f);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(m_iAnimIndex_Second));
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Third);
		pShinobu->Get_Model()->Set_LinearTime(m_iAnimIndex_Third, 0.2f);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(m_iAnimIndex_Third));
		break;
	}

	pShinobu->Get_Model()->Set_FrameNum(pShinobu->Get_AnimIndex(), 100);

}

void CToolState::Exit(CShinobu* pShinobu)
{

}



