#include "stdafx.h"
#include "ImGuiManager.h"
#include "GameInstance.h"

#include "Kyoujuro.h"
#include "Tanjiro.h"
#include "Akaza.h"
#include "Rui.h"
#include "Nezuko.h"
#include "Shinobu.h"
#include "Camera_Dynamic.h"
#include "CamLine.h"
#include "Layer.h"
#include "Enmu.h"

#include "Enmu_Chaos_Head.h"
#include "Enmu_Left_Hand.h"
#include "Enmu_Right_Hand.h"
#include "Enmu_Chok.h"
#include "Enmu_Shield.h"

#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"
#include "Terrain.h"
#include "EnmuBoss.h"

IMPLEMENT_SINGLETON(CImGuiManager)

CImGuiManager::CImGuiManager()
{
}

HRESULT CImGuiManager::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);



	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	m_bImguiEnable = false;

	CComponent* pOut = pGameInstance->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Renderer");
	m_pRendererCom = (CRenderer*)pOut;

	if (nullptr == m_pRendererCom)
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);



	for (_uint i = 0; i < 250; ++i)
	{
		m_strCamFiles[i] = new char[MAX_PATH];
	}
	Read_CamActionFiles();

	return S_OK;
}

void CImGuiManager::Tick(_float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Key_Pressing(DIK_LCONTROL))
		if (pGameInstance->Key_Down(DIK_1))
			m_bImguiEnable = !m_bImguiEnable;


	if (m_bImguiEnable/* && true == g_bDebug*/)
	{
		ShowGui(fTimeDelta);
	}

	RELEASE_INSTANCE(CGameInstance);
	ImGui::EndFrame();
}

void CImGuiManager::ShowGui(_float fTimeDelta)
{
	ImGui::Begin("Close, Open = Ctrl + 1");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	if (m_iLevel != g_iLevel)
	{
		LEVEL eLevel = LEVEL_STATIC;
		_bool	bCheck = true;
		if (g_iLevel == LEVEL_LOADING || g_iLevel == LEVEL_LOGO || g_iLevel == LEVEL_SELECTCHAR || g_iLevel == LEVEL_GAMERESULT
			|| g_iLevel == LEVEL_MENU || g_iLevel == LEVEL_STORYMENU || g_iLevel == LEVEL_SELECTMAP)
		{
			bCheck = false;
		}
		if (true == bCheck)
		{
			CGameObject* pGameObject = pGameInstance->Find_Layer(g_iLevel, L"Layer_Camera")->Get_LayerFront();
			if (nullptr != pGameObject)
			{
				Safe_Release(m_pCamera);
				m_pCamera = pGameObject;
				Safe_AddRef(m_pCamera);
			}
		}

		m_iLevel = g_iLevel;
	}

	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Post Processing"))
		{
			PostProcessing(fTimeDelta);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Animation"))
		{
			AnimationDebug(fTimeDelta);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Cam_Action"))
		{
			if (nullptr != m_pCamera)
			{
				Camera_Action(fTimeDelta);
			}

			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();
	}


	ImGui::End();

	RELEASE_INSTANCE(CGameInstance);
}

void CImGuiManager::Render()
{
	if (m_bImguiEnable)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void CImGuiManager::PostProcessing(_float fTimeDelta)
{
	ImGuiIO& io = ImGui::GetIO();
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	static char*	AOButton = ("AO Button");
	static _bool	bAO_OnOff = true;
	if (ImGui::Button(AOButton, ImVec2(ImGui::GetWindowWidth() * 0.35f, 25.f)))
		bAO_OnOff = !bAO_OnOff;
	if (nullptr != m_pRendererCom)
		m_pRendererCom->AO_OnOff(bAO_OnOff);

	static _bool bInputLightName = false;
	static char strLightName[30][6] = { "" };
	if (false == bInputLightName)
	{
		strcpy_s(strLightName[0], "None");

		for (_uint i = 1; i < 30; ++i)
		{
			char cTemp[6] = "";
			_itoa_s(i - 1, cTemp, sizeof(cTemp), 10);
			strcpy_s(strLightName[i], sizeof(cTemp), cTemp);
		}

		bInputLightName = true;
	}

	//	20
	//	m_fPostProcessingValue = { 0.15f, 0.15f, 0.4f, 40.f, 450.f, 0.3f, 0.5f, 1.36f, 0.4f, 1.f, 20.f, 300.f, 0.05f, 1.79f, 0.2f, 0.85f, 1.f, 0.3f, 15.f, 1.f, 1.f };
	static float vFogColor[3] = { 0.15f, 0.15f, 0.4f };

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat3("Fog Color", &m_fPostProcessingValue[CRenderer::VALUE_FOGCOLOR_R], 0.001f, 0.000f, 1.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Fog Distance", &m_fPostProcessingValue[CRenderer::VALUE_FOGDISTANCE], 1.f, 0.f, 500.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Fog Range", &m_fPostProcessingValue[CRenderer::VALUE_FOGRANGE], 1.f, 1.f, 2000.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Fog Min Power", &m_fPostProcessingValue[CRenderer::VALUE_FOGMINPOWER], 0.001f, 0.f, 1.f, "%.3f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Fog Cubemap Power", &m_fPostProcessingValue[CRenderer::VALUE_CUBEMAPFOG], 0.001f, 0.f, 1.f, "%.3f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("AO Value", &m_fPostProcessingValue[CRenderer::VALUE_AO], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("AO Radius", &m_fPostProcessingValue[CRenderer::VALUE_AORADIUS], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Glow Blurring Count", &m_fPostProcessingValue[CRenderer::VALUE_GLOWBLURCOUNT], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Distortion", &m_fPostProcessingValue[CRenderer::VALUE_DISTORTION], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("OutLine", &m_fPostProcessingValue[CRenderer::VALUE_OUTLINE], 0.02f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("InnerLine", &m_fPostProcessingValue[CRenderer::VALUE_INNERLINE], 0.001f, 0.f);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Env Light", &m_fPostProcessingValue[CRenderer::VALUE_ENVLIGHT], 0.001f, -3.f, 10.f, "%.3f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("LightShaft", &m_fPostProcessingValue[CRenderer::VALUE_LIGHTSHAFT], 0.001f, -3.f, 10.f, "%.3f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("LightShaft_TestLength", &m_fPostProcessingValue[CRenderer::VALUE_LIGHTSHAFT_TESTLENGTH], 0.02f, 0.01f, 10.f, "%.2f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("LightShaft_Minus", &m_fPostProcessingValue[CRenderer::VALUE_LIGHTSHAFT_MINUS], 1.f, 1.f, 20.f, "%.1f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("LightPower", &m_fPostProcessingValue[CRenderer::VALUE_LIGHTPOWER], 0.001f, -3.f, 10.f, "%.3f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Shadow Test Length", &m_fPostProcessingValue[CRenderer::VALUE_SHADOWTESTLENGTH], 0.001f, -3.f, 3.f, "%.3f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Player Shadow Length", &m_fPostProcessingValue[CRenderer::VALUE_PLC_SHADOW], 0.00001f, 0.00001f, 1.f, "%.5f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("MapGrayScaleMaxTime", &m_fPostProcessingValue[CRenderer::VALUE_MAPGRAYSCALETIME], 0.1f, 0.f, 100.f, "%.1f");

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
	ImGui::DragFloat("Shadow power", &m_fPostProcessingValue[CRenderer::VALUE_SHADOWPOWER], 0.001f, 0.f, 1.f, "%.3f");

	if (0.f > m_fPostProcessingValue[CRenderer::VALUE_MAPGRAYSCALETIME])
		m_fPostProcessingValue[CRenderer::VALUE_MAPGRAYSCALETIME] = 0.f;

	if (nullptr != m_pRendererCom)
	{
		for (_int i = 0; i < CRenderer::VALUE_END; ++i)
			m_pRendererCom->Set_Value(CRenderer::VALUETYPE(i), m_fPostProcessingValue[i]);
	}



	ImGui::Separator();



	static _float3	vDirLightDir = _float3(0.f, 0.f, 0.f);
	static _float3	vDirLightColor = _float3(1.f, 1.f, 1.f);
	static _float3	vDirLightPower = _float3(0.4f, 0.4f, 0.4f);
	static _float3	vDirLightSpecular = _float3(1.f, 1.f, 1.f);

	static _int		iLightIndex = 0;
	static _int		iPreLightIndex = 0;
	static _float3	vLightPosition = _float3(0.f, 0.f, 0.f);
	static _float	fLightRange = 1.f;
	static _float3	vLightColor = _float3(0.f, 0.f, 0.f);
	static _float3	vLightPower = _float3(0.f, 0.f, 0.f);
	static _float3	vLightSpecular = _float3(0.f, 0.f, 0.f);

	static _float4 vPlayerShadowLightPos = _float4(0.f, 0.f, 0.f, 0.f);
	static _float4 vPlayerShadowLighAt = _float4(0.f, 0.f, 0.f, 0.f);
	static _float4 vPlayerFol_ShadowLightPos = _float4(0.f, 0.f, 0.f, 0.f);
	static _float4 vPlayerFol_ShadowLighAt = _float4(0.f, 0.f, 0.f, 0.f);
	static _float4 vObjectShadowLightPos = _float4(0.f, 0.f, 0.f, 0.f);
	static _float4 vObjectShadowLighAt = _float4(0.f, 0.f, 0.f, 0.f);

	//	Level Change
	static _int		iPreLevel = LEVEL_STATIC;
	if (g_iLevel != iPreLevel)
	{
		const LIGHTDESC* pDirectionalLightDesc = pGameInstance->Get_LightDesc(_int(LIGHTDESC::TYPE_DIRECTIONAL));
		if (nullptr != pDirectionalLightDesc)
		{
			vDirLightDir = *(_float3*)&pDirectionalLightDesc->vDirection;
			vDirLightColor = *(_float3*)&pDirectionalLightDesc->vDiffuse;
			vDirLightPower = *(_float3*)&pDirectionalLightDesc->vAmbient;
			vDirLightSpecular = *(_float3*)&pDirectionalLightDesc->vSpecular;
		}
		const LIGHTDESC* pPlayerShadowDesc = pGameInstance->Get_ShadowLightDesc(_int(LIGHTDESC::TYPE_RUISHADOW));
		if (nullptr != pPlayerShadowDesc)
		{
			vPlayerShadowLightPos = pPlayerShadowDesc->vDirection;
			vPlayerShadowLighAt = pPlayerShadowDesc->vDiffuse;
		}
		const LIGHTDESC* pPlayerFol_ShadowDesc = pGameInstance->Get_ShadowLightDesc(_int(LIGHTDESC::TYPE_FIELDSHADOW));
		if (nullptr != pPlayerFol_ShadowDesc)
		{
			vPlayerFol_ShadowLightPos = pPlayerFol_ShadowDesc->vDirection;
			vPlayerFol_ShadowLighAt = pPlayerFol_ShadowDesc->vDiffuse;
		}
		const LIGHTDESC* pObjectShadowDesc = pGameInstance->Get_ShadowLightDesc(_int(LIGHTDESC::TYPE_BATTLESHADOW));
		if (nullptr != pObjectShadowDesc)
		{
			vObjectShadowLightPos = pObjectShadowDesc->vDirection;
			vObjectShadowLighAt = pObjectShadowDesc->vDiffuse;
		}

		iPreLevel = g_iLevel;
	}



	ImGui::Text("Directional Light");
	ImGui::DragFloat3("Dir Light Dir", &vDirLightDir.x, 0.001f, 0.f, 1.f, "%.3f", window_flags);
	ImGui::DragFloat3("Dir Light Color", &vDirLightColor.x, 0.001f, 0.f, 1.f, "%.3f", window_flags);
	ImGui::DragFloat3("Dir Light Power", &vDirLightPower.x, 0.001f, 0.f, 1.f, "%.3f", window_flags);
	ImGui::DragFloat3("Dir Light Specular", &vDirLightSpecular.x, 0.001f, 0.f, 1.f, "%.3f", window_flags);
	const LIGHTDESC* pDirectionalLightDesc = pGameInstance->Get_LightDesc(_int(LIGHTDESC::TYPE_DIRECTIONAL));
	if (nullptr != pDirectionalLightDesc)
	{
		LIGHTDESC tDirLightDesc;

		tDirLightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
		XMStoreFloat4(&tDirLightDesc.vDirection, XMLoadFloat3(&vDirLightDir));
		XMStoreFloat4(&tDirLightDesc.vDiffuse, XMLoadFloat3(&vDirLightColor));
		XMStoreFloat4(&tDirLightDesc.vAmbient, XMLoadFloat3(&vDirLightPower));
		XMStoreFloat4(&tDirLightDesc.vSpecular, XMLoadFloat3(&vDirLightSpecular));

		pGameInstance->Set_LightDesc(LIGHTDESC::TYPE_DIRECTIONAL, tDirLightDesc);
	}



	ImGui::Separator();



	ImGui::Text("Point Light");
	ImGui::DragFloat3("Light Pos", &vLightPosition.x, 2.f, -2000.f, 2000.f, "%.1f", window_flags);
	ImGui::DragFloat("Light Range", &fLightRange, 0.2f, 0.f, 5000.f, "%.2f", window_flags);
	ImGui::DragFloat3("Light Color", &vLightColor.x, 0.001f, 0.f, 1.f, "%.3f", window_flags);
	ImGui::DragFloat3("Light Power", &vLightPower.x, 0.001f, 0.f, 1.f, "%.3f", window_flags);
	ImGui::DragFloat3("Light Specular", &vLightSpecular.x, 0.001f, 0.f, 1.f, "%.3f", window_flags);

	_int			iNumLight = 1;
	while (true)
	{
		const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(iNumLight);
		if (nullptr == pLightDesc)
			break;

		++iNumLight;
	}

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(_int(LIGHTDESC::TYPE_POINT1) + iLightIndex - 1);
	if (nullptr != pLightDesc)
	{
		if (0 < iLightIndex)
		{
			LIGHTDESC pNewLightDesc;
			pNewLightDesc.eType = LIGHTDESC::TYPE(_int(LIGHTDESC::TYPE_POINT1) + (iLightIndex - 1));
			pNewLightDesc.vPosition = _float4(vLightPosition.x, vLightPosition.y, vLightPosition.z, 1.f);
			pNewLightDesc.fRange = fLightRange;
			pNewLightDesc.vDiffuse = _float4(vLightColor.x, vLightColor.y, vLightColor.z, 1.f);
			pNewLightDesc.vAmbient = _float4(vLightPower.x, vLightPower.y, vLightPower.z, 1.f);
			pNewLightDesc.vSpecular = _float4(vLightSpecular.x, vLightSpecular.y, vLightSpecular.z, 1.f);

			pGameInstance->Set_LightDesc(iLightIndex, pNewLightDesc);
		}
	}



	if (ImGui::Button("Light Add", ImVec2(ImGui::GetWindowWidth() * 0.35f, 25.f)))
	{
		if (iNumLight < 6)	//	Dir + Point5개
		{
			LIGHTDESC	tLightDesc;
			tLightDesc.eType = LIGHTDESC::TYPE(_int(LIGHTDESC::TYPE_POINT1) + (iNumLight - 1));
			tLightDesc.vPosition = _float4(vLightPosition.x, vLightPosition.y, vLightPosition.z, 1.f);
			tLightDesc.fRange = fLightRange;
			tLightDesc.vDiffuse = _float4(vLightColor.x, vLightColor.y, vLightColor.z, 1.f);
			tLightDesc.vAmbient = _float4(vLightPower.x, vLightPower.y, vLightPower.z, 1.f);
			tLightDesc.vSpecular = _float4(vLightSpecular.x, vLightSpecular.y, vLightSpecular.z, 1.f);
			pGameInstance->Add_Light(m_pDevice, m_pContext, tLightDesc);

			++iNumLight;
		}
		else
		{
			ERR_MSG(L"Light NumMax = 5");
		}
	}



	if (ImGui::Button("Light Delete", ImVec2(ImGui::GetWindowWidth() * 0.35f, 25.f)))
	{
		if (0 < iLightIndex)	//	Dir + Point5개
		{
			if (FAILED(pGameInstance->Delete_Light(iLightIndex)))
				ERR_MSG(L"실패");
			else
			{
				--iLightIndex;
				--iNumLight;
			}
		}
	}




	ImGui::PushItemWidth(160);
	ImGui::BeginChild("Light", ImVec2(ImGui::GetWindowWidth() * 0.3f, 130), true, window_flags);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Light"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	for (_int i = 0; i < iNumLight; i++)
	{
		if (ImGui::Selectable(strLightName[i], iLightIndex == i))
			iLightIndex = i;
	}
	if (iPreLightIndex != iLightIndex)
	{
		if (0 < iLightIndex)
		{
			const LIGHTDESC* pChoiceLightDesc = pGameInstance->Get_LightDesc(_int(LIGHTDESC::TYPE_POINT1) + iLightIndex - 1);

			_float4 vPos = pChoiceLightDesc->vPosition;
			vLightPosition = _float3(vPos.x, vPos.y, vPos.z);
			fLightRange = pChoiceLightDesc->fRange;
			vLightColor = *(_float3*)(&pChoiceLightDesc->vDiffuse.x);
			vLightPower = *(_float3*)(&pChoiceLightDesc->vAmbient.x);
			vLightSpecular = *(_float3*)(&pChoiceLightDesc->vSpecular.x);
		}

		iPreLightIndex = iLightIndex;
	}
	ImGui::EndChild();


	ImGui::Separator();



	ImGui::Text("Player NonFollow Light");
	const LIGHTDESC* pPlayerShadowLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW);
	if (nullptr != pPlayerShadowLightDesc)
	{
		LIGHTDESC tLightDesc = *pPlayerShadowLightDesc;
		memcpy(&vPlayerShadowLightPos, &tLightDesc.vDirection.x, sizeof(_float4));
		_vector vLook = XMVectorSetW(XMLoadFloat4(&tLightDesc.vDiffuse) + XMLoadFloat3(&vDirLightDir), 1.f);
		memcpy(&vPlayerShadowLighAt, &vLook, sizeof(_float4));

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.8f);
		ImGui::DragFloat3("Player(NonFol) Shadow pos", &vPlayerShadowLightPos.x, 5.f, -2000.f, 2000.f, "%.1f");

		pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW, vPlayerShadowLightPos, vPlayerShadowLighAt);
	}

	ImGui::Text("Player Follow Light");
	const LIGHTDESC* pPlayerFolShadowLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW);
	if (nullptr != pPlayerFolShadowLightDesc)
	{
		LIGHTDESC tLightDesc = *pPlayerFolShadowLightDesc;
		memcpy(&vPlayerShadowLightPos, &tLightDesc.vDirection.x, sizeof(_float4));
		_vector vLook = XMVectorSetW(XMLoadFloat4(&tLightDesc.vDiffuse) + XMLoadFloat3(&vDirLightDir), 1.f);
		memcpy(&vPlayerShadowLighAt, &vLook, sizeof(_float4));

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.8f);
		ImGui::DragFloat3("Player(NonFol) Shadow pos", &vPlayerShadowLightPos.x, 5.f, -2000.f, 2000.f, "%.1f");

		pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW, vPlayerShadowLightPos, vPlayerShadowLighAt);
	}



	ImGui::Separator();



	ImGui::Text("Objects Light");
	const LIGHTDESC* pShadowLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW);
	if (nullptr != pShadowLightDesc)
	{
		LIGHTDESC tLightDesc = *pShadowLightDesc;
		memcpy(&vObjectShadowLightPos, &tLightDesc.vDirection.x, sizeof(_float4));
		_vector vLook = XMVectorSetW(XMLoadFloat4(&tLightDesc.vDiffuse) + XMLoadFloat3(&vDirLightDir), 1.f);
		memcpy(&vPlayerShadowLighAt, &vLook, sizeof(_float4));

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.8f);
		ImGui::DragFloat3("Shadow pos", &vObjectShadowLightPos.x, 5.f, -2000.f, 2000.f, "%.1f");

		pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW, vObjectShadowLightPos, vObjectShadowLighAt);
	}




	RELEASE_INSTANCE(CGameInstance);
}

void CImGuiManager::Camera_Action(_float fTimeDelta)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	static _int eChoice = 0;
	ImGui::RadioButton("Eye", &eChoice, (_int)CAM_EYE); ImGui::SameLine();
	ImGui::RadioButton("At", &eChoice, (_int)CAM_AT);
	ImGui::Separator();

	//	변수
	static _bool	bSaveWindow_Camera = false;
	static _bool	bLoadWindow_Camera = false;
	static float	f3Movement_Pos[3] = { 0.f, 0.f, 0.f };
	static _int iCamIndex[CAM_END] = { 0 , 0 };
	static _int	iPreCamIndex[CAM_END] = { 0, 0 };
	_int		iObjSize[CAM_END] = { (_int)m_vecCamObjects[CAM_EYE].size(), (_int)m_vecCamObjects[CAM_AT].size() };
	static _int iFixCamIndex[CAM_END] = { 0, 0 };
	static float fAtSphereLength = 10.f;
	
	static _int		iCamTimeIndex = 0;
	static _int		iPreCamTimeIndex = 0;
	static _float	fCamTime = 1.f;

	m_iNumCam[CAM_EYE] = (_int)m_vecCam[CAM_EYE].size();
	m_iNumCam[CAM_AT] = (_int)m_vecCam[CAM_AT].size();
	m_iNumCamTime = (_int)m_vecCamTime.size();


	//	Camera Action Pos
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
	static _float3		f3CamPos = _float3{ 0.f, 0.f, 0.f };
	ImGui::Text("Camera : %.2f / %.2f / %.2f", f3CamPos.x, f3CamPos.y, f3CamPos.z);
	if (nullptr != m_pCamera)
	{
		_float4 vCamPos = pGameInstance->Get_CamPosition();
		f3CamPos.x = vCamPos.x;
		f3CamPos.y = vCamPos.y;
		f3CamPos.z = vCamPos.z;
	}


	//	CamAction Save, Load
	if (ImGui::Button("Save_Cams", ImVec2(ImGui::GetWindowWidth() * 0.35f, 25.f)))
		bSaveWindow_Camera = true;
	if (true == bSaveWindow_Camera)
		bSaveWindow_Camera = Window_SaveCams(&bSaveWindow_Camera);
	ImGui::SameLine();
	if (ImGui::Button("Load_Cams", ImVec2(ImGui::GetWindowWidth() * 0.35f, 25.f)))
		bLoadWindow_Camera = true;
	if (true == bLoadWindow_Camera)
		bLoadWindow_Camera = Window_LoadCams(&bLoadWindow_Camera);
	ImGui::Separator();



	//	Eye, At Interface
	ImGui::DragFloat3("Position", f3Movement_Pos, 0.05f);
	_float fSpeed = 0.01f;
	//if (pGameInstance->Key_Pressing(DIK_LSHIFT))
	//	fSpeed = -0.01f;
	//if (pGameInstance->Key_Down(DIK_1))
	//	f3Movement_Pos[0] += fSpeed;
	//if (pGameInstance->Key_Down(DIK_2))
	//	f3Movement_Pos[1] += fSpeed;
	//if (pGameInstance->Key_Down(DIK_3))
	//	f3Movement_Pos[2] += fSpeed;
	if (0 < m_iNumCam[eChoice] && 0 != iCamIndex[eChoice])
	{
		m_vecCamObjects[eChoice][(iCamIndex[eChoice] - 1)]->Set_Pos(f3Movement_Pos[0], f3Movement_Pos[1], f3Movement_Pos[2]);
		XMStoreFloat4(&m_vecCam[eChoice][iCamIndex[eChoice]], XMVectorSet(f3Movement_Pos[0], f3Movement_Pos[1], f3Movement_Pos[2], 1.f));
		if (1 == iCamIndex[eChoice])
			XMStoreFloat4(&m_vecCam[eChoice][iCamIndex[eChoice] - 1], XMVectorSet(f3Movement_Pos[0], f3Movement_Pos[1], f3Movement_Pos[2], 1.f));

		Sort_CamNodes(iCamIndex[eChoice], (CAMTYPE)eChoice);
	}



	//	Cam Nodes
	static _bool bInputCamName = false;
	static char strEyeName[150][6] = { "" };
	static char strAtName[150][6] = { "" };
	static char strCamTimeName[150][6] = { "" };
	if (false == bInputCamName)
	{
		strcpy_s(strEyeName[0], "None");
		strcpy_s(strAtName[0], "None");
		strcpy_s(strCamTimeName[0], "None");
		for (_uint i = 1; i < 150; ++i)
		{
			char cTemp[6] = "";
			_itoa_s(i - 1, cTemp, sizeof(cTemp), 10);
			strcpy_s(strEyeName[i], sizeof(cTemp), cTemp);
			strcpy_s(strAtName[i], sizeof(cTemp), cTemp);
			strcpy_s(strCamTimeName[i], sizeof(cTemp), cTemp);
		}

		bInputCamName = true;
	}
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	ImGui::PushItemWidth(160);
	ImGui::BeginChild("Eye", ImVec2(ImGui::GetWindowWidth() * 0.3f, 130), true, window_flags);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Eye"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	for (_int i = 0; i < m_iNumCam[CAM_EYE]; i++)
	{
		if (ImGui::Selectable(strEyeName[i], iCamIndex[CAM_EYE] == i))
			iCamIndex[CAM_EYE] = i;
	}
	if (0 != iCamIndex[CAM_EYE])
	{
		eChoice = (_int)CAM_EYE;
		iCamIndex[CAM_AT] = 0;

		_float4 vPos = m_vecCam[CAM_EYE][iCamIndex[CAM_EYE]];
		f3Movement_Pos[0] = vPos.x;
		f3Movement_Pos[1] = vPos.y;
		f3Movement_Pos[2] = vPos.z;

		m_vecCamObjects[CAM_EYE][iCamIndex[CAM_EYE] - 1]->Set_Color(_float3(1.f, 0.f, 1.f));
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("At", ImVec2(ImGui::GetWindowWidth() * 0.3f, 130), true, window_flags);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("At"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	for (_int i = 0; i < m_iNumCam[CAM_AT]; i++)
	{
		if (ImGui::Selectable(strAtName[i], iCamIndex[CAM_AT] == i))
			iCamIndex[CAM_AT] = i;
	}
	if (0 != iCamIndex[CAM_AT])
	{
		eChoice = (_int)CAM_AT;
		iCamIndex[CAM_EYE] = 0;

		_float4 vPos = m_vecCam[CAM_AT][iCamIndex[CAM_AT]];
		f3Movement_Pos[0] = vPos.x;
		f3Movement_Pos[1] = vPos.y;
		f3Movement_Pos[2] = vPos.z;

		m_vecCamObjects[CAM_AT][iCamIndex[CAM_AT] - 1]->Set_Color(_float3(1.f, 0.f, 1.f));
	}
	ImGui::EndChild();

	if (0 != iCamIndex[CAM_EYE])
	{
		if (iPreCamIndex[CAM_EYE] != iCamIndex[CAM_EYE])	// Change EyeIndex
		{
			_float4 vPos = m_vecCam[CAM_EYE][iCamIndex[CAM_EYE]];
			f3Movement_Pos[0] = vPos.x;
			f3Movement_Pos[1] = vPos.y;
			f3Movement_Pos[2] = vPos.z;

			if (0 != iPreCamIndex[CAM_EYE] &&
				iPreCamIndex[CAM_EYE] <= m_vecCamObjects[CAM_EYE].size())
				m_vecCamObjects[CAM_EYE][iPreCamIndex[CAM_EYE] - 1]->Set_Color(_float3(1.f, 0.f, 0.f));
			if (0 != iPreCamIndex[CAM_AT] &&
				iPreCamIndex[CAM_AT] <= m_vecCamObjects[CAM_AT].size())
				m_vecCamObjects[CAM_AT][iPreCamIndex[CAM_AT] - 1]->Set_Color(_float3(0.f, 1.f, 0.f));

			m_vecCamObjects[CAM_EYE][iCamIndex[CAM_EYE] - 1]->Set_Color(_float3(1.f, 0.f, 1.f));

			iPreCamIndex[CAM_EYE] = iCamIndex[CAM_EYE];
		}
	}
	else if (0 != iCamIndex[CAM_AT])
	{
		if (iPreCamIndex[CAM_AT] != iCamIndex[CAM_AT])	// Change AtIndex
		{
			_float4 vPos = m_vecCam[CAM_AT][iCamIndex[CAM_AT]];
			f3Movement_Pos[0] = vPos.x;
			f3Movement_Pos[1] = vPos.y;
			f3Movement_Pos[2] = vPos.z;

			if (0 != iPreCamIndex[CAM_EYE] &&
				iPreCamIndex[CAM_EYE] <= m_vecCamObjects[CAM_EYE].size())
				m_vecCamObjects[CAM_EYE][iPreCamIndex[CAM_EYE] - 1]->Set_Color(_float3(1.f, 0.f, 0.f));
			if (0 != iPreCamIndex[CAM_AT] &&
				iPreCamIndex[CAM_AT] <= m_vecCamObjects[CAM_AT].size())
				m_vecCamObjects[CAM_AT][iPreCamIndex[CAM_AT] - 1]->Set_Color(_float3(0.f, 1.f, 0.f));

			m_vecCamObjects[CAM_AT][iCamIndex[CAM_AT] - 1]->Set_Color(_float3(1.f, 0.f, 1.f));

			iPreCamIndex[CAM_AT] = iCamIndex[CAM_AT];
		}
	}

	if (0 != iCamIndex[CAM_EYE])
		iFixCamIndex[CAM_EYE] = iCamIndex[CAM_EYE] - 1;		//	vector에 사용되는 인덱스.
	else if (0 != iCamIndex[CAM_AT])
		iFixCamIndex[CAM_AT] = iCamIndex[CAM_AT] - 1;		//	vector에 사용되는 인덱스.





	if (ImGui::Button("Move", ImVec2(ImGui::GetWindowWidth() * 0.20f, 20.f)) &&
		1 < m_iNumCam[eChoice])
	{
		_float3 vPos = _float3(f3Movement_Pos[0], f3Movement_Pos[1], f3Movement_Pos[2]);
		((CCamera_Dynamic*)m_pCamera)->Set_Pos(vPos);
	}

	ImGui::SameLine();
	if (ImGui::Button("LookAt", ImVec2(ImGui::GetWindowWidth() * 0.20f, 20.f)) &&
		1 < m_iNumCam[eChoice])
	{
		_float3 vPos = _float3(f3Movement_Pos[0], f3Movement_Pos[1], f3Movement_Pos[2]);
		((CTransform*)(m_pCamera->Find_Component(L"Com_Transform")))->LookAt(XMVectorSetW(XMLoadFloat3(&vPos), 1.f));
	}





	//	Push, Pop
	if (ImGui::Button("Push", ImVec2(ImGui::GetWindowWidth() * 0.20f, 20.f)))
	{
		if (0 == m_iNumCam[eChoice])
			m_vecCam[eChoice].push_back(pGameInstance->Get_CamPosition());		//	맨 처음 None용 인덱스. 못씀.
		m_vecCam[eChoice].push_back(pGameInstance->Get_CamPosition());

		if (0 != m_iNumCam[eChoice])
		{
			if (iCamIndex[eChoice] + 1 != m_iNumCam[eChoice])		//	사이에 집어넣기
			{
				_float4	vPosTemp = *(m_vecCam[eChoice].end() - 1);
				for (std::vector<_float4>::iterator iter = m_vecCam[eChoice].begin() + (iCamIndex[eChoice] + 1);
					iter != m_vecCam[eChoice].end(); ++iter)
				{
					swap(*iter, vPosTemp);
				}
			}
		}

		_vector vPos[4];
		_int	iSize = (_int)m_vecCam[eChoice].size();
		for (_int j = 0; j < 4; ++j)
		{
			_int	iIndex = max(min(iFixCamIndex[eChoice] + j, iSize - 1), 0);		//	최소 = 0, 최대 = Size
			vPos[j] = XMLoadFloat4(&m_vecCam[eChoice][iIndex]);
		}


		CGameObj* pGameObject = nullptr;
		CCamLine::tagCamLineDesc tCamLineDesc;
		tCamLineDesc.vColor = eChoice == CAM_EYE ? _float3(1.f, 0.f, 0.f) : _float3(0.f, 1.f, 0.f);
		for (_int i = 0; i < 4; ++i)
			XMStoreFloat3(&tCamLineDesc.vPos[i], vPos[i]);
		pGameInstance->Add_GameObject(L"Prototype_GameObject_CamLine", LEVEL_STATIC, L"Layer_CamLine", &tCamLineDesc);
		if (nullptr == tCamLineDesc.pMe)
		{
			ERR_MSG(L"Failed to Create : CamLine");
		}
		else
		{
			m_vecCamObjects[eChoice].push_back(tCamLineDesc.pMe);
			Safe_AddRef(tCamLineDesc.pMe);
		}

		Sort_CamNodes((CAMTYPE)eChoice);
		++iCamIndex[eChoice];
		++iFixCamIndex[eChoice];

		f3Movement_Pos[0] = m_vecCam[eChoice][iCamIndex[eChoice]].x;
		f3Movement_Pos[1] = m_vecCam[eChoice][iCamIndex[eChoice]].y;
		f3Movement_Pos[2] = m_vecCam[eChoice][iCamIndex[eChoice]].z;
	}
	ImGui::SameLine();
	if (ImGui::Button("Pop", ImVec2(ImGui::GetWindowWidth() * 0.20f, 20.f)) &&
		iFixCamIndex[eChoice] >= 1)
	{
		auto iter_Obj = m_vecCamObjects[eChoice].begin() + (iFixCamIndex[eChoice]);
		(*iter_Obj)->Set_Dead();
		Safe_Release(*iter_Obj);
		iter_Obj = m_vecCamObjects[eChoice].erase(iter_Obj);

		auto iter_EyeAt = m_vecCam[eChoice].begin() + (iFixCamIndex[eChoice] + 1);
		m_vecCam[eChoice].erase(iter_EyeAt);

		Sort_CamNodes((CAMTYPE)eChoice);

		f3Movement_Pos[0] = m_vecCam[eChoice][iCamIndex[eChoice] - 1].x;
		f3Movement_Pos[1] = m_vecCam[eChoice][iCamIndex[eChoice] - 1].y;
		f3Movement_Pos[2] = m_vecCam[eChoice][iCamIndex[eChoice] - 1].z;

		--iCamIndex[eChoice];
		--iFixCamIndex[eChoice];
		--iPreCamIndex[eChoice];
	}

	ImGui::DragFloat("Length", &fAtSphereLength, 0.01f, 0.1f, 1000.f, "%.2f");

	if (ImGui::Button("push Length_", ImVec2(ImGui::GetWindowWidth() * 0.20f, 20.f)))
	{
		if (eChoice == CAM_AT)
		{
			_float4 vAtSpherePos;
			XMStoreFloat4(&vAtSpherePos, XMVectorSetW(XMLoadFloat4(&pGameInstance->Get_CamPosition()) + (fAtSphereLength * XMLoadFloat4(&pGameInstance->Get_CamLook())), 1.f));
			if (0 == m_iNumCam[eChoice])
				m_vecCam[eChoice].push_back(vAtSpherePos);		//	맨 처음 None용 인덱스. 못씀.
			m_vecCam[eChoice].push_back(vAtSpherePos);

			if (0 != m_iNumCam[eChoice])
			{
				if (iCamIndex[eChoice] + 1 != m_iNumCam[eChoice])		//	사이에 집어넣기
				{
					_float4	vPosTemp = *(m_vecCam[eChoice].end() - 1);
					for (std::vector<_float4>::iterator iter = m_vecCam[eChoice].begin() + (iCamIndex[eChoice] + 1);
						iter != m_vecCam[eChoice].end(); ++iter)
					{
						swap(*iter, vPosTemp);
					}
				}
			}

			_vector vPos[4];
			_int	iSize = (_int)m_vecCam[eChoice].size();
			for (_int j = 0; j < 4; ++j)
			{
				_int	iIndex = max(min(iFixCamIndex[eChoice] + j, iSize - 1), 0);		//	최소 = 0, 최대 = Size
				vPos[j] = XMLoadFloat4(&m_vecCam[eChoice][iIndex]);
			}


			CGameObj* pGameObject = nullptr;
			CCamLine::tagCamLineDesc tCamLineDesc;
			tCamLineDesc.vColor = eChoice == CAM_EYE ? _float3(1.f, 0.f, 0.f) : _float3(0.f, 1.f, 0.f);
			for (_int i = 0; i < 4; ++i)
				XMStoreFloat3(&tCamLineDesc.vPos[i], vPos[i]);
			pGameInstance->Add_GameObject(L"Prototype_GameObject_CamLine", LEVEL_STATIC, L"Layer_CamLine", &tCamLineDesc);
			if (nullptr == tCamLineDesc.pMe)
			{
				ERR_MSG(L"Failed to Create : CamLine");
			}
			else
			{
				m_vecCamObjects[eChoice].push_back(tCamLineDesc.pMe);
				Safe_AddRef(tCamLineDesc.pMe);
			}

			Sort_CamNodes((CAMTYPE)eChoice);
			++iCamIndex[eChoice];
			++iFixCamIndex[eChoice];

			f3Movement_Pos[0] = m_vecCam[eChoice][iCamIndex[eChoice]].x;
			f3Movement_Pos[1] = m_vecCam[eChoice][iCamIndex[eChoice]].y;
			f3Movement_Pos[2] = m_vecCam[eChoice][iCamIndex[eChoice]].z;
		}
	}


	if (ImGui::Button("Sort", ImVec2(ImGui::GetWindowWidth() * 0.12f, 20.f)))
	{
		for (_int CamType = 0; CamType < CAM_END; ++CamType)
		{
			Sort_CamNodes((CAMTYPE)CamType);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear", ImVec2(ImGui::GetWindowWidth() * 0.12f, 20.f)))
	{
		m_vecCam[eChoice].clear();

		for (auto & iter : m_vecCamObjects[eChoice])
		{
			iter->Set_Dead();
			Safe_Release(iter);
		}
		m_vecCamObjects[eChoice].clear();

		iCamIndex[eChoice] = 0;
		iPreCamIndex[eChoice] = 0;

		iObjSize[eChoice] = (_int)m_vecCamObjects[eChoice].size();
		m_iNumCam[eChoice] = (_int)m_vecCam[eChoice].size();
	}



	static _bool bRenderObjs = true;
	static _bool bPreRenderObjs = true;
	ImGui::Checkbox("Obj Render", &bRenderObjs);
	if (bPreRenderObjs != bRenderObjs)
	{
		((CTerrain*)(pGameInstance->Find_Layer(g_iLevel, L"Layer_Terrain")->Get_LayerFront()))->Set_SplRender(!bRenderObjs);
		list<CGameObject*> plistMesh = pGameInstance->Find_Layer(g_iLevel, L"Layer_MeshObj_Static")->Get_ObjectList();
		list<CGameObject*> plistMeshInst = pGameInstance->Find_Layer(g_iLevel, L"Layer_MeshObj_Static_Inst")->Get_ObjectList();
		for (auto& iterMesh : plistMesh)
		{
			dynamic_cast<CMeshObj_Static*>(iterMesh)->Set_SplRender(!bRenderObjs);
		}
		for (auto& iterMeshinst : plistMeshInst)
		{
			dynamic_cast<CMeshObj_Static_Inst*>(iterMeshinst)->Set_SplRender(!bRenderObjs);
		}

		bPreRenderObjs = bRenderObjs;
	}




	//	Time
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
	ImGui::DragFloat("Time", &fCamTime, 0.01f, 0.f, 7.f, "%.2f");
	if (0.f > fCamTime)
		fCamTime = 0.f;
	if (m_iNumCamTime > iCamTimeIndex)
		m_vecCamTime[iCamTimeIndex] = fCamTime;



	ImGui::BeginChild("Time List", ImVec2(ImGui::GetWindowWidth() * 0.3f, 130), true, window_flags);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Time"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	for (_int i = 0; i < m_iNumCamTime; i++)
	{
		if (ImGui::Selectable(strCamTimeName[i], iCamTimeIndex == i))
			iCamTimeIndex = i;
	}
	ImGui::EndChild();
	if (m_iNumCamTime > iCamTimeIndex)
		fCamTime = m_vecCamTime[iCamTimeIndex];


	if (ImGui::Button("Time Push", ImVec2(ImGui::GetWindowWidth() * 0.20f, 20.f)))
	{
		if (0 == m_vecCamTime.size())
			m_vecCamTime.push_back(0.f);
		m_vecCamTime.push_back(1.f);
		m_iNumCamTime = (_int)m_vecCamTime.size();
		if (2 < m_iNumCamTime)
		{
			if (iCamTimeIndex + 1 != m_iNumCamTime)		//	사이에 집어넣기
			{
				_float	fTimeTemp = *(m_vecCamTime.end() - 1);
				for (std::vector<_float>::iterator iter = m_vecCamTime.begin() + (iCamTimeIndex + 1);
					iter != m_vecCamTime.end(); ++iter)
				{
					swap(*iter, fTimeTemp);
				}
			}
		}

		++iCamTimeIndex;

		fCamTime = 1.f;
	}
	ImGui::SameLine();
	if (ImGui::Button("Time Pop", ImVec2(ImGui::GetWindowWidth() * 0.20f, 20.f)) &&
		0 < iCamTimeIndex)
	{
		auto iter = m_vecCamTime.begin() + iCamTimeIndex - 1;
		iter = m_vecCamTime.erase(iter);

		--iCamTimeIndex;

		if (iter != m_vecCamTime.end())
			fCamTime = *iter;
	}




	if (ImGui::Button("Time Clear", ImVec2(ImGui::GetWindowWidth() * 0.2f, 20.f)))
	{
		m_vecCamTime.clear();

		iCamTimeIndex = 0;
		iPreCamTimeIndex = 0;
		fCamTime = 1.f;

		m_iNumCamTime = 0;
	}



	static _float fSettingTime = 0.f;
	static _bool bClusterRender = true;
	static _bool bPreClusterRender = true;
	static _bool bTimeSlow = false;
	static _float fClusterRadius = 0.1f;
	static _float fPreClusterRadius = 0.1f;
	static char strCamActionName[MAX_PATH][10] = { "Tan1", "Tan2", "Tan3", "Tan4", "Tan5",
		"RuiStt", "Rui0", "Rui1", "Rui2", "Rui3" , "Rui4" , "Rui5" ,
		"RgkStt", "Rgk0", "Rgk1", "Rgk2", "Rgk3" , "Rgk4" , "Rgk5" , "Rgk6", "Rgk7" , "Rgk8",
		"AkzStt", "Akz0", "Akz1", "Akz2", "Akz3" , "Akz4" , "Akz5" , "Akz6",
		"NzkStt", "Nzk0", "Nzk1", "Nzk2", "Nzk3" , "Nzk4" , "Nzk5" , "Nzk6", "Nzk7", "Nzk8", "Nzk9_0", "Nzk9_1",
		"SnbStt", "Snb0", "Snb1", "Snb2", "Snb3"
	};
	static _int iCameraActionIndex = 0;
	if (ImGui::CollapsingHeader("Setting Actions"))
	{
		//	Play Time
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.68f);
		ImGui::SliderFloat("Full Time", &fSettingTime, 0.f, (_float)(m_vecCamTime.size() - 1), "%f");
		if (0.f > fSettingTime)
			fSettingTime = 0.f;

		//	Play CutScene
		if (ImGui::Button("Play", ImVec2(ImGui::GetWindowWidth() * 0.2f, 20.f)))
			m_bCutScene = true;

		/*if (pGameInstance->Key_Down(DIK_F3) ||
			pGameInstance->Key_Down(DIK_F4) || 
			pGameInstance->Key_Down(DIK_F5) ||
			pGameInstance->Key_Down(DIK_F6) ||
			pGameInstance->Key_Down(DIK_F7) ||
			pGameInstance->Key_Down(DIK_F8) ||
			pGameInstance->Key_Down(DIK_F9) ||
			pGameInstance->Key_Down(DIK_CAPSLOCK) ||
			pGameInstance->Key_Down(DIK_PGUP) ||
			pGameInstance->Key_Down(DIK_PGDN))
			m_bCutScene = true;*/

		((CCamera_Dynamic*)m_pCamera)->Start_CutScene(m_bCutScene, CCamera_Dynamic::CUTSCENE_END);
		if (m_iNumCam[CAM_EYE] == m_iNumCam[CAM_AT] &&
			m_iNumCam[CAM_EYE] == m_iNumCamTime + 1 &&
			true == m_bCutScene)
			//	((CCamera_Dynamic*)m_pCamera)->Set_CutScene_Tool(m_vecCam[CAM_EYE], m_vecCam[CAM_AT], m_vecCamTime, m_fMotionBlur);
			m_bCutScene = ((CCamera_Dynamic*)m_pCamera)->Play_CutScene(m_vecCam[CAM_EYE], m_vecCam[CAM_AT], m_vecCamTime, &fSettingTime, fTimeDelta * (_float)m_bCutScene);


		ImGui::SameLine();
		if (ImGui::Button("Stop", ImVec2(ImGui::GetWindowWidth() * 0.2f, 20.f)))
			m_bCutScene = false;

		ImGui::Checkbox("Slow", &bTimeSlow);
		g_bDeathTime = bTimeSlow;

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
		ImGui::DragFloat2("MotionBlur", &m_fMotionBlur.x, 0.01f, 0.f, 10.f, "%.2f");
		m_pRendererCom->MotionBlur(m_fMotionBlur.x, m_fMotionBlur.y);

		ImGui::Checkbox("Render", &bClusterRender);
		if (bPreClusterRender != bClusterRender)
		{
			for (_int iType = 0; iType < CAM_END; ++iType)
			{
				for (auto & iter : m_vecCamObjects[iType])
				{
					iter->Set_Rendering(bClusterRender);
				}
			}

			bPreClusterRender = bClusterRender;
		}

		ImGui::DragFloat("Cluster Radius", &fClusterRadius, 0.001f, 0.f, 1.f, "%.3f");
		if (0.f > fClusterRadius)
			fClusterRadius = 0.001f;

		for (_int iType = 0; iType < CAM_END; ++iType)
		{
			for (auto & iter : m_vecCamObjects[iType])
			{
				iter->Set_SphereSize(fClusterRadius);
			}
		}
		fPreClusterRadius = fClusterRadius;

		ImGui::BeginChild("Change CamAction", ImVec2(ImGui::GetWindowWidth() * 0.3f, 130), true, window_flags);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Change CamAction"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		for (_int i = 0; i < CCamera_Dynamic::CUTSCENE_END; i++)
		{
			if (ImGui::Selectable(strCamActionName[i], iCameraActionIndex == i))
				iCameraActionIndex = i;
		}
		ImGui::EndChild();
		if (ImGui::Button("Change", ImVec2(ImGui::GetWindowWidth() * 0.2f, 20.f)))
			((CCamera_Dynamic*)m_pCamera)->Change_CutScene((CCamera_Dynamic::CUTSCENE)iCameraActionIndex, m_vecCam[CAM_EYE], m_vecCam[CAM_AT], m_vecCamTime, m_fMotionBlur);
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CImGuiManager::Sort_CamNodes(CAMTYPE eCamType)
{
	_float3 vPos[4];
	_int	iObjSize = (_int)m_vecCamObjects[eCamType].size();

	if (3 > iObjSize)
		return;

	for (_int i = 0; i < iObjSize; ++i)
	{
		_float3 vPos[4];
		for (_int j = 0; j < 4; ++j)
		{
			_int	iIndex = max(min(i + j - 1, iObjSize), 0);		//	최소 = 0, 최대 = Size
			XMStoreFloat3(&vPos[j], XMLoadFloat4(&m_vecCam[eCamType][iIndex]));
		}

		m_vecCamObjects[eCamType][i]->Set_Pos(vPos);
	}
}

void CImGuiManager::Sort_CamNodes(_int iIndex, CAMTYPE eCamType)
{
	if (0 > iIndex)
		return;

	_float3 vPos[4];
	_int	iObjSize = (_int)m_vecCamObjects[eCamType].size();
	if (2 > iObjSize)
		return;

	_int	iSortIndex = max(min(iIndex - 2, iObjSize), 0);
	_int	iSortFinal = max(min(iIndex + 2, iObjSize), 0);
	for (_int i = iSortIndex; i < iSortFinal; ++i)
	{
		_float3 vPos[4];
		for (_int j = 0; j < 4; ++j)
		{
			_int	iIndex = max(min(i + j - 1, iObjSize), 0);		//	최소 = 0, 최대 = Size
			XMStoreFloat3(&vPos[j], XMLoadFloat4(&m_vecCam[eCamType][iIndex]));
		}

		m_vecCamObjects[eCamType][i]->Set_Pos(vPos);
	}
}

_bool CImGuiManager::Window_LoadCams(_bool * bOpen)
{
	static int		iIndex = 0;
	ImGui::SetNextWindowSize(ImVec2(400, 450), ImGuiCond_FirstUseEver);
	ImGui::Begin("Load CamActions", bOpen);
	if (false == *bOpen)
	{
		iIndex = 0;
		ImGui::End();
		return false;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	ImGui::ListBox("CamAct Files", &iIndex, m_strCamFiles, m_iNumCamFiles, 10);
	if (ImGui::Button("Load", ImVec2(50.f, 20.f)))
	{
		char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/CamActions/";
		strcat_s(szFilePath, m_strCamFiles[iIndex]);

		_tchar		szRealPath[MAX_PATH] = L"";
		MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

		HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Failed to Load : CamAction File");

			iIndex = 0;
			RELEASE_INSTANCE(CGameInstance);
			ImGui::End();
			return false;
		}

		DWORD	dwByte = 0;
		_float4*	pCamEye = new _float4;
		_float4*	pCamAt = new _float4;
		_float*		pCamTime = new _float;

		_float2*	pBlur = new _float2;
		ReadFile(hFile, pBlur, sizeof(_float2), &dwByte, nullptr);
		m_fMotionBlur = *pBlur;
		while (true)
		{
			ReadFile(hFile, pCamEye, sizeof(_float4), &dwByte, nullptr);
			ReadFile(hFile, pCamAt, sizeof(_float4), &dwByte, nullptr);
			ReadFile(hFile, pCamTime, sizeof(_float), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pCamEye);
				Safe_Delete(pCamAt);
				Safe_Delete(pCamTime);
				Safe_Delete(pBlur);

				//	std::vector<_float>::iterator iter = m_vecCamTime.end();
				m_vecCamTime.erase(m_vecCamTime.end() - 1);
				--m_iNumCamTime;
				break;
			}

			_float4		vCamEye = *pCamEye;
			_float4		vCamAt = *pCamAt;
			_float		fCamTime = *pCamTime;

			m_vecCam[CAM_EYE].push_back(vCamEye);
			++m_iNumCam[CAM_EYE];
			m_vecCam[CAM_AT].push_back(vCamAt);
			++m_iNumCam[CAM_AT];
			m_vecCamTime.push_back(fCamTime);
			++m_iNumCamTime;
		}
		CloseHandle(hFile);



		_vector		vPos[4];
		for (_int iType = 0; iType < CAM_END; ++iType)
		{
			_int		iCamSize = m_iNumCam[iType];

			for (_int i = 0; i < m_iNumCam[iType]; ++i)
			{
				if (3 < m_iNumCam[iType])
				{
					if (0 == i)		//	None은 오브젝트를 만들지않음.
						continue;

					for (_int j = 0; j < 4; ++j)
					{
						_int	iIndex = max(min(i + j - 1, iCamSize - 1), 0);		//	최소 = 0, 최대 = Size
						vPos[j] = XMLoadFloat4(&m_vecCam[iType][iIndex]);
					}

					CGameObj* pGameObject = nullptr;
					CCamLine::tagCamLineDesc tCamLineDesc;
					tCamLineDesc.vColor = iType == CAM_EYE ? _float3(1.f, 0.f, 0.f) : _float3(0.f, 1.f, 0.f);
					for (_int j = 0; j < 4; ++j)
						XMStoreFloat3(&tCamLineDesc.vPos[j], vPos[j]);
					pGameInstance->Add_GameObject(L"Prototype_GameObject_CamLine", LEVEL_STATIC, L"Layer_CamLine", &tCamLineDesc);
					if (nullptr == tCamLineDesc.pMe)
					{
						ERR_MSG(L"Failed to Create : CamLine");
					}
					else
					{
						m_vecCamObjects[iType].push_back(tCamLineDesc.pMe);
						Safe_AddRef(tCamLineDesc.pMe);
					}
				}
			}
			Sort_CamNodes((CAMTYPE)iType);
		}

		Read_CamActionFiles();
		iIndex = 0;
		RELEASE_INSTANCE(CGameInstance);
		ImGui::End();
		return false;
	}


	RELEASE_INSTANCE(CGameInstance);
	ImGui::End();
	return true;
}

_bool CImGuiManager::Window_SaveCams(_bool * bOpen)
{
	static char		InputBuf[MAX_PATH] = "";
	int				iIndex = -1;

	ImGui::SetNextWindowSize(ImVec2(400, 450), ImGuiCond_FirstUseEver);
	ImGui::Begin("Save CamAction", bOpen);
	if (false == *bOpen)
	{
		strcpy_s(InputBuf, "");
		ImGui::End();
		return false;
	}

	ImGui::ListBox("CamAction Files", &iIndex, m_strCamFiles, m_iNumCamFiles, 10);
	ImGui::InputText("File Name", InputBuf, IM_ARRAYSIZE(InputBuf));
	if (ImGui::Button("Save", ImVec2(50.f, 20.f)))
	{
		if (m_vecCam[CAM_EYE].size() != m_vecCam[CAM_AT].size() ||
			m_vecCam[CAM_EYE].size() != m_vecCamTime.size() + 1)
		{
			ERR_MSG(L"Eye Size != At Size != CamTime Size");

			strcpy_s(InputBuf, "");
			ImGui::End();
			return false;
		}
		_int iSize = (_int)m_vecCam[CAM_EYE].size();
		FILE*		fp = nullptr;

		char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/CamActions/";
		strcat_s(szFilePath, InputBuf);
		strcat_s(szFilePath, ".cma");
		errno_t		err = fopen_s(&fp, szFilePath, "wb");

		if (0 == err)
		{
			fwrite(&(m_fMotionBlur), sizeof(_float2), 1, fp);
			for (_int i = 0; i < iSize; ++i)
			{
				_float4 vEyePos;
				_float4 vAtPos;
				_float	fUseTime;
				XMStoreFloat4(&vEyePos, XMLoadFloat4(&m_vecCam[CAM_EYE][i]));
				XMStoreFloat4(&vAtPos, XMLoadFloat4(&m_vecCam[CAM_AT][i]));
				if (i != iSize)
					fUseTime = m_vecCamTime[i];

				fwrite(&(vEyePos), sizeof(_float4), 1, fp);
				fwrite(&(vAtPos), sizeof(_float4), 1, fp);

				if (i != iSize)
					fwrite(&(fUseTime), sizeof(_float), 1, fp);
			}

			ERR_MSG(L"Success to Save : Cam Action");
			fclose(fp);
		}
		else
			ERR_MSG(L"Failed to Load : Cam Action");

		Read_CamActionFiles();

		strcpy_s(InputBuf, "");
		ImGui::End();
		return false;
	}


	ImGui::End();

	return true;
}

void CImGuiManager::Read_CamActionFiles()
{
	string path = "../Bin/Resources/Data/CamActions/*.cma";
	m_iNumCamFiles = 0;

	struct _finddata_t fd;
	intptr_t handle;
	if ((handle = _findfirst(path.c_str(), &fd)) == -1L)
	{
		ERR_MSG(L"Failed to Read : CamAction Files");
		return;
	}
	do
	{
		char strFileName[MAX_PATH] = "";
		strcpy_s(strFileName, fd.name);

		strcpy_s(m_strCamFiles[m_iNumCamFiles], MAX_PATH, strFileName);
		++m_iNumCamFiles;
	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);
}



void CImGuiManager::AnimationDebug(_float fTimeDelta)
{

	if (g_iLevel == 12)
	{
		if (m_bReset == false)
		{
			m_vecObjList.clear();
			m_vecAnimation.clear();

			for (_uint i = 0; i < CEnmuBoss::Get_Instance()->Get_EnmuPartsList().size(); ++i)
			{
				m_vecObjList.push_back(CEnmuBoss::Get_Instance()->Get_EnmuPartsList()[i]);
			}
			m_bReset = true;
		}

		EnmuBossAnimDebug();
	}
	else
		LiveCharacterList();

}

void CImGuiManager::Add_LiveCharacter(CCharacters * pCharacter)
{
	if (pCharacter == nullptr)
		return;

	else if (pCharacter != nullptr)
		m_vecObjList.push_back(pCharacter);

}

void CImGuiManager::LiveCharacterList()
{

	ImVec2 vListSize(100, 60);
	ImVec2 vObjSize(100, 20);
	static int selected = 999;


	if (ImGui::BeginListBox("Character", vListSize))
	{
		for (_uint i = 0; i < m_vecObjList.size(); ++i)
		{
			wstring wStrName = m_vecObjList[i]->Get_PlayerInfo().strName;
			string strName;

			if (wStrName == L"탄지로")
				strName = "Tanjiro";
			else if (wStrName == L"쿄주로")
				strName = "Kyoujuro";
			else if (wStrName == L"루이")
				strName = "Rui";
			else if (wStrName == L"아카자")
				strName = "Akaza";
			else if (wStrName == L"네즈코")
				strName = "Nezuko";
			else if (wStrName == L"시노부")
				strName = "Shinobu";
			else if (wStrName == L"엔무")
				strName = "Enmu";

			//// 보스
			//else if (wStrName == L"엔무(각성)")
			//	strName = "Head";
			//else if (wStrName == L"엔무(쉴드)")
			//	strName = "Shield";
			//else if (wStrName == L"엔무_오른손")
			//	strName = "RightHand";
			//else if (wStrName == L"엔무_왼손")
			//	strName = "LeftHand";
			//else if (wStrName == L"엔무_촉수")
			//	strName = "CHOKCHOK";


			if (ImGui::Selectable(strName.c_str(), selected == i, 0, vObjSize))
			{
				selected = i;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}


	if (selected != 999)
		CharacterAnimationList(selected);
}

void CImGuiManager::CharacterAnimationList(_uint _iIndex)
{
	ImVec2 vListSize(300, 200);
	ImVec2 vObjSize(300, 15);
	static int selected = 0;

	if (_iIndex == 0)
	{
		if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
		{
			m_vecAnimation = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
		{
			m_vecAnimation = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
		{
			m_vecAnimation = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
		{
			m_vecAnimation = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
		{
			m_vecAnimation = ((CNezuko*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
		{
			m_vecAnimation = ((CShinobu*)(m_vecObjList[0]))->Get_Model()->Get_Animation();
		}

	}
	else if (_iIndex == 1)
	{
		if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
		{
			m_vecAnimation = ((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
		{
			m_vecAnimation = ((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
		{
			m_vecAnimation = ((CRui*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
		{
			m_vecAnimation = ((CAkaza*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
		{
			m_vecAnimation = ((CNezuko*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
		{
			m_vecAnimation = ((CShinobu*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"엔무")
		{
			m_vecAnimation = ((CEnmu*)(m_vecObjList[1]))->Get_Model()->Get_Animation();
		}
	}


	if (ImGui::BeginListBox("..", vListSize))
	{
		for (_uint i = 0; i < m_vecAnimation.size(); ++i)
		{
			string Temp = to_string(i) + ". ";
			string strName = m_vecAnimation[i]->Get_AnimName();

			if (_iIndex == 0)
			{
				if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
				{
					strName.erase(strName.begin(), strName.begin() + 33);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				//strName.erase(strName.begin(), strName.begin() + 38);
			}
			else if (_iIndex == 1)
			{
				if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}

				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"엔무")
				{
					strName.erase(strName.begin(), strName.begin() + 38);
				}
				//strName.erase(strName.begin(), strName.begin() + 53);
			}

			strName = Temp + strName;

			if (ImGui::Selectable(strName.c_str(), selected == i, 0, vObjSize))
			{
				selected = i;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}


	ImGui::Text("-----------add animation------------");


	ImVec2 vListSize1(300, 100);
	ImVec2 vObjSize1(300, 20);
	static int selected1 = 0;
	if (ImGui::BeginListBox(".", vListSize1))
	{

		for (_uint i = 0; i < m_vecAnimIndex.size(); ++i)
		{
			string str = to_string(i);
			string Temp = to_string(i) + ". ";

			str = Temp + str;

			if (ImGui::Selectable(str.c_str(), stoi(str) == i, 0, vObjSize1))
			{
				selected1 = i;
			}
			ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("Play Animation"))
	{
		if (_iIndex == 0)
		{
			if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
			{
				((CTanjiro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
			{
				((CKyoujuro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
			{
				((CRui*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
			{
				((CAkaza*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
			{
				((CNezuko*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
			{
				((CShinobu*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}

		}
		else if (_iIndex == 1)
		{
			if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
			{
				((CKyoujuro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
			{
				((CTanjiro*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
			{
				((CRui*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
			{
				((CAkaza*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
			{
				((CNezuko*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
			{
				((CShinobu*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"엔무")
			{
				((CEnmu*)(m_vecObjList[_iIndex]))->Set_ToolState(selected, 0, 0, 0, false);
			}
		}
	}




	if (ImGui::Button("Add Animation"))
	{
		if (_iIndex == 0)
		{
			if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
			{
				m_vecAnimIndex.push_back(selected);
			}
		}
		else if (_iIndex == 1)
		{
			if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
			{
				m_vecAnimIndex.push_back(selected);
			}
			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
			{
				m_vecAnimIndex.push_back(selected);
			}

			else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"엔무")
			{
				m_vecAnimIndex.push_back(selected);
			}
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Clear Animation"))
	{
		m_vecAnimIndex.clear();
	}


	if (ImGui::Button("Play All Animation"))
	{
		if (!m_vecAnimIndex.empty())
		{
			size_t iIndexSize = m_vecAnimIndex.size();

			if (_iIndex == 0)
			{
				if (iIndexSize < 3)
				{
					m_vecAnimIndex.push_back(-1);
					m_vecAnimIndex.push_back(-1);
					m_vecAnimIndex.push_back(-1);
				}

				if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
				{
					((CTanjiro*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
				{
					((CKyoujuro*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
				{
					((CRui*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
				{
					((CAkaza*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
				{
					((CNezuko*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
				{
					((CShinobu*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
			}
			else if (_iIndex == 1)
			{

				if (iIndexSize < 3)
				{
					m_vecAnimIndex.push_back(-1);
					m_vecAnimIndex.push_back(-1);
					m_vecAnimIndex.push_back(-1);
				}

				if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
				{
					((CKyoujuro*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
				{
					((CTanjiro*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
				{
					((CRui*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
				{
					((CAkaza*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
				{
					((CNezuko*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
				{
					((CShinobu*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}

				else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"엔무")
				{
					((CEnmu*)(m_vecObjList[_iIndex]))->
						Set_ToolState(m_vecAnimIndex[0], m_vecAnimIndex[1], m_vecAnimIndex[2], 0, true);
				}
			}
		}
	}


	Character_Compare_Duration(_iIndex);
	Character_Compare_Frame(_iIndex);

	static _float fSetDuration = 0.f;
	if (ImGui::CollapsingHeader("Duration Set"))
	{
		ImGui::DragFloat("Set Duration : ", &fSetDuration, 0.f, m_fDuration);
	}

	if (ImGui::SliderFloat("Duration : ", &m_fCurrentDuration, 0.f, m_fDuration))
	{
		Character_Set_Duration(_iIndex);
	}
	else if (0 != fSetDuration)
	{
		m_fCurrentDuration = fSetDuration;
		Character_Set_Duration(0);
		Character_Set_Duration(1);
	}

	ImGui::SameLine();
	ImGui::Text("%f", m_fDuration);

	if (ImGui::SliderInt("Frame : ", &m_iCurrentFrame, 0, m_iFrame))
	{
		Character_Set_Frame(_iIndex);
	}
	ImGui::SameLine();
	ImGui::Text("%d", m_iFrame);


	ImGui::Text("%f %f %f", XMVectorGetX(m_vecObjList[0]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)), XMVectorGetY(m_vecObjList[0]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)), XMVectorGetZ(m_vecObjList[0]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	ImGui::Text("%f %f %f", XMVectorGetX(m_vecObjList[1]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)), XMVectorGetY(m_vecObjList[1]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)), XMVectorGetZ(m_vecObjList[1]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));


}


void CImGuiManager::Clear_CharacterList()
{
	for (auto& iter : m_vecObjList)
		Safe_Release(iter);

	m_vecObjList.clear();
}

void CImGuiManager::Clear_AnimationList()
{
	for (auto& iter : m_vecAnimation)
		Safe_Release(iter);

	m_vecAnimation.clear();
}

void CImGuiManager::Character_Compare_Duration(_uint _iIndex)
{
	if (_iIndex == 0)
	{
		if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
		{
			_uint iAnimIndex = ((CTanjiro*)(m_vecObjList[0]))->Get_AnimIndex();

			m_fCurrentDuration = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
		{
			_uint iAnimIndex = ((CKyoujuro*)(m_vecObjList[0]))->Get_AnimIndex();

			m_fCurrentDuration = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
		{
			_uint iAnimIndex = ((CRui*)(m_vecObjList[0]))->Get_AnimIndex();

			m_fCurrentDuration = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
		{
			_uint iAnimIndex = ((CAkaza*)(m_vecObjList[0]))->Get_AnimIndex();

			m_fCurrentDuration = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[0]))->Get_AnimIndex();

			m_fCurrentDuration = ((CNezuko*)(m_vecObjList[0]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CNezuko*)(m_vecObjList[0]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
		{
			_uint iAnimIndex = ((CShinobu*)(m_vecObjList[0]))->Get_AnimIndex();

			m_fCurrentDuration = ((CShinobu*)(m_vecObjList[0]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CShinobu*)(m_vecObjList[0]))->Get_Model()->Get_Duration();
		}


	}
	else if (_iIndex == 1)
	{
		if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
		{
			_uint iAnimIndex = ((CKyoujuro*)(m_vecObjList[1]))->Get_AnimIndex();

			m_fCurrentDuration = ((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
		{
			_uint iAnimIndex = ((CTanjiro*)(m_vecObjList[1]))->Get_AnimIndex();

			m_fCurrentDuration = ((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
		{
			_uint iAnimIndex = ((CRui*)(m_vecObjList[1]))->Get_AnimIndex();

			m_fCurrentDuration = ((CRui*)(m_vecObjList[1]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CRui*)(m_vecObjList[1]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
		{
			_uint iAnimIndex = ((CAkaza*)(m_vecObjList[1]))->Get_AnimIndex();

			m_fCurrentDuration = ((CAkaza*)(m_vecObjList[1]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CAkaza*)(m_vecObjList[1]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[1]))->Get_AnimIndex();

			m_fCurrentDuration = ((CNezuko*)(m_vecObjList[1]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CNezuko*)(m_vecObjList[1]))->Get_Model()->Get_Duration();
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
		{
			_uint iAnimIndex = ((CShinobu*)(m_vecObjList[1]))->Get_AnimIndex();

			m_fCurrentDuration = ((CShinobu*)(m_vecObjList[1]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CShinobu*)(m_vecObjList[1]))->Get_Model()->Get_Duration();
		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"엔무")
		{
			_uint iAnimIndex = ((CEnmu*)(m_vecObjList[1]))->Get_AnimIndex();

			m_fCurrentDuration = ((CEnmu*)(m_vecObjList[1]))->Get_Model()->Get_CurrentTime();
			m_fDuration = ((CEnmu*)(m_vecObjList[1]))->Get_Model()->Get_Duration();
		}
	}
}

void CImGuiManager::Character_Set_Duration(_uint _iIndex)
{
	if (_iIndex == 0)
	{
		if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
		{
			_uint iAnimIndex = ((CTanjiro*)(m_vecObjList[0]))->Get_AnimIndex();

			((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);

		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
		{
			_uint iAnimIndex = ((CKyoujuro*)(m_vecObjList[0]))->Get_AnimIndex();

			((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
		{
			_uint iAnimIndex = ((CRui*)(m_vecObjList[0]))->Get_AnimIndex();

			((CRui*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CRui*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
		{
			_uint iAnimIndex = ((CAkaza*)(m_vecObjList[0]))->Get_AnimIndex();

			((CAkaza*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CAkaza*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[0]))->Get_AnimIndex();

			((CNezuko*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CNezuko*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}

		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[0]))->Get_AnimIndex();

			((CShinobu*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CShinobu*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}

	}
	else if (_iIndex == 1)
	{
		if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
		{
			_uint iAnimIndex = ((CKyoujuro*)(m_vecObjList[1]))->Get_AnimIndex();

			((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
		{
			_uint iAnimIndex = ((CTanjiro*)(m_vecObjList[1]))->Get_AnimIndex();

			((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
		{
			_uint iAnimIndex = ((CRui*)(m_vecObjList[1]))->Get_AnimIndex();

			((CRui*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CRui*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
		{
			_uint iAnimIndex = ((CAkaza*)(m_vecObjList[1]))->Get_AnimIndex();

			((CAkaza*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CAkaza*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[1]))->Get_AnimIndex();

			((CNezuko*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CNezuko*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[1]))->Get_AnimIndex();

			((CShinobu*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CShinobu*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"엔무")
		{
			_uint iAnimIndex = ((CEnmu*)(m_vecObjList[1]))->Get_AnimIndex();

			((CEnmu*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CEnmu*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fCurrentDuration);
		}
	}


}

void CImGuiManager::Character_Compare_Frame(_uint _iIndex)
{
	if (_iIndex == 0)
	{
		if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
		{
			_uint iAnimIndex = ((CTanjiro*)(m_vecObjList[0]))->Get_AnimIndex();

			m_iCurrentFrame = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
		{
			_uint iAnimIndex = ((CKyoujuro*)(m_vecObjList[0]))->Get_AnimIndex();

			m_iCurrentFrame = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
		{
			_uint iAnimIndex = ((CRui*)(m_vecObjList[0]))->Get_AnimIndex();

			m_iCurrentFrame = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
		{
			_uint iAnimIndex = ((CAkaza*)(m_vecObjList[0]))->Get_AnimIndex();

			m_iCurrentFrame = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[0]))->Get_AnimIndex();

			m_iCurrentFrame = ((CNezuko*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CNezuko*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}

		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
		{
			_uint iAnimIndex = ((CShinobu*)(m_vecObjList[0]))->Get_AnimIndex();

			m_iCurrentFrame = ((CShinobu*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CShinobu*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}


	}
	else if (_iIndex == 1)
	{
		if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
		{
			_uint iAnimIndex = ((CKyoujuro*)(m_vecObjList[1]))->Get_AnimIndex();

			m_iCurrentFrame = ((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
		{
			_uint iAnimIndex = ((CTanjiro*)(m_vecObjList[1]))->Get_AnimIndex();

			m_iCurrentFrame = ((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
		{
			_uint iAnimIndex = ((CRui*)(m_vecObjList[1]))->Get_AnimIndex();

			m_iCurrentFrame = ((CRui*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
		{
			_uint iAnimIndex = ((CAkaza*)(m_vecObjList[1]))->Get_AnimIndex();

			m_iCurrentFrame = ((CAkaza*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_AllFrame();

		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[1]))->Get_AnimIndex();

			m_iCurrentFrame = ((CNezuko*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CNezuko*)(m_vecObjList[1]))->Get_Model()->Get_AllFrame();

		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
		{
			_uint iAnimIndex = ((CShinobu*)(m_vecObjList[1]))->Get_AnimIndex();

			m_iCurrentFrame = ((CShinobu*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CShinobu*)(m_vecObjList[1]))->Get_Model()->Get_AllFrame();
		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"엔무")
		{
			_uint iAnimIndex = ((CEnmu*)(m_vecObjList[1]))->Get_AnimIndex();

			m_iCurrentFrame = ((CEnmu*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();
			m_iFrame = ((CEnmu*)(m_vecObjList[1]))->Get_Model()->Get_AllFrame();
		}
	}


}

void CImGuiManager::Character_Set_Frame(_uint _iIndex)
{

	if (_iIndex == 0)
	{
		if (m_vecObjList[0]->Get_PlayerInfo().strName == L"탄지로")
		{
			_uint iAnimIndex = ((CTanjiro*)(m_vecObjList[0]))->Get_AnimIndex();

			int iFrame = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CTanjiro*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);

		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"쿄주로")
		{
			_uint iAnimIndex = ((CKyoujuro*)(m_vecObjList[0]))->Get_AnimIndex();

			int iFrame = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"루이")
		{
			_uint iAnimIndex = ((CRui*)(m_vecObjList[0]))->Get_AnimIndex();

			int iFrame = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CRui*)(m_vecObjList[0]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CRui*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CRui*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}
		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"아카자")
		{
			_uint iAnimIndex = ((CAkaza*)(m_vecObjList[0]))->Get_AnimIndex();

			int iFrame = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CAkaza*)(m_vecObjList[0]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CAkaza*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CAkaza*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}

		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"네즈코")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[0]))->Get_AnimIndex();

			int iFrame = ((CNezuko*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CNezuko*)(m_vecObjList[0]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CNezuko*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CNezuko*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}


		else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"시노부")
		{
			_uint iAnimIndex = ((CShinobu*)(m_vecObjList[0]))->Get_AnimIndex();

			int iFrame = ((CShinobu*)(m_vecObjList[0]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CShinobu*)(m_vecObjList[0]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CShinobu*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CShinobu*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}


	}
	else if (_iIndex == 1)
	{
		if (m_vecObjList[1]->Get_PlayerInfo().strName == L"쿄주로")
		{
			_uint iAnimIndex = ((CKyoujuro*)(m_vecObjList[1]))->Get_AnimIndex();

			int iFrame = ((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CKyoujuro*)(m_vecObjList[1]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CKyoujuro*)(m_vecObjList[0]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"탄지로")
		{
			_uint iAnimIndex = ((CTanjiro*)(m_vecObjList[1]))->Get_AnimIndex();

			int iFrame = ((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CTanjiro*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"루이")
		{
			_uint iAnimIndex = ((CRui*)(m_vecObjList[1]))->Get_AnimIndex();

			int iFrame = ((CRui*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CRui*)(m_vecObjList[1]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CRui*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CRui*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}
		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"아카자")
		{
			_uint iAnimIndex = ((CAkaza*)(m_vecObjList[1]))->Get_AnimIndex();

			int iFrame = ((CAkaza*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CAkaza*)(m_vecObjList[1]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CAkaza*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CAkaza*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"네즈코")
		{
			_uint iAnimIndex = ((CNezuko*)(m_vecObjList[1]))->Get_AnimIndex();

			int iFrame = ((CNezuko*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CNezuko*)(m_vecObjList[1]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CNezuko*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CNezuko*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}

		else if (m_vecObjList[1]->Get_PlayerInfo().strName == L"시노부")
		{
			_uint iAnimIndex = ((CShinobu*)(m_vecObjList[1]))->Get_AnimIndex();

			int iFrame = ((CShinobu*)(m_vecObjList[1]))->Get_Model()->Get_CurrentFrame();

			_float fCurDuration = ((CShinobu*)(m_vecObjList[1]))->Get_Model()->Get_Duration();

			m_fSettingFrameTime = (m_iCurrentFrame + 1) * ((fCurDuration) / m_iFrame);

			((CShinobu*)(m_vecObjList[1]))->Get_Model()->Reset_Anim(iAnimIndex);
			((CShinobu*)(m_vecObjList[1]))->Get_Model()->Set_CurrentTime(m_fSettingFrameTime);
		}
	}
}

void CImGuiManager::EnmuBossAnimDebug()
{
	ImVec2 vListSize(100, 60);
	ImVec2 vObjSize(100, 20);
	static int selected = 999;


	if (ImGui::BeginListBox("Character", vListSize))
	{
		for (_uint i = 0; i < m_vecObjList.size(); ++i)
		{
			wstring wStrName = m_vecObjList[i]->Get_PlayerInfo().strName;
			string strName;


			// 보스
			if (wStrName == L"엔무(각성)")
				strName = "Head";
			else if (wStrName == L"엔무(쉴드)")
				strName = "Shield";
			else if (wStrName == L"엔무_오른손")
				strName = "RightHand";
			else if (wStrName == L"엔무_왼손")
				strName = "LeftHand";
			else if (wStrName == L"엔무_촉수")
				strName = "CHOKCHOK";

			if (ImGui::Selectable(strName.c_str(), selected == i, 0, vObjSize))
			{
				selected = i;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}




	if (selected != 999)
		EnmuBossCharacterList(selected);

}

void CImGuiManager::EnmuBossCharacterList(_uint _iIndex)
{
	ImVec2 vListSize(300, 200);
	ImVec2 vObjSize(300, 15);
	static int selected = 0;


	if (m_vecObjList[0]->Get_PlayerInfo().strName == L"엔무(각성)")
	{
		m_vecAnimation = ((CEnmu_Chaos_Head*)(m_vecObjList[_iIndex]))->Get_Model()->Get_Animation();
	}
	else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"엔무(쉴드)")
	{
		m_vecAnimation = ((CEnmu_Shield*)(m_vecObjList[_iIndex]))->Get_Model()->Get_Animation();
	}
	else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"엔무_오른손")
	{
		m_vecAnimation = ((CEnmu_Right_Hand*)(m_vecObjList[_iIndex]))->Get_Model()->Get_Animation();
	}
	else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"엔무_왼손")
	{
		m_vecAnimation = ((CEnmu_Left_Hand*)(m_vecObjList[_iIndex]))->Get_Model()->Get_Animation();
	}
	else if (m_vecObjList[0]->Get_PlayerInfo().strName == L"엔무_촉수")
	{
		m_vecAnimation = ((CEnmu_Chok*)(m_vecObjList[_iIndex]))->Get_Model()->Get_Animation();
	}

	if (ImGui::BeginListBox("..", vListSize))
	{
		for (_uint i = 0; i < m_vecAnimation.size(); ++i)
		{
			string Temp = to_string(i) + ". ";
			string strName = m_vecAnimation[i]->Get_AnimName();


			strName.erase(strName.begin(), strName.begin() + 23);
			strName = Temp + strName;
			if (ImGui::Selectable(strName.c_str(), selected == i, 0, vObjSize))
			{
				selected = i;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}



	if (ImGui::Button("Play Animation"))
	{
		CEnmuBoss::Get_Instance()->Set_ToolState(_iIndex, selected, 0, 0, 0, false);
	}

	m_fCurrentDuration = m_vecObjList[_iIndex]->Get_Model()->Get_CurrentTime_Index(selected);
	m_fDuration = m_vecObjList[_iIndex]->Get_Model()->Get_Duration_Index(selected);

	if (ImGui::SliderFloat("Duration : ", &m_fCurrentDuration, 0.f, m_fDuration))
	{
		
	}

	ImGui::Text("distance : %f", m_fEnmuBossDist);
	ImGui::Text("campos : x: %f y: %f z : %f", m_vCameraPosition.x, m_vCameraPosition.y, m_vCameraPosition.z);



	static float vjumpset[3];
	ImGui::InputFloat3("jumpset", vjumpset);
	ImGui::SameLine();
	if (ImGui::Button("JumpSetButton"))
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, L"Layer_Tanjiro")->Get_LayerFront())->Set_JumpState(vjumpset[0], vjumpset[1], vjumpset[2]);
	}
}

void CImGuiManager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pRendererCom);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pCamera);

	for (_int j = 0; j < CAM_END; ++j)
	{
		for (auto & iter : m_vecCamObjects[j])
		{
			iter->Set_Dead();
			Safe_Release(iter);
		}
		m_vecCamObjects[j].clear();
	}

	for (_uint i = 0; i < 250; ++i)
	{
		Safe_Delete_Array(m_strCamFiles[i]);
	}

	//	Clear_CharacterList();
	//	Clear_AnimationList();

}
