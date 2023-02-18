#include "stdafx.h"
#include "Level_GameResult.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "MenuModel.h"
#include "GameObj.h"
#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"
#include "Terrain.h"
#include "SoundMgr.h"

CLevel_GameResult::CLevel_GameResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GameResult::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Set_RankScore();
	pUIManager->Add_BattleResult();
	RELEASE_INSTANCE(CUI_Manager);

	CSoundMgr::Get_Instance()->PlayBGM(TEXT("CharSel.wav"), fBGM);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Load_Map(L"Layer_BackGround", "11_Map_Rui")))
		return E_FAIL;

	if (FAILED(Load_StaticObjects("11_Rui")))
		return E_FAIL;

	return S_OK;
}

void CLevel_GameResult::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	m_fStartTime += fTimeDelta;
	if (m_iSub != 99)
	{
		m_pModel->Set_ModelIndex(m_iMain);
		m_pModel->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(430.f - g_iWinSizeX * 0.5f, -820.f + g_iWinSizeY * 0.5f, -350.f, 1.f));
		m_pModelSub->Set_ModelIndex(m_iSub);
		m_pModelSub->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(850.f - g_iWinSizeX * 0.5f, -820.f + g_iWinSizeY * 0.5f, -350.f, 1.f));
		if (!m_bStart && m_fStartTime > 2.f)
		{
			dynamic_cast<CMenuModel*>(m_pModel)->Get_ModelCom(m_iMain)->Set_CurrentAnimIndex(1);
			dynamic_cast<CMenuModel*>(m_pModel)->Get_ModelCom(m_iMain)->Set_Loop(1);
			dynamic_cast<CMenuModel*>(m_pModel)->Get_ModelCom(m_iMain)->Set_LinearTime(1, 0.01f);
			dynamic_cast<CMenuModel*>(m_pModelSub)->Get_ModelCom(m_iSub)->Set_CurrentAnimIndex(1);
			dynamic_cast<CMenuModel*>(m_pModelSub)->Get_ModelCom(m_iSub)->Set_Loop(1);
			dynamic_cast<CMenuModel*>(m_pModelSub)->Get_ModelCom(m_iSub)->Set_LinearTime(1, 0.01f);

			m_bStart = true;
		}
	}
	else if (m_iSub == 99)
	{
		m_pModel->Set_ModelIndex(m_iMain);
		m_pModel->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(640.f - g_iWinSizeX * 0.5f, -820.f + g_iWinSizeY * 0.5f, -300.f, 1.f));
		if (!m_bStart && m_fStartTime > 2.f)
		{
			dynamic_cast<CMenuModel*>(m_pModel)->Get_ModelCom(m_iMain)->Set_CurrentAnimIndex(1);
			dynamic_cast<CMenuModel*>(m_pModel)->Get_ModelCom(m_iMain)->Set_Loop(1);
			dynamic_cast<CMenuModel*>(m_pModel)->Get_ModelCom(m_iMain)->Set_LinearTime(1, 0.01f);
			m_bStart = true;
		}
	}

	if (pGameInstance->Key_Down(DIK_T))
	{
		pUIManager->RankInfo_ZeroMemory(0);
		pUIManager->RankInfo_ZeroMemory(1);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_MENU))))
			return;
	}

	Safe_Release(pGameInstance);

	RELEASE_INSTANCE(CUI_Manager);
}

void CLevel_GameResult::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_GameResult::Ready_Lights()
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

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
HRESULT CLevel_GameResult::Load_StaticObjects(char * pFileName)
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
		tMeshObj_StaticDesc.iCurrentLevel = LEVEL_SELECTCHAR;
		tMeshObj_StaticDesc.iModelIndex = *pMeshIndex;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_SELECTCHAR, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
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
			tMeshObj_Static_InstDesc.iCurrentLevel = LEVEL_GAMERESULT;
			tMeshObj_Static_InstDesc.iModelIndex = Pair.first;
			tMeshObj_Static_InstDesc.iNumInstancing = iNumInstancing;

			if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_GAMERESULT, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
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
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_GAMERESULT;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_GAMERESULT, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
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
HRESULT CLevel_GameResult::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iFirstChar = pUIManager->Get_Result_Info().iFirstTarget;//첫번째캐릭터 번호
	_uint iSecondChar = pUIManager->Get_Result_Info().iSecondTarget; //서브캐릭터 번호
	m_iMain = iFirstChar;
	m_iSub = iSecondChar;
	if (iSecondChar != 99)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuModel"), LEVEL_GAMERESULT, TEXT("Layer_UI"), &m_pModel)))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuModel"), LEVEL_GAMERESULT, TEXT("Layer_UI"), &m_pModelSub)))
			return E_FAIL;
		dynamic_cast<CMenuModel*>(m_pModel)->Get_ModelCom(m_iMain)->Set_CurrentAnimIndex(0);
		dynamic_cast<CMenuModel*>(m_pModelSub)->Get_ModelCom(m_iSub)->Set_CurrentAnimIndex(0);
		m_pModel->Set_1P(1);
		m_pModelSub->Set_1P(3);
	}
	else if (iSecondChar == 99)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuModel"), LEVEL_GAMERESULT, TEXT("Layer_UI"), &m_pModel)))
			return E_FAIL;
		dynamic_cast<CMenuModel*>(m_pModel)->Get_ModelCom(m_iMain)->Set_CurrentAnimIndex(0);
		m_pModel->Set_1P(1);
	}
	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

HRESULT CLevel_GameResult::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_GAMERESULT, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GameResult::Load_Map(const _tchar * pLayerTag, char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTerrain::NEWTERRAINDESC tNewTerrainDesc;
	ZeroMemory(&tNewTerrainDesc, sizeof tNewTerrainDesc);

	//	strcpy_s(tTerrainDesc.strFileName, MAX_PATH, "TestMap");
	strcpy_s(tNewTerrainDesc.tTerrainDesc.strFileName, MAX_PATH, pFileName);
	if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_NewTerrain", LEVEL_GAMERESULT, pLayerTag, &tNewTerrainDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_GameResult * CLevel_GameResult::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GameResult*	pInstance = new CLevel_GameResult(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_GameResult"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GameResult::Free()
{
	__super::Free();


}
