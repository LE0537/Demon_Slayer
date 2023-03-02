#include "stdafx.h"
#include "..\Public\Kyoujuro_CinemaState.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include"Effect_Manager.h"

using namespace Kyoujuro;

CKyoujuro_CinemaState::CKyoujuro_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CKyoujuro_CinemaState::HandleInput(CKyoujuro * pKyoujuro)
{


	return nullptr;
}

CKyoujuroState * CKyoujuro_CinemaState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	_float fValue = 0.f;
	switch (m_eScene)
	{
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_START:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_START))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_START);
			return new CKyoujuro_CinemaState(SCENE_0);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_0:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_0))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_0);
			return new CKyoujuro_CinemaState(SCENE_1);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_1:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_1))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_1);
			return new CKyoujuro_CinemaState(SCENE_2);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_2:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_2))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_2);
			return new CKyoujuro_CinemaState(SCENE_3);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_3:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_3))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_3);
			return new CKyoujuro_CinemaState(SCENE_4);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_4:
		pKyoujuro->Get_Renderer()->Set_PointBlur(860, 460, 100.f, 0.1f, 0.5f);			//	PointBlur

		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_4))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_4);
			return new CKyoujuro_CinemaState(SCENE_5);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_5:
		m_fTime += fTimeDelta;
		fValue = 80.f + (20.f * sin(m_fTime * 10.f));
		pKyoujuro->Get_Renderer()->Set_PointBlur(640, 360, fValue, 0.5f, 0.8f);			//	PointBlur

		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_5))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_5);
			return new CKyoujuro_CinemaState(SCENE_6);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_6:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_6))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_6);
			return new CKyoujuro_CinemaState(SCENE_7);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_7:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_7))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_7);
			return new CKyoujuro_CinemaState(SCENE_8);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_8:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro_CinemaState::ANIM_SCENE_8))
		{
			pKyoujuro->Set_SplWeapon(false);
			pKyoujuro->Reset_SplWeapon();
			pKyoujuro->Get_Model()->Set_End(CKyoujuro_CinemaState::ANIM_SCENE_8);
			return new CKyoujuro_CinemaState(SCENE_END);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_END:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_SPLSKL_END))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_SPLSKL_END);
			return new CIdleState();
		}
		break;
	default:
		break;
	}

	

	return nullptr;
}

CKyoujuroState * CKyoujuro_CinemaState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{


	pKyoujuro->Get_Model()->Play_Animation_Skill(fTimeDelta);


	return nullptr;
}

void CKyoujuro_CinemaState::Enter(CKyoujuro * pKyoujuro)
{
	pKyoujuro->Set_SplSkl(true);

	CGameInstance* pGameInstance = nullptr;
	switch (m_eScene)
	{
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_START: {
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pKyoujuro->Get_NavigationHeight().y, 64.f, 1.f));
		pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.2f, pKyoujuro->Get_NavigationHeight().y, 64.f, 1.f));
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(XMVectorSet(52.f, pKyoujuro->Get_NavigationHeight().y, 64.f, 1.f));
		pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(52.f, pKyoujuro->Get_NavigationHeight().y, 64.f, 1.f));
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_START);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_START);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_START));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_START, 0.01f);
		pKyoujuro->Set_SplSkl(true);
		//pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pKyoujuro->Get_NavigationHeight().y, 64.f, 1.f));
		//pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pKyoujuro->Get_NavigationHeight().y, 38.5f, 1.f));

		//pKyoujuro->Get_Transform()->Set_PlayerLookAt();
		//pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt();

		pGameInstance = GET_INSTANCE(CGameInstance);
		((CCamera_Dynamic*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Camera")->Get_LayerFront()))->Start_CutScene(true, CCamera_Dynamic::CUTSCENE_RGK_START);
		

		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_0: {
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(false);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_0);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_0);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_0));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_0, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO2_BG, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO2_SWORD1, pKyoujuro->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO2_AURA1, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO2_AURA2, pKyoujuro);

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_1:
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(false);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_1));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_2:
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_2));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_2, 0.01f);

		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_3:
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_3));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_4:
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(true);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_060);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_4);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_4);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_4));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_4);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_4, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_5:
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(true);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_070); // 75
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_5);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_5);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_5));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_5);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_5, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_6:
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_080); // 78
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_6);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_6);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_6));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_6);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_6, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_7:
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_090);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_7);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_7);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_7));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_7);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_7, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_8:
		pKyoujuro->Set_SplWeapon(true);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_100);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_8);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_8);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_8));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_8);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_8, 0.01f);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_END:
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.5183f, pKyoujuro->Get_NavigationHeight().y, 56.1f, 1.f));
		pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(56.56f, pKyoujuro->Get_NavigationHeight().y, 50.03f, 1.f));

		pKyoujuro->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_KNOCKBACK, 0.f, 0.f, 8.f);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_SPLSKL_END);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIM_SPLSKL_END);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro::ANIM_SPLSKL_END));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_SPLSKL_END);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_SPLSKL_END, 0.01f);



		break;
	default:
		break;
	}


}

void CKyoujuro_CinemaState::Exit(CKyoujuro * pKyoujuro)
{
}

