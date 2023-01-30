#include "stdafx.h"
#include "RuiSkill_Shoot.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "AkazaShoot.h"

using namespace Rui;


CSkill_ShootState::CSkill_ShootState(STATE_TYPE eType)
{

	m_eStateType = eType;

}

CRuiState * CSkill_ShootState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CRuiState * CSkill_ShootState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());





	return nullptr;
}

CRuiState * CSkill_ShootState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	_int i = pRui->Get_Model()->Get_CurrentFrame();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;

	CAkazaShoot::AKAZASHOOTINFO	tInfo;
	tInfo.pPlayer = pRui;
	tInfo.pTarget = m_pTarget;

	if (m_fMove > 0.17f && m_iHit < 1)
	{
		CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_AkazaShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
			return nullptr;

		RELEASE_INSTANCE(CGameInstance);
		m_fMove = 0.f;
		++m_iHit;
	}

	RELEASE_INSTANCE(CGameInstance);

	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.3f);

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_ShootState(CRuiState::TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_ShootState(CRuiState::TYPE_END);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		}
	}
	

	return nullptr;
}

void CSkill_ShootState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_SKILL_SHOOTCOMMON;

	pRui->Get_Model()->Reset_Anim(pRui->Get_AnimIndex());
	
	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOT_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOT_0, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOT_0);
		pRui->Get_Model()->Set_FrameNum(CRui::ANIM_SKILL_SHOOT_0, 100);
		pRui->Get_Model()->Set_FrameTime(CRui::ANIM_SKILL_SHOOT_0, 1, 30, 1.2f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_SKILL_SHOOT_0, 1, 30);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOT_1);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOT_1, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOT_1);
		pRui->Get_Model()->Set_FrameNum(CRui::ANIM_SKILL_SHOOT_1, 100);
		pRui->Get_Model()->Set_FrameTime(CRui::ANIM_SKILL_SHOOT_1, 1, 30, 1.2f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_SKILL_SHOOT_1, 1, 30);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOT_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOT_2, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOT_2);
		pRui->Get_Model()->Set_FrameNum(CRui::ANIM_SKILL_SHOOT_2, 100);
		pRui->Get_Model()->Set_FrameTime(CRui::ANIM_SKILL_SHOOT_2, 1, 30, 1.2f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_SKILL_SHOOT_2, 1, 100);
		break;
	}

}

void CSkill_ShootState::Exit(CRui* pRui)
{
	if (m_eStateType == TYPE_END)
	{
		pRui->Get_Model()->Set_FrameNum(CRui::ANIM_SKILL_SHOOT_2, 100);
		pRui->Get_Model()->Set_FrameTime(CRui::ANIM_SKILL_SHOOT_2, 0, 100, 1.f);
		pRui->Get_Model()->Set_UsingFrame(CRui::ANIM_SKILL_SHOOT_2, 0, 100);
		pRui->Get_Model()->Reset_Anim(pRui->Get_AnimIndex());
	}
}

