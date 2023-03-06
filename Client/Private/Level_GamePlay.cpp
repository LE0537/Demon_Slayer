#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

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

#include "Effect_Manager.h"
#include "ImGuiManager.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Akaza.h"
unsigned int APIENTRY Thread_GamePlay(void* pArg)
{
	CLevel_GamePlay*		pLoader = (CLevel_GamePlay*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	pUIManager->Set_CharNameUIZero();

	pLoader->Ready_Lights();
	g_fLoading = 10.f;
	pLoader->Ready_Layer_Camera(TEXT("Layer_Camera"));
	g_fLoading = 30.f;
	pLoader->Ready_Layer_Player(TEXT("Layer_Player"));
	g_fLoading = 40.f;
	pLoader->Ready_Layer_BackGround(TEXT("Layer_BackGround"));
	g_fLoading = 50.f;
	pLoader->Load_Map(L"Layer_Terrain", "11_Map_Rui");
	g_fLoading = 60.f;
	if (pUIManager->Get_SelMapNum() == 0)
	{
		pLoader->Load_StaticObjects("11_Rui");
	}
	else if (pUIManager->Get_SelMapNum() == 1)
	{
		pLoader->Load_StaticObjects("TrainBattle");
	}
	g_fLoading = 80.f;

	pLoader->Ready_Layer_Effect(TEXT("Layer_Effect"));

	if (!pUIManager->Get_BattleTypeCheck() && pUIManager->Get_SaveStory())
	{
		pLoader->Battle_Dialog(TEXT("Layer_Dialog"));
	}
	g_fLoading = 100.f;
	CComponent* pOut = pGameInstance->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Renderer");
	pLoader->Set_Renderer((CRenderer*)pOut);


	if (pUIManager->Get_SelMapNum() == 0)
	{
		_float fValue[CRenderer::VALUE_END] = { 0.15f, 0.2f ,0.15f ,75.f ,87.f ,0.85f, 0.2f, 1.36f,0.4f, 1.f,20.f, 200.f, 0.05f, 1.6f, 0.5f, 1.f, 3.f, 0.7f, 1.f, 0.3f, 15.f, 0.1f };
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_R), 0.15f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_G), 0.2f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_B), 0.15f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGDISTANCE), 75.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGRANGE), 87.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGMINPOWER), 0.85f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_CUBEMAPFOG), 0.2f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_AO), 1.36f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_AORADIUS), 0.4f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_GLOWBLURCOUNT), 1.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_DISTORTION), 20.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_OUTLINE), 200.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_INNERLINE), 0.05f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_ENVLIGHT), 1.6f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT), 0.5f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT_TESTLENGTH), 1.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT_MINUS), 3.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTPOWER), 0.7f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_SHADOWTESTLENGTH), 1.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_PLC_SHADOW), 0.3f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_MAPGRAYSCALETIME), 15.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_SHADOWPOWER), 0.1f);
		pLoader->Get_Renderer()->ReturnValue();
		pLoader->Get_Renderer()->Set_Far(g_fFar);

		for (_int i = 0; i < CRenderer::VALUE_END; ++i)
			CImGuiManager::Get_Instance()->Setting_PostProcessingValue(i, fValue[i]);
	}
	else if (pUIManager->Get_SelMapNum() == 1)
	{
		_float fValue[CRenderer::VALUE_END] = { 0.11f, 0.15f ,0.3f ,71.f ,171.f ,0.85f, 0.2f, 1.36f,0.4f, 1.f,20.f, 200.f, 0.05f, 1.6f, 0.5f, 1.f, 3.f, 0.55f, 1.f, 0.3f, 15.f, 0.1f };
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_R), 0.11f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_G), 0.15f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGCOLOR_B), 0.3f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGDISTANCE), 71.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGRANGE), 171.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_FOGMINPOWER), 0.85f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_CUBEMAPFOG), 0.2f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_AO), 1.36f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_AORADIUS), 0.4f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_GLOWBLURCOUNT), 1.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_DISTORTION), 20.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_OUTLINE), 200.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_INNERLINE), 0.05f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_ENVLIGHT), 1.6f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT), 0.5f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT_TESTLENGTH), 1.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTSHAFT_MINUS), 3.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_LIGHTPOWER), 0.55f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_SHADOWTESTLENGTH), 1.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_PLC_SHADOW), 0.3f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_MAPGRAYSCALETIME), 15.f);
		pLoader->Get_Renderer()->Set_OriginValue(CRenderer::VALUETYPE(CRenderer::VALUE_SHADOWPOWER), 0.1f);
		pLoader->Get_Renderer()->ReturnValue();
		pLoader->Get_Renderer()->Set_Far(g_fFar);

		for (_int i = 0; i < CRenderer::VALUE_END; ++i)
			CImGuiManager::Get_Instance()->Setting_PostProcessingValue(i, fValue[i]);
	}


	if (pUIManager->Get_BattleTypeCheck())
	{
		CSoundMgr::Get_Instance()->BGM_Stop();
		CSoundMgr::Get_Instance()->PlayBGM(TEXT("PlayerBattle.wav"), g_fBGM);
	}

	RELEASE_INSTANCE(CGameInstance);
	


	LeaveCriticalSection(&pLoader->Get_CriticalSection());
	pLoader->Set_Finished();
	g_bThread = false;
	pUIManager->Set_LoadingDead();
	RELEASE_INSTANCE(CUI_Manager);
	return 0;
}
CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	g_iLevel = LEVEL_GAMEPLAY;
	g_bThread = true;
	g_fLoading = 0.f;
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

	pUIManager->Add_Loading();

	CoInitializeEx(nullptr, 0);

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_GamePlay, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;


	g_fFar = 1800.f;

	RELEASE_INSTANCE(CUI_Manager);

	
	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
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
		if (m_bCheckADVAkaza)
		{
			if (!m_bCinema)
			{
				m_bCinema = true;
				CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_StoryScene(CCamera_Dynamic::STORYSCENE_ADV_AKAZA);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_QuestBattleCam(true);
				RELEASE_INSTANCE(CGameInstance);
			}
		}
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

		if (!m_bCreateUI)
		{
			if (pUIManager->Get_BattleTypeCheck())
			{
				_bool bOniCheck = pUIManager->P1_Oni_Check();
				if (!bOniCheck)
					pUIManager->Add_P1_PersonHpUI();
				else
					pUIManager->Add_P1_OniHpUI();

				bOniCheck = pUIManager->P2_Oni_Check();
				if (!bOniCheck)
					pUIManager->Add_P2_PersonHpUI();
				else
					pUIManager->Add_P2_OniHpUI();

				pUIManager->Add_BattleUI();
				pUIManager->Add_P1_Combo();
				pUIManager->Add_P2_Combo();
			}
			else
			{
				pUIManager->Add_P1_PersonHpUI();
				pUIManager->Add_P2_OniHpUI();
				pUIManager->Add_P1_Combo();
				pUIManager->Add_P2_Combo();
				pUIManager->Add_AdvBattleUI();
				pUIManager->Add_AdvResult(LEVEL_GAMEPLAY);
			}

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
		else
		{
			if (pUIManager->Get_2P()->Get_PlayerInfo().iHp <= 0 && pUIManager->Get_2P()->Get_PlayerInfo().strName == TEXT("아빠 거미"))
			{
				m_fNextLevelTime += fTimeDelta;
				if (m_fNextLevelTime > 3.f && !pUIManager->Get_AdvResult())
					pUIManager->Set_FadeIn();
				else if (pUIManager->Get_AdvResult())
				{
					pUIManager->Set_SaveStory(true);
					pUIManager->Set_RuiDadBattle(false);
					pUIManager->Set_AdvResult(false);
					pUIManager->Set_FadeOut();
					if (FAILED(pGameInstance->Open_Level(LEVEL_ADVRUI, CLevel_AdvRui::Create(m_pDevice, m_pContext))))
					return;
				}
			}
			else if(pUIManager->Get_StroyEventEnd())
			{
				m_fNextLevelTime += fTimeDelta;
				if (m_fNextLevelTime > 15.f && !pUIManager->Get_AdvResult())
					pUIManager->Set_FadeIn();
				else if (pUIManager->Get_AdvResult() && pUIManager->Get_StroyEventEnd())
				{
					pUIManager->Set_AdvResult(false);
					pUIManager->Set_FadeOut();
					if (FAILED(pGameInstance->Open_Level(LEVEL_STORYMENU, CLevel_StoryMenu::Create(m_pDevice, m_pContext))))
						return;
				}
			}
			else if (pUIManager->Get_2P()->Get_PlayerInfo().iHp <= 0 && pUIManager->Get_2P()->Get_PlayerInfo().strName == TEXT("아카자"))
			{
				if(pUIManager->Get_StroyEventEnd())
					m_fNextLevelTime += fTimeDelta;

				if (!m_bCinemaEnd)
				{
					CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
					dynamic_cast<CAkaza*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Akaza"))->Get_LayerFront())->Set_AiMode(false);
					dynamic_cast<CKyoujuro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Kyoujuro"))->Get_LayerFront())->Set_StorySpl();
					RELEASE_INSTANCE(CGameInstance);
					m_bCinemaEnd = true;
					g_bDeathTime = false;
				}
				if (m_fNextLevelTime > 15.f && !pUIManager->Get_AdvResult())
					pUIManager->Set_FadeIn();
				else if (pUIManager->Get_AdvResult())
				{
					pUIManager->Set_AdvResult(false);
					pUIManager->Set_FadeOut();
					if (FAILED(pGameInstance->Open_Level(LEVEL_STORYMENU, CLevel_StoryMenu::Create(m_pDevice, m_pContext))))
						return;
				}
			}
		}

		RELEASE_INSTANCE(CUI_Manager);
		RELEASE_INSTANCE(CGameInstance);
	}
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


		//tCharacterDesc2p.i1P2P = 33;
		//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), &tCharacterDesc2p)))
		//	return E_FAIL;

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
	case 6:
		tCharacterDesc2p.i1P2P = 11;
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiDad"), LEVEL_GAMEPLAY, TEXT("Layer_RuiDad"), &tCharacterDesc2p)))
			return E_FAIL;
		CSoundMgr::Get_Instance()->BGM_Stop();
		CSoundMgr::Get_Instance()->PlayBGM(TEXT("Battle_RuiDad.wav"), g_fBGM);
		break;
	case 7:
		tCharacterDesc2p.i1P2P = 11;
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rui"), LEVEL_GAMEPLAY, TEXT("Layer_Rui"), &tCharacterDesc2p)))
			return E_FAIL;
		CSoundMgr::Get_Instance()->BGM_Stop();
		CSoundMgr::Get_Instance()->PlayBGM(TEXT("Battle_Rui.wav"), g_fBGM);
		break;
	case 8:
		tCharacterDesc2p.i1P2P = 11;
		tCharacterDesc2p.bSub = false;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Akaza"), LEVEL_GAMEPLAY, TEXT("Layer_Akaza"), &tCharacterDesc2p)))
			return E_FAIL;
		m_bCheckADVAkaza = true;
		tCharacterDesc2p.i1P2P = 33;

		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_ADVAkaza();

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tanjiro"), LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), &tCharacterDesc2p)))
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
	CameraDesc.CameraDesc.fFar = g_fFar;

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

HRESULT CLevel_GamePlay::Battle_Dialog(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BattleDialog"), g_iLevel, pLayerTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

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

	Safe_Release(m_pRendererCom);
}
