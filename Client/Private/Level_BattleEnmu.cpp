#include "stdafx.h"
#include "..\Public\Level_BattleEnmu.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "SoundMgr.h"
#include "UI_Manager.h"
#include "Level_Loading.h"
#include "Level_GameResult.h"
#include "Level_AdvRui.h"
#include "Level_StroyMenu.h"
#include "GameObj.h"
#include "MeshObj_Static.h"
#include "MeshObj_Static_Inst.h"
#include "Terrain.h"
#include "Door.h"
#include "Effect_Manager.h"

unsigned int APIENTRY Thread_BattleEnmu(void* pArg)
{
	CLevel_BattleEnmu*		pLoader = (CLevel_BattleEnmu*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Add_Loading();

	_float ThreadTime = 0.f;
	_float ThreadDelay = 0.f;
	_bool  m_bTreadStop = true;
	while (m_bTreadStop)
	{
		pGameInstance->Update_TimeDelta(TEXT("ThreadTimer_Default"));
		ThreadTime += pGameInstance->Get_TimeDelta(TEXT("ThreadTimer_Default"));

		if (ThreadTime >= 1.0f / 60.0f)
		{
			ThreadTime = 0.f;
			pGameInstance->Update_TimeDelta(TEXT("ThreadTimer_60"));
			ThreadDelay += pGameInstance->Get_TimeDelta(TEXT("ThreadTimer_60"));

			pUIManager->Tick_Loading(pGameInstance->Get_TimeDelta(TEXT("ThreadTimer_60")));
			if (ThreadDelay > 3.f)
			{
				m_bTreadStop = false;
				break;
			}
		}
	}
	pUIManager->Set_LoadingDead();
	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
	LeaveCriticalSection(&pLoader->Get_CriticalSection());
	pLoader->Set_Finished();
	g_bThread = false;
	return 0;
}
CLevel_BattleEnmu::CLevel_BattleEnmu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_BattleEnmu::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	g_iLevel = LEVEL_BATTLEENMU;
	g_bThread = true;

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	CoInitializeEx(nullptr, 0);

	InitializeCriticalSection(&m_CriticalSection);

	pGameInstance->Update_TimeDelta(TEXT("ThreadTimer_Default"));
	pGameInstance->Update_TimeDelta(TEXT("ThreadTimer_60"));

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_BattleEnmu, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	pUIManager->Set_CharNameUIZero();

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;


	if (FAILED(Load_StaticObjects("TrainMap")))
		return E_FAIL;


	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;



	CComponent* pOut = pGameInstance->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Renderer");
	m_pRendererCom = (CRenderer*)pOut;

	if (nullptr == m_pRendererCom)
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_R), 0.15f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_G), 0.15f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_B), 0.4f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGDISTANCE), 40.f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_FOGRANGE), 450.f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_AO), 1.36f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_AORADIUS), 0.4f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_GLOWBLURCOUNT), 1.f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_DISTORTION), 20.f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_OUTLINE), 300.f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_INNERLINE), 0.05f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_ENVLIGHT), 1.79f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT), 0.2f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTPOWER), 0.85f);
	m_pRendererCom->Set_Value(CRenderer::VALUETYPE(CRenderer::VALUE_SHADOWTESTLENGTH), 1.f);

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);

	CSoundMgr::Get_Instance()->PlayBGM(TEXT("PlayerBattle.wav"), fBGM);

	

	return S_OK;
}

void CLevel_BattleEnmu::Tick(_float fTimeDelta)
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
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

		if (!m_bCreateUI)
		{
			_bool bOniCheck = pUIManager->P1_Oni_Check();
			if (!bOniCheck)
				pUIManager->Add_P1_PersonHpUI_Level_Enmu();
			else
				pUIManager->Add_P1_OniHpUI_Level_Enmu();

			bOniCheck = pUIManager->P2_Oni_Check();
			if (!bOniCheck)
				pUIManager->Add_P2_PersonHpUI_Level_Enmu();
			else
				pUIManager->Add_P2_OniHpUI_Level_Enmu();

		/*	pUIManager->Add_BattleUI_Enmu();
			pUIManager->Add_P1_Combo_Enmu();
			pUIManager->Add_P2_Combo_Enmu();*/
			
			m_bCreateUI = true;
		}

		if (pUIManager->Get_BattleTypeCheck())
		{
			if (pUIManager->Get_LevelResultOn())
			{
				if (FAILED(pGameInstance->Open_Level(LEVEL_GAMERESULT, CLevel_GameResult::Create(m_pDevice, m_pContext))))
					return;
			}
		}



		RELEASE_INSTANCE(CUI_Manager);
		RELEASE_INSTANCE(CGameInstance);
	}
}

void CLevel_BattleEnmu::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_BattleEnmu::Ready_Lights()
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

	const LIGHTDESC* pLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW);
	if (nullptr == pLightDesc)
	{
		if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
			return E_FAIL;
	}
	else
	{
		pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW, LightDesc.vDirection, LightDesc.vDiffuse);
	}

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

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_BATTLESHADOW;
	LightDesc.vDirection = _float4(-220.f, 600.f, -200.f, 1.f);			//	eye
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

HRESULT CLevel_BattleEnmu::Ready_Layer_Player(const _tchar * pLayerTag)
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
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_BATTLEENMU, TEXT("Layer_Tanjiro"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 1:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_BATTLEENMU, TEXT("Layer_Kyoujuro"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 2:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_BATTLEENMU, TEXT("Layer_Rui"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 3:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_BATTLEENMU, TEXT("Layer_Akaza"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 4:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_BATTLEENMU, TEXT("Layer_Nezuko"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 5:
		tCharacterDesc1p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shinobu"), LEVEL_BATTLEENMU, TEXT("Layer_Shinobu"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	default:
		break;
	}
	switch (i2p)
	{
	case 0:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_BATTLEENMU, TEXT("Layer_Tanjiro"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 1:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_BATTLEENMU, TEXT("Layer_Kyoujuro"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 2:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_BATTLEENMU, TEXT("Layer_Rui"), &tCharacterDesc2p)))
			return E_FAIL;
		//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiDad"), LEVEL_BATTLEENMU, TEXT("Layer_Rui"), &tCharacterDesc2p)))
		//	return E_FAIL;
		break;
	case 3:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_BATTLEENMU, TEXT("Layer_Akaza"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 4:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_BATTLEENMU, TEXT("Layer_Nezuko"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 5:
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shinobu"), LEVEL_BATTLEENMU, TEXT("Layer_Shinobu"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 6:
		tCharacterDesc2p.i1P2P = 11;
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiDad"), LEVEL_BATTLEENMU, TEXT("Layer_RuiDad"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 7:
		tCharacterDesc2p.i1P2P = 11;
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_BATTLEENMU, TEXT("Layer_Rui"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 8: //엔무
		tCharacterDesc2p.i1P2P = 22;
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_BATTLEENMU, TEXT("Layer_Rui"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	default:
		break;
	}
	switch (i1p_2)
	{
	case 0:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_BATTLEENMU, TEXT("Layer_Tanjiro"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 1:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_BATTLEENMU, TEXT("Layer_Kyoujuro"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 2:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_BATTLEENMU, TEXT("Layer_Rui"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 3:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_BATTLEENMU, TEXT("Layer_Akaza"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 4:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_BATTLEENMU, TEXT("Layer_Nezuko"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	case 5:
		tCharacterDesc1p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shinobu"), LEVEL_BATTLEENMU, TEXT("Layer_Shinobu"), &tCharacterDesc1p)))
			return E_FAIL;
		break;
	default:
		break;
	}
	switch (i2p_2)
	{
	case 0:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_BATTLEENMU, TEXT("Layer_Tanjiro"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 1:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_BATTLEENMU, TEXT("Layer_Kyoujuro"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 2:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_BATTLEENMU, TEXT("Layer_Rui"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 3:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_BATTLEENMU, TEXT("Layer_Akaza"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 4:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Nezuko"), LEVEL_BATTLEENMU, TEXT("Layer_Nezuko"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	case 5:
		tCharacterDesc2p.bSub = true;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shinobu"), LEVEL_BATTLEENMU, TEXT("Layer_Shinobu"), &tCharacterDesc2p)))
			return E_FAIL;
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CUI_Manager);
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleEnmu::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkyBox"), LEVEL_BATTLEENMU, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_BattleEnmu::Ready_Layer_Effect(const _tchar * pLayerTag)
{


	return S_OK;
}

HRESULT CLevel_BattleEnmu::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(25.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = 1800.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_BATTLEENMU, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleEnmu::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_BATTLEENMU, pLayerTag)))
	//	return E_FAIL;

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_BATTLEENMU, pLayerTag)))
	return E_FAIL;*/

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kyoujuro"), LEVEL_BATTLEENMU, TEXT("Layer_Kyoujuro"))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_BATTLEENMU, pLayerTag)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleEnmu::Load_StaticObjects(char * pFileName)
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
		tMeshObj_StaticDesc.iCurrentLevel = LEVEL_BATTLEENMU;
		tMeshObj_StaticDesc.iModelIndex = *pMeshIndex;

		if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_BATTLEENMU, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
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
			tMeshObj_Static_InstDesc.iCurrentLevel = LEVEL_BATTLEENMU;
			tMeshObj_Static_InstDesc.iModelIndex = Pair.first;
			tMeshObj_Static_InstDesc.iNumInstancing = iNumInstancing;

			CDoor::DOOR_DESC	tCharacterDesc1p;
			tCharacterDesc1p.m_pPlayer = m_pPlayer;

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
					if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_BATTLEENMU, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
					{
						ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects_Instancing");
						continue;
					}
				}
				break;
			default:
				break;
			}
			tMeshObj_Static_InstDesc.iTypeNum = 4;
			if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static_Instancing", LEVEL_BATTLEENMU, L"Layer_MeshObj_Static_Inst", &tMeshObj_Static_InstDesc)))
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
				tMeshObj_StaticDesc.iCurrentLevel = LEVEL_BATTLEENMU;
				tMeshObj_StaticDesc.iModelIndex = Pair.first;
				tMeshObj_StaticDesc.matWorld = tMapObjDesc.matWorld;
				tMeshObj_StaticDesc.fGlowPower = tMapObjDesc.fGlowPower;
				tMeshObj_StaticDesc.bAlphaBlend = tMapObjDesc.bAlphaBlend;

		
				switch (tMeshObj_StaticDesc.iModelIndex)
				{
				case 2097:
					for (_int i = 0; i < 3; ++i)
					{
						tMeshObj_StaticDesc.iTypeNum = i;
						if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_BATTLEENMU, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
						{
							ERR_MSG(L"Failed to Load : StaticObj - Add GameObjects");
							continue;
						}
					}
					continue;
					break;
				default:
					break;
				}
				tMeshObj_StaticDesc.iTypeNum = 4;
				if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_MeshObj_Static", LEVEL_BATTLEENMU, L"Layer_MeshObj_Static", &tMeshObj_StaticDesc)))
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

HRESULT CLevel_BattleEnmu::Load_Map(const _tchar* pLayerTag, char * pFileName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTerrain::NEWTERRAINDESC tNewTerrainDesc;
	ZeroMemory(&tNewTerrainDesc, sizeof tNewTerrainDesc);

	//	strcpy_s(tTerrainDesc.strFileName, MAX_PATH, "TestMap");
	strcpy_s(tNewTerrainDesc.tTerrainDesc.strFileName, MAX_PATH, pFileName);
	if (FAILED(pGameInstance->Add_GameObject(L"Prototype_GameObject_NewTerrain", LEVEL_BATTLEENMU, pLayerTag, &tNewTerrainDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_BattleEnmu * CLevel_BattleEnmu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_BattleEnmu*	pInstance = new CLevel_BattleEnmu(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_BattleEnmu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_BattleEnmu::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
}
