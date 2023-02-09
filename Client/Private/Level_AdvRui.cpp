#include "stdafx.h"
#include "Level_AdvRui.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "GameObj.h"
#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"
#include "Terrain.h"
#include "Level_GamePlay.h"
CLevel_AdvRui::CLevel_AdvRui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_AdvRui::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	/*CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	pUI_Manager->Add_Adc_Menu();
	RELEASE_INSTANCE(CUI_Manager);*/

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Load_StaticObjects("RuiStory")))
		return E_FAIL;

	return S_OK;
}

void CLevel_AdvRui::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Key_Down(DIK_F1))
		g_bDebug = !g_bDebug;
	RELEASE_INSTANCE(CGameInstance);
}

void CLevel_AdvRui::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_AdvRui::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;

	/* For.Point */
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_FIELDSHADOW;
	LightDesc.vDirection = _float4(-10.f, 150.f, -10.f, 1.f);
	LightDesc.vDiffuse = _float4(60.f, -20.f, 60.f, 1.f);
	LightDesc.vAmbient = _float4(0.f, 0.1f, 0.f, 0.f);

	_vector vLook = XMLoadFloat4(&LightDesc.vDiffuse) - XMLoadFloat4(&LightDesc.vDirection);

	if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	XMStoreFloat4(&LightDesc.vDirection, XMVector3Normalize(vLook));
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_BATTLESHADOW;
	LightDesc.vDirection = _float4(-400.f, 500.f, -400.f, 1.f);		//	eye
	XMStoreFloat4(&LightDesc.vDiffuse, XMVectorSetW(XMLoadFloat4(&_float4(-10.f, 150.f, -10.f, 1.f)) + XMVector3Normalize(vLook), 1.f));
	//	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vDirection = _float4(-10.f, 150.f, -10.f, 1.f);		//	eye
	LightDesc.vDiffuse = _float4(60.f, -20.f, 60.f, 1.f);			//	at
	LightDesc.vAmbient = _float4(0.f, 0.1f, 0.f, 0.f);

	if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc1p;

	tCharacterDesc1p.i1P2P = 10;
	tCharacterDesc1p.bSub = false;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_ADVRUI, TEXT("Layer_Tanjiro"), &tCharacterDesc1p)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_ADVRUI, TEXT("Layer_Akaza"), &tCharacterDesc1p)))
		return E_FAIL;
	tCharacterDesc1p.bSub = true;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_ADVRUI, TEXT("Layer_Nezuko"), &tCharacterDesc1p)))
		return E_FAIL;
	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_ADVRUI, TEXT("Layer_Rui"), &tCharacterDesc1p)))
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
	CameraDesc.CameraDesc.fFar = 500.f;

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
HRESULT CLevel_AdvRui::Load_Map(const _tchar* pLayerTag, char * pFileName)
{


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


}
