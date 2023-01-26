#include "stdafx.h"
#include "KyoujuroToolState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Kyoujuro;


CToolState::CToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, STATE_TYPE eType, _bool bIsContinue)
{
	m_iAnimIndex = iAnimIndex;
	m_iAnimIndex_Second = iAnimIndex_2;
	m_iAnimIndex_Third = iAnimIndex_3;
	m_eStateType = eType;
	m_bIsContinue = bIsContinue;
}

CKyoujuroState * CToolState::HandleInput(CKyoujuro * pKyoujuro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return nullptr;
}

CKyoujuroState * CToolState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	
	pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());


	if (m_bIsContinue == false)
	{
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState();
		}
	}
	else if (m_bIsContinue == true)
	{
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CKyoujuroState::TYPE_START:
				pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
				if (m_iAnimIndex == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_LOOP, true);
				break;
			case Client::CKyoujuroState::TYPE_LOOP:
				pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
				if (m_iAnimIndex_Second == -1)
					return new CIdleState();
				else
					return new CToolState(m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_END, true);
				break;
			case Client::CKyoujuroState::TYPE_END:
				pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
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

CKyoujuroState * CToolState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CToolState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_END;


	if (m_iAnimIndex == -1)
		m_iAnimIndex = CKyoujuro::ANIM_IDLE;
	else if (m_iAnimIndex_Second == -1)
		m_iAnimIndex_Second = CKyoujuro::ANIM_IDLE;
	else if (m_iAnimIndex_Third == -1)
		m_iAnimIndex_Third = CKyoujuro::ANIM_IDLE;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex);
		pKyoujuro->Get_Model()->Set_LinearTime(m_iAnimIndex, 0.2f);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(m_iAnimIndex));
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Second);
		pKyoujuro->Get_Model()->Set_LinearTime(m_iAnimIndex_Second, 0.2f);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(m_iAnimIndex_Second));
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Third);
		pKyoujuro->Get_Model()->Set_LinearTime(m_iAnimIndex_Third, 0.2f);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(m_iAnimIndex_Third));
		break;
	}

}

void CToolState::Exit(CKyoujuro * pKyoujuro)
{

}

