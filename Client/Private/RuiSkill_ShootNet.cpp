#include "stdafx.h"
#include "RuiSkill_ShootNet.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Rui;


CSkill_ShootNetState::CSkill_ShootNetState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	


	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CRuiState * CSkill_ShootNetState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CRuiState * CSkill_ShootNetState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_ShootNetState(CRuiState::TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_ShootNetState(CRuiState::TYPE_END);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	return nullptr;
}

CRuiState * CSkill_ShootNetState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_ShootNetState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_SKILL_SHOOTNET;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOTNET_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOTNET_0, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOTNET_0);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOTNET_1);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOTNET_1, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOTNET_1);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOTNET_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOTNET_2, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOTNET_2);
		break;
	}

}

void CSkill_ShootNetState::Exit(CRui* pRui)
{

}

