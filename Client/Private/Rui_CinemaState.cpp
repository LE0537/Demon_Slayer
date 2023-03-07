#include "stdafx.h"
#include "..\Public\Rui_CinemaState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "Tanjiro.h"
#include "BattleDialog.h"

using namespace Rui;

CRui_CinemaState::CRui_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CRuiState * CRui_CinemaState::HandleInput(CRui * pRui)
{

	return nullptr;
}

CRuiState * CRui_CinemaState::Tick(CRui * pRui, _float fTimeDelta)
{

	switch (m_eScene)
	{
	case Client::Rui::CRui_CinemaState::SCENE_START:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CRui_CinemaState(CRui_CinemaState::SCENE_0);
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_0:
		m_fTime += fTimeDelta;

		if (m_fTime >= 2.67f)
			return new CRui_CinemaState(CRui_CinemaState::SCENE_1);

		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_1:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CRui_CinemaState(CRui_CinemaState::SCENE_2);
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_2:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
		/*	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_FADE, this);

			RELEASE_INSTANCE(CEffect_Manager);*/
			return new CRui_CinemaState(CRui_CinemaState::SCENE_3);
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_3:
		m_fTime += fTimeDelta;
		if (3.63f <= m_fTime)
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.5183f, 10.f, 56.1f, 1.f));
			pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(56.56f, pRui->Get_NavigationHeight().y, 50.03f, 1.f));
			pRui->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_BOUND, 20.f, 30.f, 0.f);

			return new CIdleState();
		}
		break;
	}
		




	

	return nullptr;
}

CRuiState * CRui_CinemaState::Late_Tick(CRui * pRui, _float fTimeDelta)
{

	pRui->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CRui_CinemaState::Enter(CRui * pRui)
{
	CGameInstance* pGameInstance = nullptr;
	CUI_Manager* pUI_Manager = nullptr;
	switch (m_eScene)
	{
	case Client::Rui::CRui_CinemaState::SCENE_START:
		pRui->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pRui->Get_Model()->Reset_Anim(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_START));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_START, 0.01f);
		pRui->Set_SplSkl(true);
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.5183f, pRui->Get_NavigationHeight().y, 56.1f, 1.f));
		pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(56.56f, pRui->Get_NavigationHeight().y, 50.03f, 1.f));
		

		pRui->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(66.97f, pRui->Get_NavigationHeight().y, 44.423f, 1.f));
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		pGameInstance = GET_INSTANCE(CGameInstance);
		((CCamera_Dynamic*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Camera")->Get_LayerFront()))->Start_CutScene(true, CCamera_Dynamic::CUTSCENE_RUI_SPC_START);

		RELEASE_INSTANCE(CGameInstance);
		pUI_Manager = GET_INSTANCE(CUI_Manager);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplCharNum(2);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplDialogStart(true);
		RELEASE_INSTANCE(CUI_Manager);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_SplSkr.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SplSkr.wav"), g_fEffect);
		break;
	case Client::Rui::CRui_CinemaState::SCENE_0:
	{
		pRui->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pRui->Get_Model()->Reset_Anim(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_0));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_0, 0.01f);

		//pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pRui->Get_NavigationHeight().y, 64.f, 1.f));
		//pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pRui->Get_NavigationHeight().y, 38.5f, 1.f));

		//pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		//pRui->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO2_PROJ1, pRui);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO2_REDWEB1, pRui->Get_BattleTarget());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO2_TREADL1, pRui->Get_WeaponWorld2());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO2_TREADR1, pRui->Get_WeaponWorld());

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Rui::CRui_CinemaState::SCENE_1:
	{
		pRui->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_1);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_1));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_1);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_1, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO3_REDWEB1, pRui->Get_BattleTarget());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO3_WEB1, pRui->Get_BattleTarget());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO3_TREADL1, pRui->Get_WeaponWorld2());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO3_TREADR1, pRui->Get_WeaponWorld());

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Rui::CRui_CinemaState::SCENE_2: 
	{
		pRui->Get_BattleTarget()->Set_Hp(-400);
		pRui->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_2);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_2));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_2);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_2, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO4_PROJ1, pRui);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO4_REDWEB1, pRui->Get_BattleTarget());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO4_REDWEB2, pRui->Get_BattleTarget());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO4_TREADL1, pRui->Get_WeaponWorld2());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_RUI_MO4_TREADR1, pRui->Get_WeaponWorld());

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Rui::CRui_CinemaState::SCENE_3:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_4:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_5:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_6:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

}

void CRui_CinemaState::Exit(CRui * pRui)
{
}

