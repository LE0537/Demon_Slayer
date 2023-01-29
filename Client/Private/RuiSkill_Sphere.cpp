#include "stdafx.h"
#include "RuiSkill_Sphere.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Rui;


CSkill_SphereState::CSkill_SphereState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	


	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CRuiState * CSkill_SphereState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CRuiState * CSkill_SphereState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_SphereState(CRuiState::TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_SphereState(CRuiState::TYPE_END);
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

CRuiState * CSkill_SphereState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.3f);

	return nullptr;
}

void CSkill_SphereState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_SKILL_SPHERE;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SPHERE_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SPHERE_0, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SPHERE_0);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SPHERE_1);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SPHERE_1, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SPHERE_1);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SPHERE_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SPHERE_2, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SPHERE_2);
		break;
	}

}

void CSkill_SphereState::Exit(CRui* pRui)
{

}

