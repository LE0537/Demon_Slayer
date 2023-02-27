#include "stdafx.h"
#include "EnmuBossToolState.h"
#include "EnmuBossIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"


using namespace EnmuBoss;


CBossToolState::CBossToolState(_uint PartsIndex, _uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, STATE_TYPE eType, _bool bIsContinue)
{
	m_iAnimIndex = iAnimIndex;
	m_iAnimIndex_Second = iAnimIndex_2;
	m_iAnimIndex_Third = iAnimIndex_3;
	m_eStateType = eType;
	m_bIsContinue = bIsContinue;
	m_iPartsIndex = PartsIndex;
}

CEnmuBossState * CBossToolState::HandleInput(CEnmuBoss* pEnmuBoss)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return nullptr;
}

CEnmuBossState * CBossToolState::Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta)
{
	
	pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_Loop(pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_AnimIndex());


	if (m_bIsContinue == false)
	{
		if (pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Get_End(pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_AnimIndex()))
		{
			pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_End(pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_AnimIndex());
			return new CIdleState();
		}
	}
	else if (m_bIsContinue == true)
	{
		if (pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Get_End(pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CEnmuBossState::TYPE_START:
				pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_End(pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_AnimIndex());
				if (m_iAnimIndex == -1)
					return new CIdleState();
				else
					return new CBossToolState(m_iPartsIndex, m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_LOOP, true);
				break;
			case Client::CEnmuBossState::TYPE_LOOP:
				pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_End(pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_AnimIndex());
				if (m_iAnimIndex_Second == -1)
					return new CIdleState();
				else
					return new CBossToolState(m_iPartsIndex, m_iAnimIndex, m_iAnimIndex_Second, m_iAnimIndex_Third, TYPE_END, true);
				break;
			case Client::CEnmuBossState::TYPE_END:
				if (m_iAnimIndex_Third == -1)
					return new CIdleState();
				else
					pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_End(pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_AnimIndex());
				return new CIdleState();
				break;
			}
		}
	}



	return nullptr;
}

CEnmuBossState * CBossToolState::Late_Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta)
{
	pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CBossToolState::Enter(CEnmuBoss* pEnmuBoss)
{
	m_eStateId = STATE_END;

	if (m_iAnimIndex == -1)
		m_iAnimIndex = CEnmuBoss::ANIM_IDLE;
	else if (m_iAnimIndex_Second == -1)
		m_iAnimIndex_Second = CEnmuBoss::ANIM_IDLE;
	else if (m_iAnimIndex_Third == -1)
		m_iAnimIndex_Third = CEnmuBoss::ANIM_IDLE;

	switch (m_eStateType)
	{
	case Client::CEnmuBossState::TYPE_START:
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex);
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_LinearTime(m_iAnimIndex, 0.2f);
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Set_AnimIndex(static_cast<CEnmuBoss::ANIMID>(m_iAnimIndex));
		break;
	case Client::CEnmuBossState::TYPE_LOOP:
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Second);
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_LinearTime(m_iAnimIndex_Second, 0.2f);
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Set_AnimIndex(static_cast<CEnmuBoss::ANIMID>(m_iAnimIndex_Second));
		break;
	case Client::CEnmuBossState::TYPE_END:
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_CurrentAnimIndex(m_iAnimIndex_Third);
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Get_Model()->Set_LinearTime(m_iAnimIndex_Third, 0.2f);
		pEnmuBoss->Get_EnmuPartsList()[m_iPartsIndex]->Set_AnimIndex(static_cast<CEnmuBoss::ANIMID>(m_iAnimIndex_Third));
		break;
	}

}

void CBossToolState::Exit(CEnmuBoss* pEnmuBoss)
{

}



