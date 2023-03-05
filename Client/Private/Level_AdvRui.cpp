#include "stdafx.h"
#include "Level_AdvRui.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "GameObj.h"
#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"
#include "Terrain.h"
#include "MeshObj_Smell.h"
#include "MeshObj_Smell_Inst.h"
#include "Tanjiro.h"
#include "Characters.h"
#include "ImGuiManager.h"
#include "SoundMgr.h"

unsigned int APIENTRY Thread_AdvRui(void* pArg)
{
	CLevel_AdvRui*		pLoader = (CLevel_AdvRui*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	pLoader->Ready_Lights();
	g_fLoading = 10.f;
	pLoader->Ready_Layer_Camera(TEXT("Layer_Camera"));
	g_fLoading = 20.f;
	pLoader->Ready_Layer_Player(TEXT("Layer_Player"));
	g_fLoading = 40.f;
	pLoader->Ready_Layer_BackGround(TEXT("Layer_BackGround"));
	g_fLoading = 50.f;
	pLoader->Load_StaticObjects("RuiStory");
	g_fLoading = 70.f;
	pLoader->Load_Weed("Weed");
	g_fLoading = 80.f;
	pLoader->Load_Smell_1("RuiSmell1");
	g_fLoading = 100.f;
	CComponent* pOut = pGameInstance->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Renderer");
	pLoader->Set_Renderer((CRenderer*)pOut);


	_float fValue[CRenderer::VALUE_END] = { 0.07f, 0.12f, 0.1f, 55.f, 80.f, 0.83f, 0.7f, 1.36f, 0.4f, 1.f, 20.f, 100.f, 0.05f, 2.2f, 0.1f, 0.85f, 1.f };

	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_R), 0.07f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_G), 0.12f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_B), 0.1f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGDISTANCE), 55.f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGRANGE), 80.f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGMINPOWER), 0.83f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_CUBEMAPFOG), 0.7f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_AO), 1.36f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_AORADIUS), 0.4f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_GLOWBLURCOUNT), 1.f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_DISTORTION), 20.f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_OUTLINE), 100.f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_INNERLINE), 0.05f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_ENVLIGHT), 2.2f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT), 0.1f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTPOWER), 0.85f);
	pLoader->Get_Renderer()->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_SHADOWTESTLENGTH), 1.f);
	pLoader->Get_Renderer()->Set_Far(g_fFar);

	for (_int i = 0; i < CRenderer::VALUE_END; ++i)
		CImGuiManager::Get_Instance()->Setting_PostProcessingValue(i, fValue[i]);

	RELEASE_INSTANCE(CGameInstance);

	LeaveCriticalSection(&pLoader->Get_CriticalSection());
	pLoader->Set_Finished();
	g_bThread = false;
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Set_LoadingDead();
	
	if (!pUIManager->Get_SaveStory())
	{
		CSoundMgr::Get_Instance()->BGM_Stop();
		CSoundMgr::Get_Instance()->PlayBGM(TEXT("Adv_Rui.wav"), g_fBGM);
	}
	else
	{
		CSoundMgr::Get_Instance()->BGM_Stop();
		CSoundMgr::Get_Instance()->PlayBGM(TEXT("Adv_Rui2.wav"), g_fBGM);
	}
	RELEASE_INSTANCE(CUI_Manager);
	return 0;
}
CLevel_AdvRui::CLevel_AdvRui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_AdvRui::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	g_bThread = true;
	g_iLevel = LEVEL_ADVRUI;

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	pUI_Manager->Add_Loading();

	CoInitializeEx(nullptr, 0);

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_AdvRui, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	g_fFar = 1800.f;

	pUI_Manager->Add_Quiest();
	RELEASE_INSTANCE(CUI_Manager);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_AdvRui::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (m_isFinished)
	{
		if (!m_bTread)
		{
			WaitForSingleObject(m_hThread, INFINITE);

			CloseHandle(m_hThread);

			DeleteCriticalSection(&m_CriticalSection);
			m_bTread = true;
		}

		Create_Stone(fTimeDelta);

		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

		switch (pUIManager->Get_RescueCount())
		{
		case 1:
			if (!m_bRescue[0])
			{
				++m_iQuestIndex;
				m_bRescue[0] = true;
			}
			break;
		case 2:
			if (!m_bRescue[1])
			{
				++m_iQuestIndex;
				m_bRescue[1] = true;
			}
			break;
		default:
			break;
		}

		if (dynamic_cast<CTanjiro*>(m_pPlayer)->Get_Quest2MSG())
		{
			_float4 vPos;
			XMStoreFloat4(&vPos, m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			pUIManager->Set_Sel1P(0);
			pUIManager->Set_Sel1P_2(4);
			pUIManager->Set_Sel2P(6);
			pUIManager->Set_Sel2P_2(99);
			pUIManager->Set_SelMapNum(0);
			pUIManager->Set_PlayerPos(vPos);
			pUIManager->Set_RuiDadBattle(true);
			if (FAILED(pGameInstance->Open_Level(LEVEL_GAMEPLAY, CLevel_GamePlay::Create(m_pDevice, m_pContext))))
				return;

			RELEASE_INSTANCE(CUI_Manager);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		if (dynamic_cast<CTanjiro*>(m_pPlayer)->Get_Quest3MSG())
		{
			pUIManager->Set_Sel1P(0);
			pUIManager->Set_Sel1P_2(4);
			pUIManager->Set_Sel2P(7);
			pUIManager->Set_Sel2P_2(99);
			pUIManager->Set_SelMapNum(0);
			if (FAILED(pGameInstance->Open_Level(LEVEL_GAMEPLAY, CLevel_GamePlay::Create(m_pDevice, m_pContext))))
				return;

			RELEASE_INSTANCE(CUI_Manager);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}

		RELEASE_INSTANCE(CUI_Manager);
		RELEASE_INSTANCE(CGameInstance);
		if (!m_bQuest[0] || !m_bQuest[1] || !m_bQuest[2])
			Check_Smell();
	}
}

void CLevel_AdvRui::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_AdvRui::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->LightClear();

	LIGHTDESC			LightDesc;

	/* For.Point */
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_RUISHADOW;
	LightDesc.vDirection = _float4(-10.f, 150.f, -10.f, 1.f);
	LightDesc.vDiffuse = _float4(60.f, -20.f, 60.f, 1.f);

	LightDesc.vAmbient = _float4(0.f, 0.1f, 0.f, 0.f);

	const LIGHTDESC* pLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW);
	if (nullptr == pLightDesc)
	{
		if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
			return E_FAIL;
	}
	else
	{
		pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW, LightDesc.vDirection, LightDesc.vDiffuse);
	}


	_vector vLook = XMLoadFloat4(&LightDesc.vDiffuse) - XMLoadFloat4(&LightDesc.vDirection);

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	XMStoreFloat4(&LightDesc.vDirection, XMVector3Normalize(vLook));
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	pLightDesc = pGameInstance->Get_LightDesc(LIGHTDESC::TYPE_DIRECTIONAL);
	if (nullptr == pLightDesc)
	{
		if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
			return E_FAIL;
	}
	else
	{
		pGameInstance->Set_LightDesc(LIGHTDESC::TYPE_DIRECTIONAL, LightDesc);
	}



	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_BATTLESHADOW;
	LightDesc.vDirection = _float4(-1020.f, 1250.f, -570.f, 1.f);		//	eye
	XMStoreFloat4(&LightDesc.vDiffuse, XMVectorSetW(XMLoadFloat4(&LightDesc.vDirection) + XMVector3Normalize(vLook), 1.f));
	LightDesc.vAmbient = _float4(0.f, 0.1f, 0.f, 0.f);

	pLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW);
	if (nullptr == pLightDesc)
	{
		if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
			return E_FAIL;
	}
	else
	{
		pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW, LightDesc.vDirection, LightDesc.vDiffuse);
	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc1p;

	tCharacterDesc1p.i1P2P = 10;
	tCharacterDesc1p.bSub = false;
	tCharacterDesc.i1P2P = 10;
	tCharacterDesc.bSub = false;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_ADVRUI, TEXT("Layer_Tanjiro"), &tCharacterDesc)))
		return E_FAIL;
	tCharacterDesc1p.pSubChar = tCharacterDesc.pSubChar;
	m_pPlayer = tCharacterDesc.pSubChar;

	if (!CUI_Manager::Get_Instance()->Get_SaveStory())
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiDad"), LEVEL_ADVRUI, TEXT("Layer_RuiDad"), &tCharacterDesc1p)))
			return E_FAIL;
	}
	else
	{ 
		++m_iQuestIndex;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_ADVRUI, TEXT("Layer_Rui"), &tCharacterDesc1p)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiSister"), LEVEL_ADVRUI, TEXT("Layer_RuiSister"), &tCharacterDesc1p)))
			return E_FAIL;
		
	}

	//tCharacterDesc1p.bSub = true;
	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_ADVRUI, TEXT("Layer_Nezuko"), &tCharacterDesc1p)))
	//	return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(25.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = g_fFar;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_ADVRUI, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	return S_OK;
}
HRESULT CLevel_AdvRui::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkyBox"), LEVEL_ADVRUI, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_AdvRui::Load_StaticObjects(char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/StaticObjs/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".stobj");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to GamePlay : Load StaticObjs");

		return E_FAIL;
	}

	DWORD		dwByte = 0;
	_float4x4*	pWorld = new _float4x4;
	_int*		pMeshIndex = new _int;
	_float*		pGlowPower = new _float;
	_bool*		pAlphaBlend = new _bool;

	struct MAPOBJDESC
	{
		_float4x4		matWorld;
		_float			fGlowPower;
		_bool			bAlphaBlend;
	};

	//	<갯수 <메쉬넘버, 월드>>
	map<_uint, vector<MAPOBJDESC>>			map_MeshIdx_MapObjDesc;
	map<_uint, _uint>						map_MeshIdx_Num;

	while (true)
	{
		ReadFile(hFile, pMeshIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pGlowPower, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, pWorld, sizeof(_float4x4), &dwByte, nullptr);
		ReadFile(hFile, pAlphaBlend, sizeof(_bool), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pWorld);
			Safe_Delete(pMeshIndex);
			Safe_Delete(pGlowPower);
			Safe_Delete(pAlphaBlend);
			break;
		}


		MAPOBJDESC	tMapObjDesc;
		tMapObjDesc.matWorld = *pWorld;
		tMapObjDesc.fGlowPower = *pGlowPower;
		tMapObjDesc.bAlphaBlend = *pAlphaBlend;

		_uint	iCount = (_uint)map_MeshIdx_MapObjDesc.count(*pMeshIndex);
		if (!iCount)
		{
			std::vector<MAPOBJDESC>		vecWorld;
			vecWorld.push_back(tMapObjDesc);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecWorld);
		}
		else
		{
			auto Pair = map_MeshIdx_MapObjDesc.find(*pMeshIndex);
			std::vector<MAPOBJDESC>		vecObjDesc = Pair->second;
			vecObjDesc.push_back(tMapObjDesc);

			map_MeshIdx_MapObjDesc.erase(Pair);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecObjDesc);
		}

		iCount = (_uint)map_MeshIdx_Num.count(*pMeshIndex);
		if (!iCount)
			map_MeshIdx_Num.emplace(*pMeshIndex, 1);
		else
		{
			auto Pair = map_MeshIdx_Num.find(*pMeshIndex);
			_uint iNumInstancing = Pair->second;

			map_MeshIdx_Num.erase(Pair);
			map_MeshIdx_Num.emplace(*pMeshIndex, ++iNumInstancing);
		}


		/*
		CMeshObj_Static::MESHOBJ_STATIC_DESC tMeshObj_StaticDesc;
		tMeshObj_StaticDesc.matWorld = *pWorld;
		tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ADVRUI;
		tMeshObj_StaticDesc.iModelIndex = *pMeshIndex;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_ADVRUI, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
		{
		ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
		break;
		}
		*/
	}

	for (auto & Pair : map_MeshIdx_Num)
	{
		if (4 <= Pair.second)
		{
			_uint iNumInstancing = Pair.second;
			CMeshObj_Static_Inst::MESHOBJ_STATIC_INSTANCING_DESC tMeshObj_Static_InstDesc;
			_float4x4*		arrWorld = new _float4x4[iNumInstancing];
			_float*			arrGlowPower = new _float[iNumInstancing];
			for (_uint i = 0; i < iNumInstancing; ++i)
			{
				arrWorld[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].matWorld;
				arrGlowPower[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].fGlowPower;
			}

			tMeshObj_Static_InstDesc.pWorld = arrWorld;
			tMeshObj_Static_InstDesc.pGlowPower = arrGlowPower;
			tMeshObj_Static_InstDesc.iCurrentLevel = LEVEL_ADVRUI;
			tMeshObj_Static_InstDesc.iModelIndex = Pair.first;
			tMeshObj_Static_InstDesc.iNumInstancing = iNumInstancing;

			CLevel_GamePlay::CHARACTERDESC	tCharacterDesc1p;

			switch (tMeshObj_Static_InstDesc.iModelIndex)
			{
			case 2019:
				for (_uint i = 0; i < Pair.second; ++i)
				{
					tCharacterDesc1p.i1P2P = 10;
					tCharacterDesc1p.matWorld = arrWorld[i];
					tCharacterDesc1p.pSubChar = tCharacterDesc.pSubChar;
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Butterfly"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
				}
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2020:
				for (_uint i = 0; i < Pair.second; ++i)
				{
					tCharacterDesc1p.i1P2P = 10;
					tCharacterDesc1p.matWorld = arrWorld[i];
					tCharacterDesc1p.pSubChar = tCharacterDesc.pSubChar;
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Hoke"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
				}
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2021:
				for (_uint i = 0; i < Pair.second; ++i)
				{
					if (i == 0)
						continue;
					tCharacterDesc1p.i1P2P = 10;
					tCharacterDesc1p.matWorld = arrWorld[i];
					tCharacterDesc1p.pSubChar = tCharacterDesc.pSubChar;
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Deer"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
				}
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2022:
				for (_uint i = 0; i < Pair.second; ++i)
				{
					tCharacterDesc1p.i1P2P = 10;
					tCharacterDesc1p.matWorld = arrWorld[i];
					tCharacterDesc1p.pSubChar = tCharacterDesc.pSubChar;
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Deer2"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
				}
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2026:
				for (_uint i = 0; i < Pair.second; ++i)
				{
					tCharacterDesc1p.i1P2P = 10;
					tCharacterDesc1p.matWorld = arrWorld[i];
					tCharacterDesc1p.pSubChar = tCharacterDesc.pSubChar;
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BigBird"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
				}
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2031:
				for (_uint i = 0; i < Pair.second; ++i)
				{
					tCharacterDesc1p.i1P2P = 10;
					tCharacterDesc1p.matWorld = arrWorld[i];
					tCharacterDesc1p.pSubChar = tCharacterDesc.pSubChar;
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Bird"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
				}
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2041:
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2042:
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2043:
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			default:
				break;
			}

			if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
			{
				ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
				continue;
			}

			Safe_Delete_Array(arrWorld);
			Safe_Delete_Array(arrGlowPower);
		}
		else
		{
			for (_uint i = 0; i < Pair.second; ++i)
			{
				auto Pair_World = map_MeshIdx_MapObjDesc.find(Pair.first);
				std::vector<MAPOBJDESC>		vecObjDesc = Pair_World->second;

				_uint	iCount = (_uint)vecObjDesc.size();
				MAPOBJDESC	tMapObjDesc = vecObjDesc[iCount - 1];
				vecObjDesc.pop_back();

				map_MeshIdx_MapObjDesc.erase(Pair_World);
				map_MeshIdx_MapObjDesc.emplace(Pair.first, vecObjDesc);


				CMeshObj_Static::MESHOBJ_STATIC_DESC tMeshObj_StaticDesc;
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ADVRUI;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

				CLevel_GamePlay::CHARACTERDESC	tCharacterDesc1p;

				tCharacterDesc1p.i1P2P = 10;
				tCharacterDesc1p.matWorld = tMapObjDesc.matWorld;
				tCharacterDesc1p.pSubChar = tCharacterDesc.pSubChar;
				switch (tMeshObj_StaticDesc.iModelIndex)
				{
				case 2019:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Butterfly"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				case 2020:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Hoke"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				case 2021:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Deer"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				case 2022:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Deer2"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				case 2026:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BigBird"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				case 2031:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Bird"), LEVEL_ADVRUI, TEXT("Layer_Ani"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				case 2041:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Murata"), LEVEL_ADVRUI, TEXT("Layer_NPC"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				case 2042:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Ozaki"), LEVEL_ADVRUI, TEXT("Layer_NPC"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				case 2043:
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Goto"), LEVEL_ADVRUI, TEXT("Layer_NPC"), &tCharacterDesc1p)))
						return E_FAIL;
					continue;
					break;
				default:
					break;
				}

				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_ADVRUI, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
				{
					ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
					continue;
				}
			}
		}
	}

	CloseHandle(hFile);

	Safe_Delete(pWorld);
	Safe_Delete(pMeshIndex);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
HRESULT CLevel_AdvRui::Load_Map(const _tchar* pLayerTag, char * pFileName)
{


	return S_OK;
}

HRESULT CLevel_AdvRui::Load_Weed(char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/StaticObjs/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".stobj");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to GamePlay : Load Weed");

		return E_FAIL;
	}

	DWORD		dwByte = 0;
	_float4x4*	pWorld = new _float4x4;
	_int*		pMeshIndex = new _int;
	_float*		pGlowPower = new _float;
	_bool*		pAlphaBlend = new _bool;

	struct MAPOBJDESC
	{
		_float4x4		matWorld;
		_float			fGlowPower;
		_bool			bAlphaBlend;
	};

	//	<갯수 <메쉬넘버, 월드>>
	map<_uint, vector<MAPOBJDESC>>			map_MeshIdx_MapObjDesc;
	map<_uint, _uint>						map_MeshIdx_Num;

	while (true)
	{
		ReadFile(hFile, pMeshIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pGlowPower, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, pWorld, sizeof(_float4x4), &dwByte, nullptr);
		ReadFile(hFile, pAlphaBlend, sizeof(_bool), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pWorld);
			Safe_Delete(pMeshIndex);
			Safe_Delete(pGlowPower);
			Safe_Delete(pAlphaBlend);
			break;
		}


		MAPOBJDESC	tMapObjDesc;
		tMapObjDesc.matWorld = *pWorld;
		tMapObjDesc.fGlowPower = *pGlowPower;
		tMapObjDesc.bAlphaBlend = *pAlphaBlend;

		_uint	iCount = (_uint)map_MeshIdx_MapObjDesc.count(*pMeshIndex);
		if (!iCount)
		{
			std::vector<MAPOBJDESC>		vecWorld;
			vecWorld.push_back(tMapObjDesc);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecWorld);
		}
		else
		{
			auto Pair = map_MeshIdx_MapObjDesc.find(*pMeshIndex);
			std::vector<MAPOBJDESC>		vecObjDesc = Pair->second;
			vecObjDesc.push_back(tMapObjDesc);

			map_MeshIdx_MapObjDesc.erase(Pair);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecObjDesc);
		}

		iCount = (_uint)map_MeshIdx_Num.count(*pMeshIndex);
		if (!iCount)
			map_MeshIdx_Num.emplace(*pMeshIndex, 1);
		else
		{
			auto Pair = map_MeshIdx_Num.find(*pMeshIndex);
			_uint iNumInstancing = Pair->second;

			map_MeshIdx_Num.erase(Pair);
			map_MeshIdx_Num.emplace(*pMeshIndex, ++iNumInstancing);
		}

	}

	for (auto & Pair : map_MeshIdx_Num)
	{
		if (4 <= Pair.second)
		{
			_uint iNumInstancing = Pair.second;
			CMeshObj_Static_Inst::MESHOBJ_STATIC_INSTANCING_DESC tMeshObj_Static_InstDesc;
			_float4x4*		arrWorld = new _float4x4[iNumInstancing];
			_float*			arrGlowPower = new _float[iNumInstancing];
			for (_uint i = 0; i < iNumInstancing; ++i)
			{
				arrWorld[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].matWorld;
				arrGlowPower[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].fGlowPower;
			}

			tMeshObj_Static_InstDesc.pWorld = arrWorld;
			tMeshObj_Static_InstDesc.pGlowPower = arrGlowPower;
			tMeshObj_Static_InstDesc.iCurrentLevel = LEVEL_ADVRUI;
			tMeshObj_Static_InstDesc.iModelIndex = Pair.first;
			tMeshObj_Static_InstDesc.iNumInstancing = iNumInstancing;

			if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
			{
				ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
				continue;
			}

			Safe_Delete_Array(arrWorld);
			Safe_Delete_Array(arrGlowPower);
		}
		else
		{
			for (_uint i = 0; i < Pair.second; ++i)
			{
				auto Pair_World = map_MeshIdx_MapObjDesc.find(Pair.first);
				std::vector<MAPOBJDESC>		vecObjDesc = Pair_World->second;

				_uint	iCount = (_uint)vecObjDesc.size();
				MAPOBJDESC	tMapObjDesc = vecObjDesc[iCount - 1];
				vecObjDesc.pop_back();

				map_MeshIdx_MapObjDesc.erase(Pair_World);
				map_MeshIdx_MapObjDesc.emplace(Pair.first, vecObjDesc);


				CMeshObj_Static::MESHOBJ_STATIC_DESC tMeshObj_StaticDesc;
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ADVRUI;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_ADVRUI, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
				{
					ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
					continue;
				}
			}
		}
	}

	CloseHandle(hFile);

	Safe_Delete(pWorld);
	Safe_Delete(pMeshIndex);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Load_Smell_1(char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/StaticObjs/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".stobj");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to GamePlay : Load Smell");

		return E_FAIL;
	}

	DWORD		dwByte = 0;
	_float4x4*	pWorld = new _float4x4;
	_int*		pMeshIndex = new _int;
	_float*		pGlowPower = new _float;
	_bool*		pAlphaBlend = new _bool;

	struct MAPOBJDESC
	{
		_float4x4		matWorld;
		_float			fGlowPower;
		_bool			bAlphaBlend;
	};

	//	<갯수 <메쉬넘버, 월드>>
	map<_uint, vector<MAPOBJDESC>>			map_MeshIdx_MapObjDesc;
	map<_uint, _uint>						map_MeshIdx_Num;

	while (true)
	{
		ReadFile(hFile, pMeshIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pGlowPower, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, pWorld, sizeof(_float4x4), &dwByte, nullptr);
		ReadFile(hFile, pAlphaBlend, sizeof(_bool), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pWorld);
			Safe_Delete(pMeshIndex);
			Safe_Delete(pGlowPower);
			Safe_Delete(pAlphaBlend);
			break;
		}


		MAPOBJDESC	tMapObjDesc;
		tMapObjDesc.matWorld = *pWorld;
		tMapObjDesc.fGlowPower = *pGlowPower;
		tMapObjDesc.bAlphaBlend = *pAlphaBlend;

		_uint	iCount = (_uint)map_MeshIdx_MapObjDesc.count(*pMeshIndex);
		if (!iCount)
		{
			std::vector<MAPOBJDESC>		vecWorld;
			vecWorld.push_back(tMapObjDesc);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecWorld);
		}
		else
		{
			auto Pair = map_MeshIdx_MapObjDesc.find(*pMeshIndex);
			std::vector<MAPOBJDESC>		vecObjDesc = Pair->second;
			vecObjDesc.push_back(tMapObjDesc);

			map_MeshIdx_MapObjDesc.erase(Pair);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecObjDesc);
		}

		iCount = (_uint)map_MeshIdx_Num.count(*pMeshIndex);
		if (!iCount)
			map_MeshIdx_Num.emplace(*pMeshIndex, 1);
		else
		{
			auto Pair = map_MeshIdx_Num.find(*pMeshIndex);
			_uint iNumInstancing = Pair->second;

			map_MeshIdx_Num.erase(Pair);
			map_MeshIdx_Num.emplace(*pMeshIndex, ++iNumInstancing);
		}

	}

	for (auto & Pair : map_MeshIdx_Num)
	{
		if (4 <= Pair.second)
		{
			_uint iNumInstancing = Pair.second;

			_float4x4*		arrWorld = new _float4x4[iNumInstancing];
			_float*			arrGlowPower = new _float[iNumInstancing];
			for (_uint i = 0; i < iNumInstancing; ++i)
			{
				arrWorld[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].matWorld;
				arrGlowPower[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].fGlowPower;
			}

			switch (Pair.first)
			{
			case 2087:
				
					tSmellDesc[0][0].pWorld = arrWorld;
					tSmellDesc[0][0].pGlowPower = arrGlowPower;
					tSmellDesc[0][0].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[0][0].iModelIndex = Pair.first;
					tSmellDesc[0][0].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[0][0])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2088:
			
					tSmellDesc[0][1].pWorld = arrWorld;
					tSmellDesc[0][1].pGlowPower = arrGlowPower;
					tSmellDesc[0][1].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[0][1].iModelIndex = Pair.first;
					tSmellDesc[0][1].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[0][1])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2089:
			
					tSmellDesc[0][2].pWorld = arrWorld;
					tSmellDesc[0][2].pGlowPower = arrGlowPower;
					tSmellDesc[0][2].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[0][2].iModelIndex = Pair.first;
					tSmellDesc[0][2].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[0][2])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			default:
				break;
			}


			ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
			continue;


			Safe_Delete_Array(arrWorld);
			Safe_Delete_Array(arrGlowPower);
		}
		else
		{
			for (_uint i = 0; i < Pair.second; ++i)
			{
				auto Pair_World = map_MeshIdx_MapObjDesc.find(Pair.first);
				std::vector<MAPOBJDESC>		vecObjDesc = Pair_World->second;

				_uint	iCount = (_uint)vecObjDesc.size();
				MAPOBJDESC	tMapObjDesc = vecObjDesc[iCount - 1];
				vecObjDesc.pop_back();

				map_MeshIdx_MapObjDesc.erase(Pair_World);
				map_MeshIdx_MapObjDesc.emplace(Pair.first, vecObjDesc);


				CMeshObj_Static::MESHOBJ_STATIC_DESC tMeshObj_StaticDesc;
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ADVRUI;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell", LEVEL_ADVRUI, L"Layer_MeshObj_Smell", &tMeshObj_StaticDesc)))
				{
					ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
					continue;
				}
			}


		}

		

		Safe_Delete(pWorld);
		Safe_Delete(pMeshIndex);
	}
	CloseHandle(hFile);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Load_Smell_2(char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/StaticObjs/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".stobj");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to GamePlay : Load StaticObjs");

		return E_FAIL;
	}

	DWORD		dwByte = 0;
	_float4x4*	pWorld = new _float4x4;
	_int*		pMeshIndex = new _int;
	_float*		pGlowPower = new _float;
	_bool*		pAlphaBlend = new _bool;

	struct MAPOBJDESC
	{
		_float4x4		matWorld;
		_float			fGlowPower;
		_bool			bAlphaBlend;
	};

	//	<갯수 <메쉬넘버, 월드>>
	map<_uint, vector<MAPOBJDESC>>			map_MeshIdx_MapObjDesc;
	map<_uint, _uint>						map_MeshIdx_Num;

	while (true)
	{
		ReadFile(hFile, pMeshIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pGlowPower, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, pWorld, sizeof(_float4x4), &dwByte, nullptr);
		ReadFile(hFile, pAlphaBlend, sizeof(_bool), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pWorld);
			Safe_Delete(pMeshIndex);
			Safe_Delete(pGlowPower);
			Safe_Delete(pAlphaBlend);
			break;
		}


		MAPOBJDESC	tMapObjDesc;
		tMapObjDesc.matWorld = *pWorld;
		tMapObjDesc.fGlowPower = *pGlowPower;
		tMapObjDesc.bAlphaBlend = *pAlphaBlend;

		_uint	iCount = (_uint)map_MeshIdx_MapObjDesc.count(*pMeshIndex);
		if (!iCount)
		{
			std::vector<MAPOBJDESC>		vecWorld;
			vecWorld.push_back(tMapObjDesc);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecWorld);
		}
		else
		{
			auto Pair = map_MeshIdx_MapObjDesc.find(*pMeshIndex);
			std::vector<MAPOBJDESC>		vecObjDesc = Pair->second;
			vecObjDesc.push_back(tMapObjDesc);

			map_MeshIdx_MapObjDesc.erase(Pair);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecObjDesc);
		}

		iCount = (_uint)map_MeshIdx_Num.count(*pMeshIndex);
		if (!iCount)
			map_MeshIdx_Num.emplace(*pMeshIndex, 1);
		else
		{
			auto Pair = map_MeshIdx_Num.find(*pMeshIndex);
			_uint iNumInstancing = Pair->second;

			map_MeshIdx_Num.erase(Pair);
			map_MeshIdx_Num.emplace(*pMeshIndex, ++iNumInstancing);
		}

	}

	for (auto & Pair : map_MeshIdx_Num)
	{
		if (4 <= Pair.second)
		{
			_uint iNumInstancing = Pair.second;

			_float4x4*		arrWorld = new _float4x4[iNumInstancing];
			_float*			arrGlowPower = new _float[iNumInstancing];
			for (_uint i = 0; i < iNumInstancing; ++i)
			{
				arrWorld[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].matWorld;
				arrGlowPower[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].fGlowPower;
			}

			switch (Pair.first)
			{
			case 2087:
			
					tSmellDesc[1][0].pWorld = arrWorld;
					tSmellDesc[1][0].pGlowPower = arrGlowPower;
					tSmellDesc[1][0].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[1][0].iModelIndex = Pair.first;
					tSmellDesc[1][0].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[1][0])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2088:
			
					tSmellDesc[1][1].pWorld = arrWorld;
					tSmellDesc[1][1].pGlowPower = arrGlowPower;
					tSmellDesc[1][1].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[1][1].iModelIndex = Pair.first;
					tSmellDesc[1][1].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[1][1])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2089:
			
					tSmellDesc[1][2].pWorld = arrWorld;
					tSmellDesc[1][2].pGlowPower = arrGlowPower;
					tSmellDesc[1][2].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[1][2].iModelIndex = Pair.first;
					tSmellDesc[1][2].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[1][2])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			default:
				break;
			}


			ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
			continue;


			Safe_Delete_Array(arrWorld);
			Safe_Delete_Array(arrGlowPower);
		}
		else
		{
			for (_uint i = 0; i < Pair.second; ++i)
			{
				auto Pair_World = map_MeshIdx_MapObjDesc.find(Pair.first);
				std::vector<MAPOBJDESC>		vecObjDesc = Pair_World->second;

				_uint	iCount = (_uint)vecObjDesc.size();
				MAPOBJDESC	tMapObjDesc = vecObjDesc[iCount - 1];
				vecObjDesc.pop_back();

				map_MeshIdx_MapObjDesc.erase(Pair_World);
				map_MeshIdx_MapObjDesc.emplace(Pair.first, vecObjDesc);


				CMeshObj_Static::MESHOBJ_STATIC_DESC tMeshObj_StaticDesc;
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ADVRUI;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell", LEVEL_ADVRUI, L"Layer_MeshObj_Smell", &tMeshObj_StaticDesc)))
				{
					ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
					continue;
				}
			}


		}

		CloseHandle(hFile);

		Safe_Delete(pWorld);
		Safe_Delete(pMeshIndex);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Load_Smell_3(char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/StaticObjs/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".stobj");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to GamePlay : Load StaticObjs");

		return E_FAIL;
	}

	DWORD		dwByte = 0;
	_float4x4*	pWorld = new _float4x4;
	_int*		pMeshIndex = new _int;
	_float*		pGlowPower = new _float;
	_bool*		pAlphaBlend = new _bool;

	struct MAPOBJDESC
	{
		_float4x4		matWorld;
		_float			fGlowPower;
		_bool			bAlphaBlend;
	};

	//	<갯수 <메쉬넘버, 월드>>
	map<_uint, vector<MAPOBJDESC>>			map_MeshIdx_MapObjDesc;
	map<_uint, _uint>						map_MeshIdx_Num;

	while (true)
	{
		ReadFile(hFile, pMeshIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pGlowPower, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, pWorld, sizeof(_float4x4), &dwByte, nullptr);
		ReadFile(hFile, pAlphaBlend, sizeof(_bool), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pWorld);
			Safe_Delete(pMeshIndex);
			Safe_Delete(pGlowPower);
			Safe_Delete(pAlphaBlend);
			break;
		}


		MAPOBJDESC	tMapObjDesc;
		tMapObjDesc.matWorld = *pWorld;
		tMapObjDesc.fGlowPower = *pGlowPower;
		tMapObjDesc.bAlphaBlend = *pAlphaBlend;

		_uint	iCount = (_uint)map_MeshIdx_MapObjDesc.count(*pMeshIndex);
		if (!iCount)
		{
			std::vector<MAPOBJDESC>		vecWorld;
			vecWorld.push_back(tMapObjDesc);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecWorld);
		}
		else
		{
			auto Pair = map_MeshIdx_MapObjDesc.find(*pMeshIndex);
			std::vector<MAPOBJDESC>		vecObjDesc = Pair->second;
			vecObjDesc.push_back(tMapObjDesc);

			map_MeshIdx_MapObjDesc.erase(Pair);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecObjDesc);
		}

		iCount = (_uint)map_MeshIdx_Num.count(*pMeshIndex);
		if (!iCount)
			map_MeshIdx_Num.emplace(*pMeshIndex, 1);
		else
		{
			auto Pair = map_MeshIdx_Num.find(*pMeshIndex);
			_uint iNumInstancing = Pair->second;

			map_MeshIdx_Num.erase(Pair);
			map_MeshIdx_Num.emplace(*pMeshIndex, ++iNumInstancing);
		}

	}

	for (auto & Pair : map_MeshIdx_Num)
	{
		if (4 <= Pair.second)
		{
			_uint iNumInstancing = Pair.second;

			_float4x4*		arrWorld = new _float4x4[iNumInstancing];
			_float*			arrGlowPower = new _float[iNumInstancing];
			for (_uint i = 0; i < iNumInstancing; ++i)
			{
				arrWorld[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].matWorld;
				arrGlowPower[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].fGlowPower;
			}

			switch (Pair.first)
			{
			case 2087:
			
					tSmellDesc[2][0].pWorld = arrWorld;
					tSmellDesc[2][0].pGlowPower = arrGlowPower;
					tSmellDesc[2][0].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[2][0].iModelIndex = Pair.first;
					tSmellDesc[2][0].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[2][0])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2088:
			
					tSmellDesc[2][1].pWorld = arrWorld;
					tSmellDesc[2][1].pGlowPower = arrGlowPower;
					tSmellDesc[2][1].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[2][1].iModelIndex = Pair.first;
					tSmellDesc[2][1].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[2][1])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2089:
		
					tSmellDesc[2][2].pWorld = arrWorld;
					tSmellDesc[2][2].pGlowPower = arrGlowPower;
					tSmellDesc[2][2].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[2][2].iModelIndex = Pair.first;
					tSmellDesc[2][2].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[2][2])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			default:
				break;
			}


			ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
			continue;


			Safe_Delete_Array(arrWorld);
			Safe_Delete_Array(arrGlowPower);
		}
		else
		{
			for (_uint i = 0; i < Pair.second; ++i)
			{
				auto Pair_World = map_MeshIdx_MapObjDesc.find(Pair.first);
				std::vector<MAPOBJDESC>		vecObjDesc = Pair_World->second;

				_uint	iCount = (_uint)vecObjDesc.size();
				MAPOBJDESC	tMapObjDesc = vecObjDesc[iCount - 1];
				vecObjDesc.pop_back();

				map_MeshIdx_MapObjDesc.erase(Pair_World);
				map_MeshIdx_MapObjDesc.emplace(Pair.first, vecObjDesc);


				CMeshObj_Static::MESHOBJ_STATIC_DESC tMeshObj_StaticDesc;
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ADVRUI;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell", LEVEL_ADVRUI, L"Layer_MeshObj_Smell", &tMeshObj_StaticDesc)))
				{
					ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
					continue;
				}
			}


		}

		CloseHandle(hFile);

		Safe_Delete(pWorld);
		Safe_Delete(pMeshIndex);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Load_Smell_4(char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/StaticObjs/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".stobj");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to GamePlay : Load StaticObjs");

		return E_FAIL;
	}

	DWORD		dwByte = 0;
	_float4x4*	pWorld = new _float4x4;
	_int*		pMeshIndex = new _int;
	_float*		pGlowPower = new _float;
	_bool*		pAlphaBlend = new _bool;

	struct MAPOBJDESC
	{
		_float4x4		matWorld;
		_float			fGlowPower;
		_bool			bAlphaBlend;
	};

	//	<갯수 <메쉬넘버, 월드>>
	map<_uint, vector<MAPOBJDESC>>			map_MeshIdx_MapObjDesc;
	map<_uint, _uint>						map_MeshIdx_Num;

	while (true)
	{
		ReadFile(hFile, pMeshIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pGlowPower, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, pWorld, sizeof(_float4x4), &dwByte, nullptr);
		ReadFile(hFile, pAlphaBlend, sizeof(_bool), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pWorld);
			Safe_Delete(pMeshIndex);
			Safe_Delete(pGlowPower);
			Safe_Delete(pAlphaBlend);
			break;
		}


		MAPOBJDESC	tMapObjDesc;
		tMapObjDesc.matWorld = *pWorld;
		tMapObjDesc.fGlowPower = *pGlowPower;
		tMapObjDesc.bAlphaBlend = *pAlphaBlend;

		_uint	iCount = (_uint)map_MeshIdx_MapObjDesc.count(*pMeshIndex);
		if (!iCount)
		{
			std::vector<MAPOBJDESC>		vecWorld;
			vecWorld.push_back(tMapObjDesc);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecWorld);
		}
		else
		{
			auto Pair = map_MeshIdx_MapObjDesc.find(*pMeshIndex);
			std::vector<MAPOBJDESC>		vecObjDesc = Pair->second;
			vecObjDesc.push_back(tMapObjDesc);

			map_MeshIdx_MapObjDesc.erase(Pair);
			map_MeshIdx_MapObjDesc.emplace(*pMeshIndex, vecObjDesc);
		}

		iCount = (_uint)map_MeshIdx_Num.count(*pMeshIndex);
		if (!iCount)
			map_MeshIdx_Num.emplace(*pMeshIndex, 1);
		else
		{
			auto Pair = map_MeshIdx_Num.find(*pMeshIndex);
			_uint iNumInstancing = Pair->second;

			map_MeshIdx_Num.erase(Pair);
			map_MeshIdx_Num.emplace(*pMeshIndex, ++iNumInstancing);
		}

	}

	for (auto & Pair : map_MeshIdx_Num)
	{
		if (4 <= Pair.second)
		{
			_uint iNumInstancing = Pair.second;

			_float4x4*		arrWorld = new _float4x4[iNumInstancing];
			_float*			arrGlowPower = new _float[iNumInstancing];
			for (_uint i = 0; i < iNumInstancing; ++i)
			{
				arrWorld[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].matWorld;
				arrGlowPower[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].fGlowPower;
			}

			switch (Pair.first)
			{
			case 2087:
				
					tSmellDesc[3][0].pWorld = arrWorld;
					tSmellDesc[3][0].pGlowPower = arrGlowPower;
					tSmellDesc[3][0].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[3][0].iModelIndex = Pair.first;
					tSmellDesc[3][0].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[3][0])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2088:
			
					tSmellDesc[3][1].pWorld = arrWorld;
					tSmellDesc[3][1].pGlowPower = arrGlowPower;
					tSmellDesc[3][1].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[3][1].iModelIndex = Pair.first;
					tSmellDesc[3][1].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[3][1])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2089:
				
					tSmellDesc[3][2].pWorld = arrWorld;
					tSmellDesc[3][2].pGlowPower = arrGlowPower;
					tSmellDesc[3][2].iCurrentLevel = LEVEL_ADVRUI;
					tSmellDesc[3][2].iModelIndex = Pair.first;
					tSmellDesc[3][2].iNumInstancing = iNumInstancing;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell_Instancing", LEVEL_ADVRUI, L"Layer_MeshObj_Smell_Inst", &tSmellDesc[3][2])))
						return E_FAIL;
				
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			default:
				break;
			}


			ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
			continue;


			Safe_Delete_Array(arrWorld);
			Safe_Delete_Array(arrGlowPower);
		}
		else
		{
			for (_uint i = 0; i < Pair.second; ++i)
			{
				auto Pair_World = map_MeshIdx_MapObjDesc.find(Pair.first);
				std::vector<MAPOBJDESC>		vecObjDesc = Pair_World->second;

				_uint	iCount = (_uint)vecObjDesc.size();
				MAPOBJDESC	tMapObjDesc = vecObjDesc[iCount - 1];
				vecObjDesc.pop_back();

				map_MeshIdx_MapObjDesc.erase(Pair_World);
				map_MeshIdx_MapObjDesc.emplace(Pair.first, vecObjDesc);


				CMeshObj_Static::MESHOBJ_STATIC_DESC tMeshObj_StaticDesc;
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ADVRUI;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Smell", LEVEL_ADVRUI, L"Layer_MeshObj_Smell", &tMeshObj_StaticDesc)))
				{
					ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
					continue;
				}
			}


		}

		CloseHandle(hFile);

		Safe_Delete(pWorld);
		Safe_Delete(pMeshIndex);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Check_Smell()
{
	switch (m_iQuestIndex)
	{
	case 1:
		if (!m_bQuest[0])
		{
			if (FAILED(Load_Smell_2("RuiSmell2")))
				return E_FAIL;
			m_bQuest[0] = true;
			tSmellDesc[0][0].pMe->Set_Dead();
			tSmellDesc[0][1].pMe->Set_Dead();
			tSmellDesc[0][2].pMe->Set_Dead();
		}
		break;
	case 2:
		if (!m_bQuest[1])
		{
			if (FAILED(Load_Smell_3("RuiSmell3")))
				return E_FAIL;
			m_bQuest[1] = true;
			tSmellDesc[1][0].pMe->Set_Dead();
			tSmellDesc[1][1].pMe->Set_Dead();
			tSmellDesc[1][2].pMe->Set_Dead();
		}
		break;
	case 3:
		if (!m_bQuest[2])
		{
			if (FAILED(Load_Smell_4("RuiSmell4")))
				return E_FAIL;
			m_bQuest[2] = true;
			tSmellDesc[2][0].pMe->Set_Dead();
			tSmellDesc[2][1].pMe->Set_Dead();
			tSmellDesc[2][2].pMe->Set_Dead();
		}
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLevel_AdvRui::Create_Stone(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_fCreateStoneTime += fTimeDelta;
	if (m_fCreateStoneTime > 1.f && m_iStoneIndex < 12)
	{
		_int iDest = rand() % 3;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StoneSphere"), LEVEL_ADVRUI, TEXT("Layer_Stone"),&iDest)))
			return E_FAIL;

		m_fCreateStoneTime = 0.f;
		++m_iStoneIndex;
	}
	

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


CLevel_AdvRui * CLevel_AdvRui::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_AdvRui*	pInstance = new CLevel_AdvRui(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_AdvRui"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_AdvRui::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
}
