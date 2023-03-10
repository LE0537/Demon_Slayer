#include "stdafx.h"
#include "..\Public\ItemBox.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "Data_Manager.h"
#include "SoundMgr.h"
#include "Tanjiro.h"
#include "ImGuiManager.h"
#include "RuiBomb.h"
#include "SoundMgr.h"
#include "MGameHeart.h"

CItemBox::CItemBox(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CItemBox::CItemBox(const CItemBox & rhs)
	: CCharacters(rhs)
{
}

HRESULT CItemBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemBox::Initialize(void * pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;




	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-307.222f, 44.609f, -122.979f, 1.f));
	m_pTransformCom->Set_Scale(XMVectorSet(5.f, 5.f, 5.f, 0.f));

	


	m_pModelCom->Set_FrameNum(1, 199);
	m_pModelCom->Set_UsingFrame(1, 0, 60);
	m_pModelCom->Set_Loop(1);


	m_pModelCom->Set_CurrentAnimIndex(0);

	m_pModelCom->Set_CurrentTime_Index(0, 181.f);

	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);


	CRuiBomb::BOMBDESC tInfo{};
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-286.408f, 45.681f, -170.159f, 1.f); // 1
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-279.755f, 45.498f, -162.657f, 1.f); // 2 
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-278.285f, 45.430f, -176.928f, 1.f); // 3
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-272.327f, 45.217f, -169.341f, 1.f); // 4
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-270.036f, 45.181f, -183.980f, 1.f); // 5
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-265.082f, 44.495f, -176.133f, 1.f); // 6
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-262.401f, 44.776, -188.255f, 1.f); // 7
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-257.669f, 43.808f, -180.832f, 1.f); // 8
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-249.994f, 43.257f, -183.497f, 1.f); // 9
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-254.315f, 44.088f, -190.543f, 1.f); // 10;
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-241.398f, 42.958f, -186.916f, 1.f); // 11
	m_vBombPosition.push_back(tInfo.vPosition);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = XMVectorSet(-246.529f, 43.394f, -193.548f, 1.f); // 12
	m_vBombPosition.push_back(tInfo.vPosition);

	return S_OK;
}

void CItemBox::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	//if (pGameInstance->Key_Down(DIK_G))
	//{
	//	for (_uint i = BOMB_1; i < BOMB_END; ++i)
	//	{
	//		m_bBomb[i] = false;
	//	}

	//	for (_uint i = BOMB_1; i < BOMB_END; ++i)
	//	{
	//		m_fBombTime[i] = false;
	//	}

	//	for (_uint i = BOMB_1; i < BOMB_END; ++i)
	//	{
	//		m_bCreate[i] = false;
	//	}

	//
	//	m_bTopView = !m_bTopView;

	//	dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, L"Layer_Camera")->Get_LayerFront())->Set_TopView(m_bTopView);

	//	m_bBomb[BOMB_1] = !m_bBomb[BOMB_1];



	//}

	Play_Bomb(fTimeDelta);


	if (m_bPlayAnim)
		m_fMsgTime += fTimeDelta;
}

void CItemBox::Late_Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vTargetPos = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, L"Layer_Tanjiro")->Get_LayerFront())->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

	if (m_bOpen == true)
	{
		_vector vDestPos = XMVectorSet(-227.567f, 42.087f, -212.677f, 1.f);
		_float fDistance = XMVectorGetX(XMVector3Length(vDestPos - vTargetPos));

		if (fDistance <= 7.f)
		{
			m_bTopView = false;
			dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, L"Layer_Camera")->Get_LayerFront())->Set_TopView(m_bTopView);
			for (_uint i = BOMB_1; i < BOMB_END; ++i)
			{
				m_bBomb[i] = false;
			}
		}
	}

	// XMVectorSet(-289.935f, 45.183f, -152.779f, 1.f)

	if (m_bPlayAnim == false)
	{
		m_pModelCom->Reset_Anim(0);
		m_pModelCom->Set_CurrentTime_Index(0, 181.f);
	}


	if (m_bPlayAnim == true)
	{
		if (m_pModelCom->Get_CurrentTime_Index(1) >= 50.f)
		{
			m_bOpen = true;

			if (m_bSound2 == false)
			{
				CSoundMgr::Get_Instance()->PlayEffect(TEXT("RuiBombStart.wav"), 0.4f);
				m_bSound2 = true;
			}
		}

		if (m_pModelCom->Get_CurrentTime_Index(1) >= 10.f && m_bSound == false)
		{
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("CMN_BoxL_Open.wav"), 0.4f);
			m_bSound = true;
		}
	}


	if (fDist < 8.f && !m_bMsgStart)
	{
		EventCheck();
		
	}

	/*if (fDist < 8.f)
	{
		CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
		pUIManager->Set_InteractionOn();
		RELEASE_INSTANCE(CUI_Manager);
	}
	else
	{
		CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
		pUIManager->Set_InteractionOff();
		RELEASE_INSTANCE(CUI_Manager);
	}*/

	if (fDist < 8.f && !m_bMsgEnd && m_fMsgTime >= 2.f)
	{
		EventCheck2();
	}

	if (fDist < 8.f && pGameInstance->Key_Down(DIK_F))
	{
		m_bPlayAnim = true;
		m_pModelCom->Set_CurrentAnimIndex(1);

		for (_uint i = BOMB_1; i < BOMB_END; ++i)
		{
			m_bBomb[i] = false;
		}

		for (_uint i = BOMB_1; i < BOMB_END; ++i)
		{
			m_fBombTime[i] = false;
		}

		for (_uint i = BOMB_1; i < BOMB_END; ++i)
		{
			m_bCreate[i] = false;
		}
			
		m_bTopView = true;

		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, L"Layer_Camera")->Get_LayerFront())->Set_TopView(m_bTopView);

		m_bBomb[BOMB_1] = true;

	}


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	if (m_bOpen == false)
		m_pModelCom->Play_Animation(fTimeDelta);


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CItemBox::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 8)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;



}

HRESULT CItemBox::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	_vector vLightEye, vLightAt, vLightUp;
	_matrix matLightView;
	if (g_iLevel == LEVEL_GAMEPLAY)
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	else
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}
void CItemBox::Check_Event(_float fTimeDelta)
{

}

void CItemBox::Play_Bomb(_float fTimeDelta)
{
	if (m_bBomb[BOMB_1])
	{
		m_fBombTime[BOMB_1] += fTimeDelta;

		if (m_bCreate[BOMB_1] == false)
		{
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), 0.5f);
			m_bCreate[BOMB_1] = true;
			Pattern_Bomb1(fTimeDelta);
		}


		if (m_fBombTime[BOMB_1] >= 0.85f)
		{
			m_bBomb[BOMB_2] = true;

			m_bBomb[BOMB_1] = false;
			m_bCreate[BOMB_1] = false;
			m_fBombTime[BOMB_1] = 0.f;
		}

	}

	else if (m_bBomb[BOMB_2])
	{
		m_fBombTime[BOMB_2] += fTimeDelta;

		if (m_bCreate[BOMB_2] == false)
		{
			//CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), g_fEffect);
			m_bCreate[BOMB_2] = true;
			Pattern_Bomb2(fTimeDelta);
		}


		if (m_fBombTime[BOMB_2] >= 0.85f)
		{
			m_bBomb[BOMB_3] = true;

			m_bBomb[BOMB_2] = false;
			m_bCreate[BOMB_2] = false;
			m_fBombTime[BOMB_2] = 0.f;
		}
	}


	else if (m_bBomb[BOMB_3])
	{
		m_fBombTime[BOMB_3] += fTimeDelta;

		if (m_bCreate[BOMB_3] == false)
		{
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), 0.5f);
			m_bCreate[BOMB_3] = true;
			Pattern_Bomb3(fTimeDelta);
		}


		if (m_fBombTime[BOMB_3] >= 0.85f)
		{
			m_bBomb[BOMB_4] = true;

			m_bBomb[BOMB_3] = false;
			m_bCreate[BOMB_3] = false;
			m_fBombTime[BOMB_3] = 0.f;
		}
	}




	else if (m_bBomb[BOMB_4])
	{
		m_fBombTime[BOMB_4] += fTimeDelta;

		if (m_bCreate[BOMB_4] == false)
		{
			//CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), 0.5f);
			m_bCreate[BOMB_4] = true;
			Pattern_Bomb4(fTimeDelta);
		}


		if (m_fBombTime[BOMB_4] >= 0.85f)
		{
			m_bBomb[BOMB_5] = true;

			m_bBomb[BOMB_4] = false;
			m_bCreate[BOMB_4] = false;
			m_fBombTime[BOMB_4] = 0.f;
		}
	}


	else if (m_bBomb[BOMB_5])
	{
		m_fBombTime[BOMB_5] += fTimeDelta;

		if (m_bCreate[BOMB_5] == false)
		{
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), 0.5f);
			m_bCreate[BOMB_5] = true;
			Pattern_Bomb5(fTimeDelta);
		}


		if (m_fBombTime[BOMB_5] >= 0.85f)
		{
			m_bBomb[BOMB_6] = true;

			m_bBomb[BOMB_5] = false;
			m_bCreate[BOMB_5] = false;
			m_fBombTime[BOMB_5] = 0.f;
		}
	}



	else if (m_bBomb[BOMB_6])
	{
		m_fBombTime[BOMB_6] += fTimeDelta;

		if (m_bCreate[BOMB_6] == false)
		{
			//CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), 0.5f);
			m_bCreate[BOMB_6] = true;
			Pattern_Bomb6(fTimeDelta);
		}


		if (m_fBombTime[BOMB_6] >= 0.85f)
		{
			m_bBomb[BOMB_7] = true;

			m_bBomb[BOMB_6] = false;
			m_bCreate[BOMB_6] = false;
			m_fBombTime[BOMB_6] = 0.f;
		}
	}



	else if (m_bBomb[BOMB_7])
	{
		m_fBombTime[BOMB_7] += fTimeDelta;

		if (m_bCreate[BOMB_7] == false)
		{
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), 0.5f);
			m_bCreate[BOMB_7] = true;
			Pattern_Bomb7(fTimeDelta);
		}


		if (m_fBombTime[BOMB_7] >= 0.85f)
		{
			m_bBomb[BOMB_8] = true;

			m_bBomb[BOMB_7] = false;
			m_bCreate[BOMB_7] = false;
			m_fBombTime[BOMB_7] = 0.f;
		}
	}


	else if (m_bBomb[BOMB_8])
	{
		m_fBombTime[BOMB_8] += fTimeDelta;

		if (m_bCreate[BOMB_8] == false)
		{
			//CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), 0.5f);
			m_bCreate[BOMB_8] = true;
			Pattern_Bomb8(fTimeDelta);
		}


		if (m_fBombTime[BOMB_8] >= 0.85f)
		{
			m_bBomb[BOMB_9] = true;

			m_bBomb[BOMB_8] = false;
			m_bCreate[BOMB_8] = false;
			m_fBombTime[BOMB_8] = 0.f;
		}
	}


	else if (m_bBomb[BOMB_9])
	{
		m_fBombTime[BOMB_9] += fTimeDelta;

		if (m_bCreate[BOMB_9] == false)
		{
			//CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_SE_SpMoveSkill.wav"), 0.5f);
			m_bCreate[BOMB_9] = true;
			Pattern_Bomb9(fTimeDelta);
		}


		if (m_fBombTime[BOMB_9] >= 1.f)
		{
			m_bBomb[BOMB_1] = true;

			m_bBomb[BOMB_9] = false;
			m_bCreate[BOMB_9] = false;
			m_fBombTime[BOMB_9] = 0.f;
		}
	}


	


}

void CItemBox::Pattern_Bomb1(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[0];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[1];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[2];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[3];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);


	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[4];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);


	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[5];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);
}

void CItemBox::Pattern_Bomb2(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[6];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[7];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[8];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[9];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);


	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[10];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);


	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[11];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);
	


}

void CItemBox::Pattern_Bomb3(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[0];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[1];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[2];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[3];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);



}

void CItemBox::Pattern_Bomb4(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[6];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[7];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[8];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[9];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

}

void CItemBox::Pattern_Bomb5(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[2];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[3];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);

}

void CItemBox::Pattern_Bomb6(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[4];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);


	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[5];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);
}

void CItemBox::Pattern_Bomb7(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[6];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);


	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[7];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);
}

void CItemBox::Pattern_Bomb8(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[8];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);


	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[9];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);
}

void CItemBox::Pattern_Bomb9(_float fTimeDelta)
{
	CRuiBomb::BOMBDESC tInfo{};

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[10];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);


	ZeroMemory(&tInfo, sizeof(tInfo));
	tInfo.vPosition = m_vBombPosition[11];
	tInfo.pTarget = dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiBomb"), LEVEL_ADVRUI, TEXT("Layer_RuiBomb"), &tInfo);
}

void CItemBox::EventCheck()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		pUI_Manager->Set_MsgOn();
		pUI_Manager->Set_MsgName(TEXT("카마도 탄지로"));
		pUI_Manager->Set_Msg(TEXT("뭐지 이 괴상한 물체는?"));
		break;
	case 1:
		pUI_Manager->Set_Msg(TEXT("다른 세계에서 온 상자같아.. 한 번 열어보자.."));
		break;
	default:
		pUI_Manager->Reset_MsgCount();
		pUI_Manager->Set_MsgOff();
		m_bMsgStart = true;
		break;
	}
	

	RELEASE_INSTANCE(CUI_Manager);
}

void CItemBox::EventCheck2()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	switch (pUI_Manager->Get_MsgCount())
	{
	case 0:
		pUI_Manager->Set_MsgOn();
		pUI_Manager->Set_MsgName(TEXT("카마도 탄지로"));
		pUI_Manager->Set_Msg(TEXT("뭐지? 상자 안에는 아무것도 없다..."));
		break;
	case 1:
		pUI_Manager->Set_Msg(TEXT("혈귀가 파놓은 함정이다!! 큭.."));
		break;
	default:
		pUI_Manager->Reset_MsgCount();
		pUI_Manager->Set_MsgOff();
		for (_uint i = 0; i < 3; ++i)
		{
			dynamic_cast<CMGameHeart*>(pUI_Manager->Get_HeartUI(i))->Set_RenderOn(true);
		}
		pUI_Manager->Set_ItemBoxCheck(true);
		m_bMsgEnd = true;
		break;
	}


	RELEASE_INSTANCE(CUI_Manager);
}


HRESULT CItemBox::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CItemBox::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 15.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_ItemBox"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

void CItemBox::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CItemBox::Get_GuardHit(_int eType)
{
}

void CItemBox::Player_TakeDown(_float _fPow, _bool _bJump)
{
}

void CItemBox::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
}

void CItemBox::Play_Scene()
{
}

CItemBox * CItemBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CItemBox*	pInstance = new CItemBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CGoto"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItemBox::Clone(void * pArg)
{
	CGameObject*	pInstance = new CItemBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CGoto"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemBox::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);

}
