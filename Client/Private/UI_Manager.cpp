#include "stdafx.h"
#include "..\Public\UI_Manager.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Tanjiro.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Init(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, string sLoadName)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	
	Load_Data(sLoadName);

	//if (sLoadName == "P1_Person_HpUI")
	//{
	//	Load_Data(sLoadName, P1_P_DATALIST);
	//}
	//else if (sLoadName == "P1_Oni_HpUI")
	//{
	//	Load_Data(sLoadName, P1_O_DATALIST);
	//}
	//else if (sLoadName == "P2_Person_HpUI")
	//{
	//	Load_Data(sLoadName, P2_P_DATALIST);
	//}
	//else if (sLoadName == "P2_Oni_HpUI")
	//{
	//	Load_Data(sLoadName, P2_O_DATALIST);
	//}

	return S_OK;
}

void CUI_Manager::Load_Data(string sLoadName)
{
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
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return;
	}

	// 2. 파일 쓰기

	DWORD						dwByte = 0;
	CUI::LOADUIINFO				tInfo{};

	//wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();

	//ReadFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(CUI::LOADUIINFO), &dwByte, nullptr);

		if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
			break;

		//List.push_back(CUI::UIINFO(tInfo));


		if (sLoadName == "P1_Person_HpUI")
			P1_P_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "P1_Oni_HpUI")
			P1_O_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "P2_Person_HpUI")
			P2_P_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
		else if (sLoadName == "P2_Oni_HpUI")
			P2_O_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
	
	}
	Safe_Release(pGameInstance);
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
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.vScale = iter.vScale;

		P1_P_DATALIST.push_back(m_ThrowInfo);	
	}

	for (auto iter : P1_P_DATALIST)
		Add_Obj(iter);
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
		m_ThrowInfo.vScale = iter.vScale;
		
		P1_O_DATALIST.push_back(m_ThrowInfo);	
	}

	for (auto iter : P1_O_DATALIST)
		Add_Obj(iter);
}

void CUI_Manager::Add_P2_PersonHpUI()
{
	for (auto iter : P2_P_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.pTarget = m_p2P;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.vScale = iter.vScale;

		P2_P_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : P2_P_DATALIST)
		Add_Obj(iter);
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
		m_ThrowInfo.vScale = iter.vScale;

		P2_O_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : P2_O_DATALIST)
		Add_Obj(iter);
}

HRESULT CUI_Manager::Add_Obj(CUI::THROWUIINFO iter)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	switch (iter.iTextureNum)
	{
	case 0: //HpBar
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBar"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 1:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBarDeco"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 2:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBarBack"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 3:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GaugeBasePerson"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 4:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBarBack"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 5:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharIcon"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 6:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharNameUI"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 7:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillBar"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 8:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillBarParts"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 9:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ChangeBaseDeco"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 10:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ChangeSprt"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
				return E_FAIL;
			break;
		}
	case 11:
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ChangeIcon"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &iter)))
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
}


