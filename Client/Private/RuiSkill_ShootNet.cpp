#include "stdafx.h"
#include "RuiSkill_ShootNet.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiShoot.h"

using namespace Rui;


CSkill_ShootNetState::CSkill_ShootNetState(STATE_TYPE eType)
{
	m_eStateType = eType;

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
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);

	if (m_eStateType == CRuiState::TYPE_LOOP)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_fMove += fTimeDelta;

		CRuiShoot::RUISHOOTINFO	tInfo;
		tInfo.pPlayer = pRui;
		tInfo.pTarget = m_pTarget;

		if (m_iHit < 1)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			tInfo.iIndex = 0;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;
			tInfo.iIndex = 1;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;
			tInfo.iIndex = 2;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}

		RELEASE_INSTANCE(CGameInstance);
	}
	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.5f);

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

