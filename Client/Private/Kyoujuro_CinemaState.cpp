#include "stdafx.h"
#include "..\Public\Kyoujuro_CinemaState.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include"Effect_Manager.h"
#include "Terrain.h"
#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"
#include "Effect_AnimMesh.h"
#include "BattleDialog.h"

using namespace Kyoujuro;

CKyoujuro_CinemaState::CKyoujuro_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CKyoujuro_CinemaState::HandleInput(CKyoujuro * pKyoujuro)
{
	CKyoujuroState* pState = nullptr;

	return pState;
}

CKyoujuroState * CKyoujuro_CinemaState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	CGameInstance* pGameInstance = nullptr;
	list<CGameObject*> plistMesh;
	list<CGameObject*> plistMeshInst;
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
		m_fTime += fTimeDelta;
		if (1.23f < m_fTime)
		{
			pGameInstance = GET_INSTANCE(CGameInstance);
			((CTerrain*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Terrain")->Get_LayerFront()))->Set_SplRender(true);
			plistMesh = pGameInstance->Find_Layer(g_iLevel, L"Layer_MeshObj_Static")->Get_ObjectList();
			plistMeshInst = pGameInstance->Find_Layer(g_iLevel, L"Layer_MeshObj_Static_Inst")->Get_ObjectList();
			for (auto& iterMesh : plistMesh)
			{
				dynamic_cast<CMeshObj_Static*>(iterMesh)->Set_SplRender(true);
			}
			for (auto& iterMeshinst : plistMeshInst)
			{
				dynamic_cast<CMeshObj_Static_Inst*>(iterMeshinst)->Set_SplRender(true);
			}
			RELEASE_INSTANCE(CGameInstance);
		}
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
		pKyoujuro->Get_Renderer()->Set_PointBlur(860, 460, 100.f, 0.1f, 0.65f);			//	PointBlur

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
		m_fTime += fTimeDelta;
		if (2.f <= m_fTime)
		{
			if (false == m_bRenderObjects)
			{
				pGameInstance = GET_INSTANCE(CGameInstance);
				((CTerrain*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Terrain")->Get_LayerFront()))->Set_SplRender(false);
				plistMesh = pGameInstance->Find_Layer(g_iLevel, L"Layer_MeshObj_Static")->Get_ObjectList();
				plistMeshInst = pGameInstance->Find_Layer(g_iLevel, L"Layer_MeshObj_Static_Inst")->Get_ObjectList();
				for (auto& iterMesh : plistMesh)
				{
					dynamic_cast<CMeshObj_Static*>(iterMesh)->Set_SplRender(false);
				}
				for (auto& iterMeshinst : plistMeshInst)
				{
					dynamic_cast<CMeshObj_Static_Inst*>(iterMeshinst)->Set_SplRender(false);
				}
				RELEASE_INSTANCE(CGameInstance);

				m_bRenderObjects = true;
			}
		}


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

			if (pKyoujuro->Get_StorySplEnd())
			{
				CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

				pUIManager->Set_StroyEventEnd(true);

				RELEASE_INSTANCE(CUI_Manager);
				pKyoujuro->Set_Stop(true);
			}
			else
			{
				/*	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

					pEffectManger->Create_Effect(CEffect_Manager::EFF_FADE, this);

					RELEASE_INSTANCE(CEffect_Manager);*/
			}
			return new CKyoujuro_CinemaState(SCENE_END);
		}
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_END:
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_SPLSKL_END))
		{
			pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_SPLSKL_END);
			CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
			if (!pUIManager->Get_BattleTypeCheck())
			{
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_StoryScene(CCamera_Dynamic::STORYSCENE_ADV_AKAZA_DEAD);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QuestBattleCam(true);
				RELEASE_INSTANCE(CGameInstance);
			}
			RELEASE_INSTANCE(CUI_Manager);
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
	CUI_Manager* pUI_Manager = nullptr;

	if (m_eScene == CKyoujuro_CinemaState::SCENE_END)
	{
		pGameInstance = GET_INSTANCE(CGameInstance);

		((CTerrain*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Terrain")->Get_LayerFront()))->Set_SplRender(false);
		list<CGameObject*> plistMesh = pGameInstance->Find_Layer(g_iLevel, L"Layer_MeshObj_Static")->Get_ObjectList();
		list<CGameObject*> plistMeshInst = pGameInstance->Find_Layer(g_iLevel, L"Layer_MeshObj_Static_Inst")->Get_ObjectList();
		for (auto& iterMesh : plistMesh)
		{
			dynamic_cast<CMeshObj_Static*>(iterMesh)->Set_SplRender(false);
		}
		for (auto& iterMeshinst : plistMeshInst)
		{
			dynamic_cast<CMeshObj_Static_Inst*>(iterMeshinst)->Set_SplRender(false);
		}
		RELEASE_INSTANCE(CGameInstance);
	}
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

		RELEASE_INSTANCE(CGameInstance);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_SplSkr.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_SE_SplSkr.wav"), g_fEffect);

		pUI_Manager = GET_INSTANCE(CUI_Manager);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplCharNum(1);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplDialogStart(true);
		RELEASE_INSTANCE(CUI_Manager);
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_0:

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
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_1: {
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(false);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_1));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_1, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO3_GROUND1, pKyoujuro);

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_2: {
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_2));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_2, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO4_BG1, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO4_BG2, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO4_PLAYER, pKyoujuro->Get_WeaponWorld2());

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_3: {
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_3));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_3, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO5_BG1, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO5_FIRE1, pKyoujuro);
		//pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO4_PLAYER, pKyoujuro->Get_WeaponWorld2());

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_4: {
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(true);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_060);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_4);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_4);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_4));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_4);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_4, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO6_BG, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO6_HIT, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO6_HITBG, pKyoujuro);
		//pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO4_PLAYER, pKyoujuro->Get_WeaponWorld2());

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_5: {
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(true);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_070); // 75
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_5);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_5);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_5));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_5);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_5, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO7_BG1, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO7_FIRE1, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO7_FIRE2, pKyoujuro);

		RELEASE_INSTANCE(CEffect_Manager);

		pKyoujuro->Get_Renderer()->Set_Value(CRenderer::VALUE_OUTLINE, 20.f);
		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_6: {
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_080); // 78
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_6);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_6);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_6));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_6);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_6, 0.01f);

		//	Light
		pKyoujuro->Get_Renderer()->Set_Value(CRenderer::VALUE_ENVLIGHT, 0.8f);
		pKyoujuro->Get_Renderer()->Set_Value(CRenderer::VALUE_LIGHTPOWER, 0.4f);
		pKyoujuro->Add_Light(_float4(-1.f, 17.f, 78.f, 1.f), _float4(1.f, 0.2f, 0.f, 0.f), _float4(1.f, 1.f, 1.f, 1.f),
			20.f + XMVectorGetX(XMVector3Length(XMLoadFloat3(&_float3(-1.f, 17.f, 78.f)) - XMLoadFloat3(&_float3(82.f, 1.7f, 59.f)))));

		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_7: {	//	Final
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_090);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_7);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_7);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_7));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_7);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_7, 0.01f);

		pKyoujuro->Get_BattleTarget()->Set_SceneRender(false);

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		CEffect_AnimMesh::ANIM_MESH_INFO MeshInfo;
		MeshInfo.fLifeTime = 2.f;
		MeshInfo.fStartTime = 0.f;
		wcscpy_s(MeshInfo.m_szMeshName, TEXT("Prototype_Component_Model_Rengoku_Spl_078_FlameTrail01_11"));
		wcscpy_s(MeshInfo.szMeshDiffuse, TEXT("Prototype_Component_Texture_FireTest"));
		wcscpy_s(MeshInfo.szMeshDissolve, TEXT(""));
		wcscpy_s(MeshInfo.szMeshMask, TEXT(""));
		MeshInfo.vPosition = _float3(0.f, 1.f, 0.f);
		MeshInfo.vRotation = _float3(0.f, 0.f, 0.f);
		MeshInfo.pParents = pKyoujuro;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EffectAnimMesh"), g_iLevel, TEXT("Layer_Effect"), &MeshInfo);
		RELEASE_INSTANCE(CGameInstance);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		_vector vPos = XMVectorSet(-12.f, 0.f, 80.f, 1.f);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO9_BOOM, &vPos);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO9_GROUND, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO9_PROJ1, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO9_PORJ2, pKyoujuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO9_TONEIDO, &vPos);
		RELEASE_INSTANCE(CEffect_Manager);

		pKyoujuro->Get_Renderer()->Set_Value(CRenderer::VALUE_ENVLIGHT, 0.9f);
		pKyoujuro->Get_Renderer()->Set_Value(CRenderer::VALUE_LIGHTPOWER, 0.5f);
		pKyoujuro->Add_Light(_float4(-1.f, 17.f, 78.f, 1.f), _float4(1.f, 0.2f, 0.f, 0.f), _float4(1.f, 1.f, 1.f, 1.f),
			20.f + XMVectorGetX(XMVector3Length(XMLoadFloat3(&_float3(-1.f, 17.f, 78.f)) - XMLoadFloat3(&_float3(82.f, 1.7f, 59.f)))));
		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_8: {
		pKyoujuro->Set_SplWeapon(true);
		pKyoujuro->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_100);
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro_CinemaState::ANIM_SCENE_8);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro_CinemaState::ANIM_SCENE_8);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CKyoujuro_CinemaState::ANIM_SCENE_8));
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro_CinemaState::ANIM_SCENE_8);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro_CinemaState::ANIM_SCENE_8, 0.01f);
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(false);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_REN_MO10_FLASH, pKyoujuro->Get_WeaponWorld());
		RELEASE_INSTANCE(CEffect_Manager);

		pKyoujuro->Add_Light(_float4(-1.f, 17.f, 78.f, 1.f), _float4(1.f, 0.2f, 0.f, 0.f), _float4(1.f, 1.f, 1.f, 1.f),
			30.f + XMVectorGetX(XMVector3Length(XMLoadFloat3(&_float3(-1.f, 17.f, 78.f)) - XMLoadFloat3(&_float3(82.f, 1.7f, 59.f)))));
		pKyoujuro->Get_Renderer()->Set_Value(CRenderer::VALUE_ENVLIGHT, 1.1f);
		pKyoujuro->Get_Renderer()->Set_Value(CRenderer::VALUE_LIGHTPOWER, 0.6f);
		break;
	}
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_END:
		pKyoujuro->Get_BattleTarget()->Set_Hp(-400);
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
	switch (m_eScene)
	{
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_5:
		pKyoujuro->Get_Renderer()->ReturnValue();
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_7:
		pKyoujuro->Delete_MyLights();
		pKyoujuro->Get_Renderer()->ReturnValue();
		break;
	case Client::Kyoujuro::CKyoujuro_CinemaState::SCENE_8:
		pKyoujuro->Get_BattleTarget()->Set_SceneRender(true);
		pKyoujuro->Delete_MyLights();
		pKyoujuro->Get_Renderer()->ReturnValue();
		break;
	default:
		break;
	}
}

