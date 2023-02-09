#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "SoundMgr.h"
#include "UI_Manager.h"
#include "Level_Loading.h"

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

	if (FAILED(Load_Map(L"Layer_BackGround", "11_Map_Rui")))
		return E_FAIL;

//	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
//		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Load_StaticObjects("11_Rui")))
		return E_FAIL;

	CSoundMgr::Get_Instance()->PlayBGM(TEXT("hov.wav"), 0.45f);

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

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

		CUI_Manager::Get_Instance()->Add_BattleUI();
		CUI_Manager::Get_Instance()->Add_P1_Combo();
		CUI_Manager::Get_Instance()->Add_P2_Combo();

		m_bCreateUI = true;
	}

	if (pGameInstance->Key_Down(DIK_F1))
		g_bDebug = !g_bDebug;

	if (pGameInstance->Key_Down(DIK_F2))
		g_bCollBox = !g_bCollBox;

	if (pGameInstance->Key_Down(DIK_0)) {
		/*CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_TANATTACK1, m_pTarget);

		RELEASE_INSTANCE(CEffect_Manager)*/;
	}

	if (pUIManager->Get_LevelResultOn())
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMERESULT))))
			return;
	}
		
	RELEASE_INSTANCE(CUI_Manager);
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



	/* For.Point */
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_FIELDSHADOW;
	LightDesc.vDirection = _float4(-10.f, 150.f, -10.f, 1.f);		//	eye
	LightDesc.vDiffuse = _float4(60.f, -20.f, 60.f, 1.f);			//	at
	LightDesc.vAmbient = _float4(0.f, 0.1f, 0.f, 0.f);

	_vector vLook = XMLoadFloat4(&LightDesc.vDiffuse) - XMLoadFloat4(&LightDesc.vDirection);

	if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	XMStoreFloat4(&LightDesc.vDirection,XMVector3Normalize(vLook));
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	

	/* For.Directional*/
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

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

	//===================================================================================
	//================================== Load AnimObjs ==================================
	//===================================================================================
	char		pFileName[MAX_PATH] = "11_Rui";			//	파일 이름
	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/AnimObjs/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".units");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to Load : Units");

		return S_OK;
	}

	CHARACTERDESC	tCharacterDesc1p;
	CHARACTERDESC	tCharacterDesc2p;

	DWORD			dwByte = 0;
	_int*			pObjNumber = new _int;
	_int*			pNaviIndex = new _int;
	_float4x4*		pWorld = new _float4x4;

	vector<vector<CGameObject*>>	vecUnits;
	vecUnits.resize(10);

	while (true)
	{
		ReadFile(hFile, pObjNumber, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pNaviIndex, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, pWorld, sizeof(_float4x4), &dwByte, nullptr);

		if (0 == dwByte)
			break;


		_uint		iNaviIndex = *pNaviIndex;
		_float4x4	matWorld = *pWorld;

		if (1001 == *pObjNumber)
		{
			//	Player 1
			tCharacterDesc1p.i1P2P = 1;
			tCharacterDesc1p.iNaviIndex = *pNaviIndex;
			tCharacterDesc1p.matWorld = *pWorld;
		}
		else
		{
			//	Player 2
			tCharacterDesc2p.i1P2P = 2;
			tCharacterDesc2p.iNaviIndex = *pNaviIndex;
			tCharacterDesc2p.matWorld = *pWorld;
		}
	}

	Safe_Delete(pObjNumber);
	Safe_Delete(pNaviIndex);
	Safe_Delete(pWorld);

	CloseHandle(hFile);
	//===================================================================================
	//================================== Load AnimObjs ==================================
	//===================================================================================

	_uint i1p = pUIManager->Get_Sel1P();
	_uint i2p = pUIManager->Get_Sel2P();
	_uint i1p_2 = pUIManager->Get_Sel1P_2();
	_uint i2p_2 = pUIManager->Get_Sel2P_2();
	switch (i1p)
	{
	case 0:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 1:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 2:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_GAMEPLAY, TEXT("Layer_Rui"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 3:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_GAMEPLAY, TEXT("Layer_Akaza"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 4:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_GAMEPLAY, TEXT("Layer_Nezuko"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 5:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shinobu"), LEVEL_GAMEPLAY, TEXT("Layer_Shinobu"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	default:
		break;
	}
	switch (i2p)
	{
	case 0:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 1:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 2:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_GAMEPLAY, TEXT("Layer_Rui"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 3:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_GAMEPLAY, TEXT("Layer_Akaza"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 4:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_GAMEPLAY, TEXT("Layer_Nezuko"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 5:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shinobu"), LEVEL_GAMEPLAY, TEXT("Layer_Shinobu"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	default:
		break;
	}
	switch (i1p_2)
	{
	case 0:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 1:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 2:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_GAMEPLAY, TEXT("Layer_Rui"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 3:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_GAMEPLAY, TEXT("Layer_Akaza"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 4:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_GAMEPLAY, TEXT("Layer_Nezuko"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 5:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shinobu"), LEVEL_GAMEPLAY, TEXT("Layer_Shinobu"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	default:
		break;
	}
	switch (i2p_2)
	{
	case 0:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 1:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_GAMEPLAY, TEXT("Layer_Kyoujuro"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 2:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_GAMEPLAY, TEXT("Layer_Rui"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 3:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_GAMEPLAY, TEXT("Layer_Akaza"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 4:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_GAMEPLAY, TEXT("Layer_Nezuko"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 5:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shinobu"), LEVEL_GAMEPLAY, TEXT("Layer_Shinobu"), &tCharacterDesc2p)))
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkyBox"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

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
	CameraDesc.CameraDesc.fFar = 1500.f;

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
			_float*			arrGlowPower = new _float[iNumInstancing];
			for (_uint i = 0; i < iNumInstancing; ++i)
			{
				arrWorld[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].matWorld;
				arrGlowPower[i] = map_MeshIdx_MapObjDesc.find(Pair.first)->second[i].fGlowPower;
			}

			tMeshObj_Static_InstDesc.pWorld = arrWorld;
			tMeshObj_Static_InstDesc.pGlowPower = arrGlowPower;
			tMeshObj_Static_InstDesc.iCurrentLevel = LEVEL_GAMEPLAY;
			tMeshObj_Static_InstDesc.iModelIndex = Pair.first;
			tMeshObj_Static_InstDesc.iNumInstancing = iNumInstancing;

			if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_GAMEPLAY, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
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
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_GAMEPLAY;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

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
