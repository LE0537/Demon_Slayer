#include "stdafx.h"
#include "EnmuToolState.h"
#include "EnmuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "Enmu.h"

using namespace Enmu;


CToolState::CToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, STATE_TYPE eType, _bool bIsContinue)
{
	m_iAnimIndex = iAnimIndex;
	m_iAnimIndex_Second = iAnimIndex_2;
	m_iAnimIndex_Third = iAnimIndex_3;
	m_eStateType = eType;
	m_bIsContinue = bIsContinue;
}

CEnmuState * CToolState::HandleInput(CEnmu* pEnmu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return nullptr;
}

CEnmuState * CToolState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	
	pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());


	if (m_bIsContinue == false)
	{
		if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
		{
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CIdleState();
		}
	}
	else if (m_bIsContinue == true)
	{
		if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CEnmuState::TYPE_START:
				pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
				if (m_iAnimIndex == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_LOOP, true);
				break;
			case Client::CEnmuState::TYPE_LOOP:
				pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
				if (m_iAnimIndex_Second == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_END, true);
				break;
			case Client::CEnmuState::TYPE_END:
				if (m_iAnimIndex_Third == -1)
					return new CIdleState();
				else
					pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
				return new CIdleState();
				break;
			}
		}
	}



	return nullptr;
}

CEnmuState * CToolState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CToolState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_END;

	if (m_iAnimIndex == -1)
		m_iAnimIndex = CEnmu::ANIM_IDLE;
	else if (m_iAnimIndex_Second == -1)
		m_iAnimIndex_Second = CEnmu::ANIM_IDLE;
	else if (m_iAnimIndex_Third == -1)
		m_iAnimIndex_Third = CEnmu::ANIM_IDLE;

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex);
		pEnmu->Get_Model()->Set_LinearTime(m_iAnimIndex, 0.2f);
		pEnmu->Set_AnimIndex(static_cast<CEnmu::ANIMID>(m_iAnimIndex));
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Second);
		pEnmu->Get_Model()->Set_LinearTime(m_iAnimIndex_Second, 0.2f);
		pEnmu->Set_AnimIndex(static_cast<CEnmu::ANIMID>(m_iAnimIndex_Second));
		break;
	case Client::CEnmuState::TYPE_END:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Third);
		pEnmu->Get_Model()->Set_LinearTime(m_iAnimIndex_Third, 0.2f);
		pEnmu->Set_AnimIndex(static_cast<CEnmu::ANIMID>(m_iAnimIndex_Third));
		break;
	}
}

void CToolState::Exit(CEnmu* pEnmu)
{

}



