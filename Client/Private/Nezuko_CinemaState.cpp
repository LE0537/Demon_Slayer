#include "stdafx.h"
#include "..\Public\Nezuko_CinemaState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "BattleDialog.h"
#include "Effect_Manager.h"
using namespace Nezuko;

CNezuko_CinemaState::CNezuko_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CNezukoState * CNezuko_CinemaState::HandleInput(CNezuko * pNezuko)
{
	CNezukoState* pState = nullptr;
	/*CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Key_Down(DIK_F3) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_START);
	if (pGameInstance->Key_Down(DIK_F4) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_0);
	if (pGameInstance->Key_Down(DIK_F5) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_1);
	if (pGameInstance->Key_Down(DIK_F6) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_2);
	if (pGameInstance->Key_Down(DIK_F7) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_3);
	if (pGameInstance->Key_Down(DIK_F8) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_4);
	if (pGameInstance->Key_Down(DIK_F9) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_5);
	if (pGameInstance->Key_Down(DIK_CAPSLOCK) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_6);
	if (pGameInstance->Key_Down(DIK_PGUP) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_7);
	if (pGameInstance->Key_Down(DIK_PGDN) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_8);
	if (pGameInstance->Key_Down(DIK_END) && !pNezuko->Get_StoryKey())
		pState = new CNezuko_CinemaState(CNezuko_CinemaState::CINEMASCENE::SCENE_9);
	RELEASE_INSTANCE(CGameInstance);

	if (nullptr != pState)
		pNezuko->Get_BattleTarget()->Play_Scene();*/

	return pState;
}

CNezukoState * CNezuko_CinemaState::Tick(CNezuko * pNezuko, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CNezuko_CinemaState::SCENE_START:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_START))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_START);
			return new CNezuko_CinemaState(SCENE_0);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_0:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_0))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_0);
			return new CNezuko_CinemaState(SCENE_1);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_1:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_1))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_1);
			return new CNezuko_CinemaState(SCENE_2);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_2:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_2))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_2);
			return new CNezuko_CinemaState(SCENE_3);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_3:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_3))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_3);
			return new CNezuko_CinemaState(SCENE_4);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_4:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_4))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_4);
			return new CNezuko_CinemaState(SCENE_5);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_5:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_5))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_5);
			return new CNezuko_CinemaState(SCENE_6);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_6:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_6))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_6);
			return new CNezuko_CinemaState(SCENE_7);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_7:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_7))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_7);
			return new CNezuko_CinemaState(SCENE_8);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_8:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_8))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_8);
			return new CNezuko_CinemaState(SCENE_9);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_9:
		if (pNezuko->Get_Model()->Get_End(CNezuko_CinemaState::ANIM_SCENE_9))
		{
			pNezuko->Get_Model()->Set_End(CNezuko_CinemaState::ANIM_SCENE_9);
		/*	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_FADE, this);

			RELEASE_INSTANCE(CEffect_Manager);*/
			return new CNezuko_CinemaState(SCENE_END);
		}
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_END:
		if (pNezuko->Get_Model()->Get_End(CNezuko::ANIM_SPLSKL_END))
		{
			pNezuko->Get_Model()->Set_End(CNezuko::ANIM_SPLSKL_END);
			return new CIdleState();
		}
		break;
	default:
		break;
	}


	return nullptr;
}

CNezukoState * CNezuko_CinemaState::Late_Tick(CNezuko * pNezuko, _float fTimeDelta)
{

	pNezuko->Get_Model()->Play_Animation_Skill(fTimeDelta);


	return nullptr;
}

void CNezuko_CinemaState::Enter(CNezuko * pNezuko)
{
	CGameInstance* pGameInstance = nullptr;
	CUI_Manager* pUI_Manager = nullptr;
	switch (m_eScene)
	{
	case Client::Nezuko::CNezuko_CinemaState::SCENE_START:
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(62.66f, pNezuko->Get_NavigationHeight().y, 47.29f, 1.f));
		pNezuko->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(62.86f, pNezuko->Get_NavigationHeight().y, 47.29f, 1.f));
		pNezuko->Get_Transform()->Set_PlayerLookAt(XMVectorSet(56.46f, pNezuko->Get_NavigationHeight().y, 56.46f, 1.f));
		pNezuko->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(56.46f, pNezuko->Get_NavigationHeight().y, 56.46f, 1.f));
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_START);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_START);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_START));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_START);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_START, 0.01f);

		pGameInstance = GET_INSTANCE(CGameInstance);
		((CCamera_Dynamic*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Camera")->Get_LayerFront()))->Start_CutScene(true, CCamera_Dynamic::CUTSCENE_NZK_START);
		RELEASE_INSTANCE(CGameInstance);

		pNezuko->Set_SplSkl(true);

		pUI_Manager = GET_INSTANCE(CUI_Manager);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplCharNum(4);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplDialogStart(true);
		RELEASE_INSTANCE(CUI_Manager);

		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Nezuko_SplSkr.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_SE_SplSkr.wav"), g_fEffect);
		break;
	case Client::Nezuko::CNezuko_CinemaState::SCENE_0: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_0);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_0);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_0));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_0, 0.01f);
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO2_BLOODL, pNezuko->Get_WeaponWorld2());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO2_BLOODR, pNezuko->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO2_DIST, pNezuko);

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_1: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_1);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_1);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_1));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_1, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO3_BGWIND, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO3_HANDL, pNezuko->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO3_PROJ1, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO3_RUN1, pNezuko);

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_2: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_2);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_2);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_2));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_2, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO4_CLAW1, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO4_BLOODL, pNezuko->Get_WeaponWorld2());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO4_BLOODR, pNezuko->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO4_SHOCK1, pNezuko);

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_3: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_3);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_3);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_3));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_3);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_3, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO5_SHOCK1, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO5_BLOOD1, pNezuko->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO5_CLAW2, pNezuko->Get_WeaponWorld2());

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_4: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_060);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_4);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_4);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_4));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_4);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_4, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO6_PROJ, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO6_BLOODL, pNezuko->Get_WeaponWorld());

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_5: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_070);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_5);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_5);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_5));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_5);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_5, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO7_BG1, pNezuko);

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_6: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_080);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_6);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_6);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_6));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_6);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_6, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO8_BG1, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO8_GROUND1, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO8_HAND1, pNezuko->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO8_HAND2, pNezuko->Get_WeaponWorld2());

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_7: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_090);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_7);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_7);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_7));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_7);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_7, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO9_HAND1, pNezuko->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO9_HAND2, pNezuko->Get_WeaponWorld2());

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_8: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_100);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_8);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_8);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_8));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_8);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_8, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO10_GROUND, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO10_HAND1, pNezuko->Get_WeaponWorld());

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_9: {
		pNezuko->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_110);
		pNezuko->Get_Model()->Reset_Anim(CNezuko_CinemaState::ANIM_SCENE_9);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko_CinemaState::ANIM_SCENE_9);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko_CinemaState::ANIM_SCENE_9));
		pNezuko->Get_Model()->Set_Loop(CNezuko_CinemaState::ANIM_SCENE_9);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko_CinemaState::ANIM_SCENE_9, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO11_GROUND, pNezuko);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_MO11_PROJ, pNezuko);

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Nezuko::CNezuko_CinemaState::SCENE_END: {
		pNezuko->Get_BattleTarget()->Set_Hp(-400);
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.5183f, pNezuko->Get_NavigationHeight().y, 56.1f, 1.f));
		pNezuko->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(56.56f, pNezuko->Get_NavigationHeight().y, 50.03f, 1.f));

		pNezuko->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_BOUND, 20.f, 30.f, 2.f);
		pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIM_SPLSKL_END);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SPLSKL_END);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CNezuko::ANIM_SPLSKL_END));
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_SPLSKL_END);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SPLSKL_END, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_NE_END_GROUND, pNezuko->Get_BattleTarget());

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	default:
		break;
	}



}

void CNezuko_CinemaState::Exit(CNezuko * pNezuko)
{

}

