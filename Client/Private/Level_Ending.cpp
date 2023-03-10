#include "stdafx.h"
#include "..\Public\Level_Ending.h"

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
#include  "Door.h"
#include "Level_BattleEnmu.h"
#include "ImGuiManager.h"
#include "Level_BossEnmu.h"
#include "Effect_Manager.h"
#include "Layer.h"
#include "SoundMgr.h"
unsigned int APIENTRY Thread_Ending(void* pArg)
{
	CLevel_Ending*		pLoader = (CLevel_Ending*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	pLoader->Ready_Lights();
	g_fLoading = 10.f;
	pLoader->Ready_Layer_Camera(TEXT("Layer_Camera"));
	g_fLoading = 40.f;
	pLoader->Ready_Layer_Player(TEXT("Layer_Player"));
	g_fLoading = 60.f;
	pLoader->Ready_Layer_BackGround(TEXT("Layer_BackGround"));
	pLoader->Ready_Layer_EndingFont(TEXT("Layer_Font"));
	g_fLoading = 80.f;
	pLoader->Load_StaticObjects("TrainMap");
	g_fLoading = 100.f;
	CComponent* pOut = pGameInstance->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Renderer");
	pLoader->Set_Renderer((CRenderer*)pOut);

	_float fValue[CRenderer::VALUE_END] = { 1.f, 0.37f ,0.1f ,182.f ,1571.f ,0.142f ,0.f ,1.36f,0.4f,1.f,20.f,20.f,0.07f,1.3f,0.3f,0.5f, 2.f, 0.6f,0.1f,0.3f,15.f,0.f,0.f, 0.07f };
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_R), 1.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_G), 0.37f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_B), 0.1f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGDISTANCE), 182.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGRANGE), 1571.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGMINPOWER), 0.142f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_CUBEMAPFOG), 0.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_AO), 1.36f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_AORADIUS), 0.4f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_GLOWBLURCOUNT), 1.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_DISTORTION), 20.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_OUTLINE), 20.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_INNERLINE), 0.07f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_ENVLIGHT), 1.3f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT), 0.3f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT_TESTLENGTH), 0.5f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT_MINUS), 2.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTPOWER), 0.6f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_SHADOWTESTLENGTH), 0.1f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_PLC_SHADOW), 0.3f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_MAPGRAYSCALETIME), 15.f);
	pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_SHADOWPOWER), 0.07f);
	pLoader->Get_Renderer()->ReturnValue();
	pLoader->Get_Renderer()->Set_Far(g_fFar);

	for (_int i = 0; i < CRenderer::VALUE_END; ++i)
		CImGuiManager::Get_Instance()->Setting_PostProcessingValue(i, fValue[i]);

	RELEASE_INSTANCE(CGameInstance);


	LeaveCriticalSection(&pLoader->Get_CriticalSection());
	pLoader->Set_Finished();
	g_bThread = false;
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Set_LoadingDead();
	RELEASE_INSTANCE(CUI_Manager);
	return 0;
}
CLevel_Ending::CLevel_Ending(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Ending::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	g_iLevel = LEVEL_ENDING;
	g_bThread = true;
	g_fLoading = 0.f;
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	//pUI_Manager->Add_Loading();

	CoInitializeEx(nullptr, 0);

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Ending, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	g_fFar = 2000.f;

	RELEASE_INSTANCE(CUI_Manager);

	if (FAILED(__super::Initialize()))
		return E_FAIL;


	return S_OK;
}

void CLevel_Ending::Tick(_float fTimeDelta)
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
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
		
		if (!m_bEnding)
		{
			CSoundMgr::Get_Instance()->BGM_Stop();
			CSoundMgr::Get_Instance()->PlayBGM(TEXT("Ending.wav"), g_fBGM);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_StoryScene(CCamera_Dynamic::STORYSCENE_ENDING);
			dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QusetCam();
			m_bEnding = true;
		}
		Create_Wind(fTimeDelta);
	
		RELEASE_INSTANCE(CGameInstance);
	}
}

void CLevel_Ending::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_Ending::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->LightClear();

	LIGHTDESC			LightDesc;

	/* For.Point */
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_RUISHADOW;
	LightDesc.vDirection = _float4(-1.f, 30.f, 60.f, 1.f);	//	eye
	XMStoreFloat4(&LightDesc.vDiffuse, XMLoadFloat4(&LightDesc.vDirection) + XMLoadFloat4(&_float4(1.f, -1.f, 1.5f, 1.f)));
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
	//1,-1,1.5
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
	LightDesc.vDirection = _float4(-800.f, 850.f, -250.f, 1.f);		//	eye
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


	//	Point Light
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT1;
	LightDesc.vPosition = _float4(0.f, 308.f, 1060.f, 1.f);
	LightDesc.fRange = 572.f;
	LightDesc.vDiffuse = _float4(1.f, 0.7f, 0.2f, 0.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT3;
	LightDesc.vPosition = _float4(0.f, 14.f, 406.f, 1.f);
	LightDesc.fRange = 8.4f;
	LightDesc.vDiffuse = _float4(1.f, 0.7f, 0.2f, 0.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_int iModelNum = 0;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EndingModel"), LEVEL_ENDING, TEXT("Layer_Ending"), &iModelNum)))
		return E_FAIL;
	iModelNum = 1;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EndingModel"), LEVEL_ENDING, TEXT("Layer_Ending"), &iModelNum)))
		return E_FAIL;
	iModelNum = 2;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EndingModel"), LEVEL_ENDING, TEXT("Layer_Ending"), &iModelNum)))
		return E_FAIL;
	iModelNum = 3;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EndingModel"), LEVEL_ENDING, TEXT("Layer_Ending"), &iModelNum)))
		return E_FAIL;
	iModelNum = 4;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EndingModel"), LEVEL_ENDING, TEXT("Layer_Ending"), &iModelNum)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(45.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = g_fFar;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_ENDING, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	return S_OK;
}
HRESULT CLevel_Ending::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkyBox"), LEVEL_ENDING, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_EndingFont(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EndingCredit"), LEVEL_ENDING, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Ending::Load_StaticObjects(char * pFileName)
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
		tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ENDING;
		tMeshObj_StaticDesc.iModelIndex = *pMeshIndex;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_ENDING, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
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
			tMeshObj_Static_InstDesc.iCurrentLevel = LEVEL_ENDING;
			tMeshObj_Static_InstDesc.iModelIndex = Pair.first;
			tMeshObj_Static_InstDesc.iNumInstancing = iNumInstancing;

			CDoor::DOOR_DESC	tCharacterDesc1p;
			tCharacterDesc1p.m_pPlayer = m_pPlayer;
			CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
			switch (tMeshObj_Static_InstDesc.iModelIndex)
			{
			case 2001:
			case 2002:
			case 2003:
			case 2046:
			case 2081:
				for (_int i = 0; i < 3; ++i)
				{
					tMeshObj_Static_InstDesc.iTypeNum = i;
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_ENDING, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
					{
						ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
						continue;
					}
				}
				break;
			case 2108:
				for (_uint i = 0; i < Pair.second; ++i)
				{
					tCharacterDesc1p.iModelIndex = i;
					tCharacterDesc1p.matWorld = tMeshObj_Static_InstDesc.pWorld[i];
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Door"), LEVEL_ENDING, TEXT("Layer_Door"), &tCharacterDesc1p)))
					{
						Safe_Delete_Array(arrWorld);
						Safe_Delete_Array(arrGlowPower);
						return E_FAIL;
					}
				}
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2019:
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			case 2020:
				Safe_Delete_Array(arrWorld);
				Safe_Delete_Array(arrGlowPower);
				continue;
				break;
			default:
				break;
			}
			tMeshObj_Static_InstDesc.iTypeNum = 4;
			if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_ENDING, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
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
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_ENDING;
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
				case 2097:
					for (_int i = 0; i < 3; ++i)
					{
						tMeshObj_StaticDesc.iTypeNum = i;
						if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_ENDING, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
						{
							ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
							continue;
						}
					}
					continue;
					break;
				case 2109:
					continue;
					break;
				default:
					break;
				}
				tMeshObj_StaticDesc.iTypeNum = 4;
				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_ENDING, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
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
HRESULT CLevel_Ending::Load_Map(const _tchar* pLayerTag, char * pFileName)
{


	return S_OK;
}

HRESULT CLevel_Ending::Create_Wind(_float fTimeDelta)
{
	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
	if (!m_bEffect)
	{
		pEffectManger->Create_Effect(CEffect_Manager::EFF_TRAIN_SMOKE, &XMVectorSet(-0.051f,17.682f,122.145f,1.f));
		m_bEffect = true;
	}
	m_fEffectTime += fTimeDelta;
	if (m_fEffectTime > 1.f)
	{
		pEffectManger->Create_Effect(CEffect_Manager::EFF_TRAIN_WIND, &XMVectorSet(-0.52f, 16.7f, 201.444f, 1.f));
		pEffectManger->Create_Effect(CEffect_Manager::EFF_TRAIN_WIND, &XMVectorSet(-0.52f, 16.7f, 283.29f, 1.f));
		pEffectManger->Create_Effect(CEffect_Manager::EFF_TRAIN_WIND, &XMVectorSet(-0.52f, 16.7f, 365.78f, 1.f));
		m_fEffectTime = 0.f;
	}
	RELEASE_INSTANCE(CEffect_Manager);
	return S_OK;
}

CLevel_Ending * CLevel_Ending::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Ending*	pInstance = new CLevel_Ending(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Ending"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Ending::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
}
