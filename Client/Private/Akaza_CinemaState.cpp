#include "stdafx.h"
#include "..\Public\Akaza_CinemaState.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Terrain.h"
#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"
#include "Effect_Manager.h"
#include "BattleDialog.h"

using namespace Akaza;

CAkaza_CinemaState::CAkaza_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CAkazaState * CAkaza_CinemaState::HandleInput(CAkaza * pAkaza)
{
	CAkazaState* pState = nullptr;


	return pState;
}

CAkazaState * CAkaza_CinemaState::Tick(CAkaza * pAkaza, _float fTimeDelta)
{
	CGameInstance* pGameInstance = nullptr;
	list<CGameObject*> plistMesh;
	list<CGameObject*> plistMeshInst;
	_matrix matColl;
	_float3 vPos;
	_float	fValue;
	switch (m_eScene)
	{
	case Client::Akaza::CAkaza_CinemaState::SCENE_START:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_START))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_START);
			return new CAkaza_CinemaState(SCENE_0);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_0:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_0))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_0);
			return new CAkaza_CinemaState(SCENE_1);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_1:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_1))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_1);
			return new CAkaza_CinemaState(SCENE_2);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_2:
		m_fTime += fTimeDelta;
		if (1.3f < m_fTime)
		{
			pAkaza->Set_SceneRender(false);
		}
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_2))
		{
			pAkaza->Set_SceneRender(true);
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_2);
			return new CAkaza_CinemaState(SCENE_3);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_3:
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_3))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_3);
			return new CAkaza_CinemaState(SCENE_4);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_4:
		if (4.f < pAkaza->Get_Model()->Get_Duration())
		{
			fValue = 0.6f + fabs(sin(_float(rand() % 314) / 100.f) * 0.3f);
			pAkaza->Get_Renderer()->Set_PointBlur(640, 200, 120.f, 0.3f, fValue);

			if (false == m_bRenderObjects)
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

				m_bRenderObjects = true;
			}
		}
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_4))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_4);
			return new CAkaza_CinemaState(SCENE_5);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_5:
		matColl = pAkaza->Get_Model()->Get_BonePtr("C_Spine_3")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&pAkaza->Get_Model()->Get_PivotFloat4x4()) * XMLoadFloat4x4(pAkaza->Get_Transform()->Get_World4x4Ptr());
		XMStoreFloat3(&vPos, matColl.r[3]);
		pAkaza->Get_Renderer()->Set_PointBlur(vPos, 130.f, 0.2f, 0.5f);
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_5))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_5);
			return new CAkaza_CinemaState(SCENE_6);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_6:
		m_fTime += fTimeDelta;

		fValue = 0.6f + fabs(sin(m_fTime * 10.f) * 0.3f);
		pAkaza->Get_Renderer()->Set_PointBlur(640, 360, 120.f, 0.3f, fValue);			//	PointBlur		

		matColl = pAkaza->Get_Model()->Get_BonePtr("C_Spine_3")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&pAkaza->Get_Model()->Get_PivotFloat4x4()) * XMLoadFloat4x4(pAkaza->Get_Transform()->Get_World4x4Ptr());
		XMStoreFloat3(&vPos, matColl.r[3]);
		pAkaza->Get_Renderer()->Set_PointBlur(vPos, 130.f, 0.1f, 0.8f);
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_6))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_6);
			return new CAkaza_CinemaState(SCENE_7);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_7:
		m_fTime += fTimeDelta;
		if (1.f <= m_fTime)
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
		}

		matColl = pAkaza->Get_Model()->Get_BonePtr("C_Spine_3")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&pAkaza->Get_Model()->Get_PivotFloat4x4()) * XMLoadFloat4x4(pAkaza->Get_Transform()->Get_World4x4Ptr());
		XMStoreFloat3(&vPos, matColl.r[3]);
		pAkaza->Get_Renderer()->Set_PointBlur(vPos, 130.f, 0.1f, 0.6f);
		if (pAkaza->Get_Model()->Get_End(CAkaza_CinemaState::ANIM_SCENE_7))
		{
			pAkaza->Get_Model()->Set_End(CAkaza_CinemaState::ANIM_SCENE_7);
	/*		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_FADE, this);

			RELEASE_INSTANCE(CEffect_Manager);*/
			return new CAkaza_CinemaState(SCENE_8);
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_8:
		if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_SPLSKL_END))
		{
			pAkaza->Get_Model()->Set_End(CAkaza::ANIM_SPLSKL_END);
			return new CIdleState();
		}
		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_END:
		break;
	default:
		break;
	}
	

	return nullptr;
}

CAkazaState * CAkaza_CinemaState::Late_Tick(CAkaza * pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CAkaza_CinemaState::Enter(CAkaza * pAkaza)
{
	CGameInstance* pGameInstance = nullptr;

	list<CGameObject*> plistMesh;
	list<CGameObject*> plistMeshInst;
	CUI_Manager* pUI_Manager = nullptr;
	switch (m_eScene)
	{
	case Client::Akaza::CAkaza_CinemaState::SCENE_START:
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pAkaza->Get_NavigationHeight().y, 64.f, 1.f));
		pAkaza->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.2f, pAkaza->Get_NavigationHeight().y, 64.f, 1.f));
		pAkaza->Get_Transform()->Set_PlayerLookAt(XMVectorSet(52.f, pAkaza->Get_NavigationHeight().y, 64.f, 1.f));
		pAkaza->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(52.f, pAkaza->Get_NavigationHeight().y, 64.f, 1.f));
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_START);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_START);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_START));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_START);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_START, 0.01f);
		pAkaza->Set_SplSkl(true);
		pGameInstance = GET_INSTANCE(CGameInstance);
		((CCamera_Dynamic*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Camera")->Get_LayerFront()))->Start_CutScene(true, CCamera_Dynamic::CUTSCENE_AKZ_START);
		RELEASE_INSTANCE(CGameInstance);

		pUI_Manager = GET_INSTANCE(CUI_Manager);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplCharNum(3);
		dynamic_cast<CBattleDialog*>(pUI_Manager->Get_DialogUI())->Set_SplDialogStart(true);
		RELEASE_INSTANCE(CUI_Manager);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_SplSkr.wav"), g_fVoice);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_SplSkr.wav"), g_fEffect);

		break;
	case Client::Akaza::CAkaza_CinemaState::SCENE_0: {
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_0);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_0);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_0));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_0, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO2_GROUND, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);

		pGameInstance = GET_INSTANCE(CGameInstance);
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

		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_1: {
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_030);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_1);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_1);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_1));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_1, 0.01f);

		pAkaza->Get_BattleTarget()->Set_SceneRender(false);
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

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO3_GROUND, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_2: {
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_2);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_2);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_2));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_2, 0.01f);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO4_DIST, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);

		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_3: {
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_3);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_3);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_3));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_3);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_3, 0.01f);

		pAkaza->Get_BattleTarget()->Set_SceneRender(true);
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

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO5_DASH1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO5_HAND1, pAkaza->Get_WeaponWorld2());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO5_HIT1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO5_HIT2, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO5_KICK1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO5_RUN1, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_4: {
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_060);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_4);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_4);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_4));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_4);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_4, 0.01f);

		pAkaza->Get_BattleTarget()->Set_SceneRender(false);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO6_PROJ1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO6_SLASH1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO6_SLASH2, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_5: {
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_070);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_5);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_5);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_5));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_5);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_5, 0.01f);

		pAkaza->Get_BattleTarget()->Set_SceneRender(false);
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

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO7_BACKLIGHT, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO7_HAND, pAkaza->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO7_HAND2, pAkaza->Get_WeaponWorld());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO7_HAND3, pAkaza->Get_WeaponWorld2());
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO7_BG1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO7_BG2, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO7_PROJ1, pAkaza);
		RELEASE_INSTANCE(CEffect_Manager);

		pAkaza->Get_Renderer()->Set_Value(CRenderer::VALUE_DISTORTION, 100.f);
		

		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_6: {
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_080);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_6);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_6);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_6));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_6);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_6, 0.01f);

		pAkaza->Get_BattleTarget()->Set_SceneRender(true);
		pGameInstance = GET_INSTANCE(CGameInstance);
		((CTerrain*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Terrain")->Get_LayerFront()))->Set_SplRender(true);
		RELEASE_INSTANCE(CGameInstance);

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO8_BG1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO8_HAND1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO8_PROJ1, pAkaza);
		RELEASE_INSTANCE(CEffect_Manager);

		pAkaza->Get_Renderer()->Set_Value(CRenderer::VALUE_ENVLIGHT, 1.f);
		pAkaza->Get_Renderer()->Set_Value(CRenderer::VALUE_LIGHTPOWER, 0.25f);
		pAkaza->Add_Light(_float4(50.f, 2.2f, 66.6f, 1.f), _float4(0.1f, 0.5f, 1.f, 1.f), _float4(1.f, 1.f, 1.f, 1.f), 3.f);
		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_7: {
		pAkaza->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_090);
		pAkaza->Get_Model()->Reset_Anim(CAkaza_CinemaState::ANIM_SCENE_7);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza_CinemaState::ANIM_SCENE_7);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza_CinemaState::ANIM_SCENE_7));
		pAkaza->Get_Model()->Set_Loop(CAkaza_CinemaState::ANIM_SCENE_7);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza_CinemaState::ANIM_SCENE_7, 0.01f);

		pAkaza->Get_BattleTarget()->Set_SceneRender(true);
		pGameInstance = GET_INSTANCE(CGameInstance);
		((CTerrain*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Terrain")->Get_LayerFront()))->Set_SplRender(true);
		RELEASE_INSTANCE(CGameInstance);


		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO9_AURA1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO9_BG1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO9_PROJ1, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SPL_AKA_MO9_BOOM1, pAkaza);
		RELEASE_INSTANCE(CEffect_Manager);

		pAkaza->Get_Renderer()->Set_Value(CRenderer::VALUE_ENVLIGHT, 1.f);
		pAkaza->Get_Renderer()->Set_Value(CRenderer::VALUE_LIGHTPOWER, 0.25f);
		pAkaza->Add_EffectLight(LIGHTDESC::TYPE_EFF1, _float4(49.8f, 2.2f, 63.f, 1.f), _float4(0.1f, 0.5f, 1.f, 1.f), _float4(1.f, 1.f, 1.f, 1.f), 50.f, 1.3f);
		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_8: {
		pAkaza->Get_BattleTarget()->Set_Hp(-400);
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.5183f, pAkaza->Get_NavigationHeight().y, 56.1f, 1.f));
		pAkaza->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(56.56f, pAkaza->Get_NavigationHeight().y, 50.03f, 1.f));

		pAkaza->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_KNOCKBACK, 0.f, 0.f, 8.f);
		pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_SPLSKL_END);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SPLSKL_END);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CAkaza::ANIM_SPLSKL_END));
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_SPLSKL_END);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SPLSKL_END, 0.01f);

		break;
	}
	case Client::Akaza::CAkaza_CinemaState::SCENE_END:
		break;
	default:
		break;
	}
}

void CAkaza_CinemaState::Exit(CAkaza * pAkaza)
{
	CGameInstance* pGameInstance = nullptr;
	list<CGameObject*> plistMesh;
	list<CGameObject*> plistMeshInst;
	switch (m_eScene)
	{
	case Client::Akaza::CAkaza_CinemaState::SCENE_6:
		pAkaza->Delete_MyLights();
		pAkaza->Get_Renderer()->ReturnValue();
		break;

	case Client::Akaza::CAkaza_CinemaState::SCENE_7:
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

		pAkaza->Delete_MyLights();
		pAkaza->Get_Renderer()->ReturnValue();
		break;

	case Client::Akaza::CAkaza_CinemaState::SCENE_8:
		pAkaza->Get_Renderer()->ReturnValue();
		break;

	default:
		break;
	}
}
