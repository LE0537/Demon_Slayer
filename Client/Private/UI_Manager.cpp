#include "stdafx.h"
#include "..\Public\UI_Manager.h"

#include "GameInstance.h"
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

	Load_Data();

	return S_OK;
}

void CUI_Manager::Load_Data()
{
	string FilePath = "Battle";

	string temp = "../Data/Texture/";

	FilePath = temp + FilePath + ".dat";

	wchar_t RealPath[256] = { 0 };

	for (int i = 0; i < FilePath.size(); i++)
	{
		RealPath[i] = FilePath[i];
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
	CUI::UIINFO					tInfo{};

	//wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();

	//ReadFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(CUI::UIINFO), &dwByte, nullptr);

		if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
			break;

		DATATEXTURELIST.push_back(CUI::UIINFO(tInfo));

		CUI::UIINFO CloneInfo;

		ZeroMemory(&CloneInfo, sizeof(CUI::UIINFO));

		CloneInfo.iTextureNum = CUI::UIINFO(tInfo).iTextureNum;
		CloneInfo.vPos = CUI::UIINFO(tInfo).vPos;
		CloneInfo.vScale = CUI::UIINFO(tInfo).vScale;
		CloneInfo.vRot = CUI::UIINFO(tInfo).vRot;
		CloneInfo.bReversal = CUI::UIINFO(tInfo).bReversal;

		Add_Obj(CloneInfo);
		
	}
	Safe_Release(pGameInstance);
	// 3. 파일 소멸
	CloseHandle(hFile);
}

HRESULT CUI_Manager::Add_Obj(CUI::UIINFO CloneInfo)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (CloneInfo.iTextureNum == 0)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBar"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 1)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBarDeco"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 2)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBarBack"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 3)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_GaugeBasePerson"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 4)
	{
	}
	else if (CloneInfo.iTextureNum == 5)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharIcon"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 6)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharNameUI"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 7)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillBar"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 8)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillBarParts"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 9)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ChangeBaseDeco"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 10)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ChangeSprt"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}
	else if (CloneInfo.iTextureNum == 11)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ChangeIcon"), LEVEL_GAMEPLAY, TEXT("Layer_UI"), &CloneInfo)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

void CUI_Manager::Free()
{
}


