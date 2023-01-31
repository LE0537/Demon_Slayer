#include "stdafx.h"
#include "..\Public\UI_Manager.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Tanjiro.h"
//#include "Level_Manager.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Init(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CUI_Manager::Load_Data(string sLoadName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	/*P1_P_DATALIST.clear();
	P1_O_DATALIST.clear();
	P2_P_DATALIST.clear();
	P2_O_DATALIST.clear();*/

	m_sFilePath = sLoadName;

	string temp = "../Data/Texture/";

	m_sFilePath = temp + m_sFilePath + ".dat";

	wchar_t RealPath[256] = { 0 };

	for (int i = 0; i < m_sFilePath.size(); i++)
	{
		RealPath[i] = m_sFilePath[i];
	}

	HANDLE		hFile = CreateFile(RealPath,			// 파일 경로와 이름 명시
		GENERIC_READ,				// 파일 접근 모드 (GENERIC_WRITE 쓰기 전용, GENERIC_READ 읽기 전용)
		NULL,						// 공유방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가, NULL인 경우 공유하지 않는다
		NULL,						// 보안 속성, 기본값	
		OPEN_EXISTING,				// 생성 방식, CREATE_ALWAYS는 파일이 없다면 생성, 있다면 덮어 쓰기, OPEN_EXISTING 파일이 있을 경우에면 열기
		FILE_ATTRIBUTE_NORMAL,		// 파일 속성(읽기 전용, 숨기 등), FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반 파일 생성
		NULL);						// 생성도리 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않아서 NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// 팝업 창을 출력해주는 기능의 함수
		// 1. 핸들 2. 팝업 창에 띄우고자하는 메시지 3. 팝업 창 이름 4. 버튼 속성
		//MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return;
	}

	// 2. 파일 쓰기

	DWORD						dwByte = 0;
	CUI::LOADUIINFO				tInfo{};

	//wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();

	//ReadFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);



	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(CUI::LOADUIINFO), &dwByte, nullptr);

		if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
			break;

		//List.push_back(CUI::UIINFO(tInfo));


		if (sLoadName == "P1_Person_BtlUI")
			P1_P_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "P1_Oni_BtlUI")
			P1_O_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "P2_Person_BtlUI")
			P2_P_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "P2_Oni_BtlUI")
			P2_O_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "Change_Select")
			SELECT_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "LogoTitle")
			LOGOTITLE_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "Menu")
			MENU_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "Loading")
			LOADING_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "BattleUI")
			BATTLEUI_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
	}

	RELEASE_INSTANCE(CGameInstance);
	// 3. 파일 소멸
	CloseHandle(hFile);
}

void CUI_Manager::Add_P1_PersonHpUI()
{
	for (auto iter : P1_P_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.pTarget = m_p1P;
		m_ThrowInfo.pTargetSecond = m_p1P_2;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.bPlyCheck = false;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_GAMEPLAY;

		P1_P_DATALIST.push_back(m_ThrowInfo);	
	}

	for (auto iter : P1_P_DATALIST)
		Add_Btl_PlayerUI(iter);

	m_iUltBarLayerNum = 0;
	m_iCharNameLayerNum = 0;
	m_iCharIconLayerNum = 0;
}

void CUI_Manager::Add_P1_OniHpUI()
{
	for (auto iter : P1_O_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.pTarget = m_p1P;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.bPlyCheck = false;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_GAMEPLAY;

		P1_O_DATALIST.push_back(m_ThrowInfo);	
	}

	for (auto iter : P1_O_DATALIST)
		Add_Btl_PlayerUI(iter);
	
	m_iUltBarLayerNum = 0;
	m_iCharNameLayerNum = 0;
	m_iCharIconLayerNum = 0;
}

void CUI_Manager::Add_P2_PersonHpUI()
{
	for (auto iter : P2_P_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.pTarget = m_p2P;
		m_ThrowInfo.pTargetSecond = m_p2P_2;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.bPlyCheck = true;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_GAMEPLAY;

		P2_P_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : P2_P_DATALIST)
		Add_Btl_PlayerUI(iter);
	
	m_iUltBarLayerNum = 0;
	m_iCharNameLayerNum = 0;
	m_iCharIconLayerNum = 0;
}

void CUI_Manager::Add_P2_OniHpUI()
{
	for (auto iter : P2_O_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.pTarget = m_p2P;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.bPlyCheck = true;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_GAMEPLAY;

		P2_O_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : P2_O_DATALIST)
		Add_Btl_PlayerUI(iter);

	m_iUltBarLayerNum = 0;
	m_iCharNameLayerNum = 0;
	m_iCharIconLayerNum = 0;
}

void CUI_Manager::Add_Select_CharUI()
{
	for (auto iter : SELECT_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.bSelCheck = false;
		m_ThrowInfo.iLevelIndex = LEVEL_SELECTCHAR;

		SELECT_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : SELECT_DATALIST)
		Add_SelectUI(iter);

	m_iCharIconLayerNum = 0;
	m_iCharNameLayerNum = 0;
	m_iCharFrameLayerNum = 0;
	m_i1PIconLayerNum = 0;
	m_i2PIconLayerNum = 0;
	m_i1PMainLayerNum = 0;
	m_i2PMainLayerNum = 0;
	m_iSelStampLayerNum = 0;
}

void CUI_Manager::Add_Logo_Title()
{
	for (auto iter : LOGOTITLE_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_LOGO;

		LOGOTITLE_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : LOGOTITLE_DATALIST)
		Add_LogoUI(iter);
}

void CUI_Manager::Add_Menu()
{
	for (auto iter : MENU_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_LOGO;

		MENU_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : MENU_DATALIST)
		Add_MenuUI(iter);
}

void CUI_Manager::Add_Loading()
{
	for (auto iter : LOADING_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_LOADING;

		LOADING_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : LOADING_DATALIST)
		Add_LoadingUI(iter);
}

void CUI_Manager::Add_BattleUI()
{
	for (auto iter : BATTLEUI_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.pTarget = m_p1P;
		m_ThrowInfo.pTargetSecond = m_p2P;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_GAMEPLAY;

		BATTLEUI_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : BATTLEUI_DATALIST)
		Add_Btl_PlayerUI(iter);

	m_iTimerLayerNum = 0;
}

HRESULT CUI_Manager::Add_Btl_PlayerUI(CUI::THROWUIINFO iter)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	switch (iter.iTextureNum)
	{
	case 0:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GaugeLight"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 1:
		iter.iLayerNum = m_iCharIconLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharIcon"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_iCharIconLayerNum;
		break;
	case 2: //ChangeBase 0
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 3: // ChangeSprt 1
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 4:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GaugeBase"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 5:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GaugeBase"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 6://HpbarBack 2
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 7: //SkillBarBack 3
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 8:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBar"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 9:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillBar"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 10: //HpBarDeco 4
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 11: //SkillBarParts 5
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 12: //SkillBarDeco 6
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 13:
		iter.iLayerNum = m_iCharNameLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharNameUI"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_iCharNameLayerNum;
		break;
	case 14: // PersonChangeBarBack 7
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 15: // PersonChangeBarFrame 8
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 16:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PersonChanBarEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 17: // OniChangeBarFrame 9
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 18:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_OniChanBarEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 19:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_OniChanBarEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 20:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ChangeIcon"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 21:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_KeyUI"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 22: // 사람 얼티밋 게이지 베이스 10 
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 23: //오니 얼티밋 게이지 베이스 11
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 24:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 25:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 26:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 27:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 28:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 29:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockFrame"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 30:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockBase"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 31:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockNum"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 32:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltStockEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 33:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltGaugeDeco"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 34:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltGaugeDeco"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 35:
		iter.iLayerNum = m_iUltBarLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UltBar"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_iUltBarLayerNum;
		break;
	case 36: //UltBarDeco 12
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BtlFixedImg"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 37: 
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PersonChanBarEff"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 38:
		iter.iLayerNum = m_iTimerLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NumTimer"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_iTimerLayerNum;
		break;
	case 39:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RoundIcon"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	case 40:
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RoundUI"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	default:
		break;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CUI_Manager::Add_SelectUI(CUI::THROWUIINFO iter)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (iter.iTextureNum)
	{
	case 0: //HpBar
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SelectBg"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 1:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PatternWind"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 2:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PatternOne"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 3:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CloudUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 4:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CloudUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 5:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CloudUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 6:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CloudUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 7:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CloudUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 8:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerNumIcon"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 9:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerNumIcon"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 10:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharSelBg"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 11:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CloudUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 12:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CloudUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 13:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WindowBase"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 14:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WindowLeft"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 15:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WindowRight"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 16:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharFrameEff"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 17:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SleNameShadow"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 18:
	{
		iter.iLayerNum = m_iCharFrameLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharFrame"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_iCharFrameLayerNum;
		break;
	}
	case 19:
	{
		iter.iLayerNum = m_iCharIconLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharIcon"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_iCharIconLayerNum;
		break;
	}
	case 20:
	{
		iter.iLayerNum = 0;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_P1Cursor"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 21:
	{
		iter.iLayerNum = 1;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_P2Cursor"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 22:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_OniIcon"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 23:
	{
		iter.iLayerNum = m_i1PIconLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_1P_Icon"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_i1PIconLayerNum;
		break;
	}
	case 24:
	{
		iter.iLayerNum = m_i2PIconLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_2P_Icon"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_i2PIconLayerNum;
		break;
	}
	case 25:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_OniLight"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 26:
	{
		iter.iLayerNum = m_i1PMainLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_1P_MainOnBase"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_i1PMainLayerNum;
		break;
	}
	case 27:
	{
		iter.iLayerNum = m_i2PMainLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_2P_MainOnBase"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_i2PMainLayerNum;
		break;
	}
	case 28:
	{
		iter.iLayerNum = m_iCharNameLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharNameUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_iCharNameLayerNum;
		break;
	}
	case 29:
	{
		iter.iLayerNum = m_iSelStampLayerNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SelStamp"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		++m_iSelStampLayerNum;
		break;
	}

	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUI_Manager::Add_LogoUI(CUI::THROWUIINFO iter)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	switch (iter.iTextureNum)
	{
	case 0: 
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LogoFixedImg"), LEVEL_LOGO, TEXT("Layer_LogoUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 1:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LogoBackEff"), LEVEL_LOGO, TEXT("Layer_LogoUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 2:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LogoBackLight"), LEVEL_LOGO, TEXT("Layer_LogoUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 3:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LogoFixedImg"), LEVEL_LOGO, TEXT("Layer_LogoUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 4:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LogoFixedImg"), LEVEL_LOGO, TEXT("Layer_LogoUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 5:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LogoFixedImg"), LEVEL_LOGO, TEXT("Layer_LogoUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 6:
	{
		iter.iLayerNum = m_iLogoButtonNum;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LogoButton"), LEVEL_LOGO, TEXT("Layer_LogoUI"), &iter)))
			return E_FAIL;
		++m_iLogoButtonNum;
		break;
	}
	default:
		break;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CUI_Manager::Add_MenuUI(CUI::THROWUIINFO iter)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	switch (iter.iTextureNum)
	{
	case 0:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuChar"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 1:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuCursor"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 2:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuFixedImg"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 3:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuBackDeco"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 4:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuFixedImg"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 5:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuFixedImg"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 6:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuDarkCloud"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 7:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuDarkCloud"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 8:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuFrontCloud"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 9:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuFrontCloud"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 10:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuFixedImg"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 11:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuTitle"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 16:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuTitle"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 17:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PatternWind"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 18:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PatternOne"), LEVEL_LOGO, TEXT("Layer_MenuUI"), &iter)))
			return E_FAIL;
		break;
	}
	default:
		break;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CUI_Manager::Add_LoadingUI(CUI::THROWUIINFO iter)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	switch (iter.iTextureNum)
	{
	case 0:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingFixedImg"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 1:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingCloud"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 2:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingCloud"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 3:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingShoji"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 4:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingShojiLeft"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 5:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingShojiRight"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 6:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingBaseTxt"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 7:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingBaseTitle"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 8:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingFixedImg"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 9:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingAnim"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 10:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingFixedImg"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 11:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingFixedImg"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	case 12:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_LoadingBar"), LEVEL_LOADING, TEXT("Layer_LoadingUI"), &iter)))
			return E_FAIL;
		break;
	}
	default:
		break;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

_bool CUI_Manager::P1_Oni_Check()
{
	return  m_p1P->Get_PlayerInfo().bOni;
}

_bool CUI_Manager::P2_Oni_Check()
{
	return  m_p2P->Get_PlayerInfo().bOni;
}

void CUI_Manager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	P1_P_LOADDATALIST.clear();
	P1_O_LOADDATALIST.clear();
	P2_P_LOADDATALIST.clear();
	P2_O_LOADDATALIST.clear();
	SELECT_LOADDATALIST.clear();

	P1_P_DATALIST.clear();
	P1_O_DATALIST.clear();
	P2_P_DATALIST.clear();
	P2_O_DATALIST.clear();
	SELECT_DATALIST.clear();
}