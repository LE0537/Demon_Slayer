#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "SoundMgr.h"
#include "UI_Manager.h"

#include "GameObj.h"
#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"
#include "Terrain.h"

#include "Effect_Manager.h"


CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Set_CharNameUIZero();
	RELEASE_INSTANCE(CUI_Manager);

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

	if (FAILED(Load_Map(L"Layer_BackGround", "11_Map")))
		return E_FAIL;

//	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
//		return E_FAIL;

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;	


	if (FAILED(Load_StaticObjects("11Test")))
		return E_FAIL;


	CSoundMgr::Get_Instance()->PlayBGM(TEXT("hov.wav"), 0.45f);

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bCreateUI)
	{
		_bool bOniCheck = CUI_Manager::Get_Instance()->P1_Oni_Check();
		if(!bOniCheck)
			CUI_Manager::Get_Instance()->Add_P1_PersonHpUI();
		else
			CUI_Manager::Get_Instance()->Add_P1_OniHpUI();

		bOniCheck = CUI_Manager::Get_Instance()->P2_Oni_Check();
		if (!bOniCheck)
			CUI_Manager::Get_Instance()->Add_P2_PersonHpUI();
		else
			CUI_Manager::Get_Instance()->Add_P2_OniHpUI();

		m_bCreateUI = true;
	}

	if (pGameInstance->Key_Down(DIK_F1))
		g_bDebug = !g_bDebug;

	if (pGameInstance->Key_Down(DIK_F2))
		g_bCollBox = !g_bCollBox;
	
	RELEASE_INSTANCE(CGameInstance);
}

void CLevel_GamePlay::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(-1.f, 2.f, -2.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	/* For.Point */
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_FIELDSHADOW;
	LightDesc.vDirection = _float4(-30.f, 60.f, -30.f, 0.f);
	LightDesc.vDiffuse = _float4(50.f, 0.f, 30.f, 1.f);
	LightDesc.vAmbient = _float4(0.f, 0.1f, 0.f, 0.f);


	if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

	_uint i1p = pUIManager->Get_1P();
	_uint i2p = pUIManager->Get_2P();
	_int b1p = 1;
	_int b2p = 2;
	switch (i1p)
	{
	case 0:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_GAMEPLAY, TEXT("Layer_Akaza"),&b1p)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), &b1p)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_GAMEPLAY, TEXT("Layer_Rui"), &b1p)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), &b1p)))
			return E_FAIL;
		break;
	default:
		break;
	}
	switch (i2p)
	{
	case 0:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_GAMEPLAY, TEXT("Layer_Akaza"), &b2p)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), &b2p)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_GAMEPLAY, TEXT("Layer_Rui"), &b2p)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), &b2p)))
			return E_FAIL;
		break;
	default:
		break;
	}
	RELEASE_INSTANCE(CUI_Manager);
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);



//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BattleField"), LEVEL_GAMEPLAY, pLayerTag)))
//		return E_FAIL;



	Safe_Release(pGameInstance);



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_GAMEPLAY, pLayerTag, &CameraDesc)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_GAMEPLAY, pLayerTag)))
	//	return E_FAIL;

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;*/

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_GAMEPLAY, pLayerTag)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Load_StaticObjects(char * pFileName)
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

	//	<갯수 <메쉬넘버, 월드>>
	map<_uint, vector<_float4x4>>		map_MeshIdx_World;
	map<_uint, _uint>					map_MeshIdx_Num;

	while (true)
	{
		ReadFile(hFile, pMeshIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pWorld, sizeof(_float4x4), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pWorld);
			Safe_Delete(pMeshIndex);
			break;
		}


		_uint	iCount = (_uint)map_MeshIdx_World.count(*pMeshIndex);
		if (!iCount)
		{
			std::vector<_float4x4>		vecWorld;
			vecWorld.push_back(*pWorld);
			map_MeshIdx_World.emplace(*pMeshIndex, vecWorld);
		}
		else
		{
			auto Pair = map_MeshIdx_World.find(*pMeshIndex);
			std::vector<_float4x4>		vecWorld = Pair->second;
			vecWorld.push_back(*pWorld);

			map_MeshIdx_World.erase(Pair);
			map_MeshIdx_World.emplace(*pMeshIndex, vecWorld);
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
		tMeshObj_StaticDesc.iCurrentLevel = LEVEL_GAMEPLAY;
		tMeshObj_StaticDesc.iModelIndex = *pMeshIndex;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_GAMEPLAY, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
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
			for (_uint i = 0; i < iNumInstancing; ++i)
			{
				arrWorld[i] = map_MeshIdx_World.find(Pair.first)->second[i];
			}

			tMeshObj_Static_InstDesc.pWorld = arrWorld;
			tMeshObj_Static_InstDesc.iCurrentLevel = LEVEL_GAMEPLAY;
			tMeshObj_Static_InstDesc.iModelIndex = Pair.first;
			tMeshObj_Static_InstDesc.iNumInstancing = iNumInstancing;

			if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_GAMEPLAY, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
			{
				ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
				continue;
			}

			Safe_Delete_Array(arrWorld);
		}
		else
		{
			for (_uint i = 0; i < Pair.second; ++i)
			{
				auto Pair_World = map_MeshIdx_World.find(Pair.first);
				std::vector<_float4x4>		vecWorld = Pair_World->second;

				_uint	iCount = (_uint)vecWorld.size();
				_float4x4	matWorld4x4 = vecWorld[iCount - 1];
				vecWorld.pop_back();

				map_MeshIdx_World.erase(Pair_World);
				map_MeshIdx_World.emplace(Pair.first, vecWorld);


				CMeshObj_Static::MESHOBJ_STATIC_DESC tMeshObj_StaticDesc;
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_GAMEPLAY;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = matWorld4x4;

				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_GAMEPLAY, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
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

HRESULT CLevel_GamePlay::Load_Map(const _tchar* pLayerTag, char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTerrain::NEWTERRAINDESC tNewTerrainDesc;
	ZeroMemory(&tNewTerrainDesc, sizeof tNewTerrainDesc);

	//	strcpy_s(tTerrainDesc.strFileName, MAX_PATH, "TestMap");
	strcpy_s(tNewTerrainDesc.tTerrainDesc.strFileName, MAX_PATH, pFileName);
	if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_NewTerrain", LEVEL_GAMEPLAY, pLayerTag, &tNewTerrainDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
	

}
