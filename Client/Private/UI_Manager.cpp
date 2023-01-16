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

	HANDLE		hFile = CreateFile(RealPath,			// ���� ��ο� �̸� ���
		GENERIC_READ,				// ���� ���� ��� (GENERIC_WRITE ���� ����, GENERIC_READ �б� ����)
		NULL,						// �������, ������ �����ִ� ���¿��� �ٸ� ���μ����� ������ �� ����� ���ΰ�, NULL�� ��� �������� �ʴ´�
		NULL,						// ���� �Ӽ�, �⺻��	
		OPEN_EXISTING,				// ���� ���, CREATE_ALWAYS�� ������ ���ٸ� ����, �ִٸ� ���� ����, OPEN_EXISTING ������ ���� ��쿡�� ����
		FILE_ATTRIBUTE_NORMAL,		// ���� �Ӽ�(�б� ����, ���� ��), FILE_ATTRIBUTE_NORMAL �ƹ��� �Ӽ��� ���� �Ϲ� ���� ����
		NULL);						// �������� ������ �Ӽ��� ������ ���ø� ����, �츮�� ������� �ʾƼ� NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// �˾� â�� ������ִ� ����� �Լ�
		// 1. �ڵ� 2. �˾� â�� �������ϴ� �޽��� 3. �˾� â �̸� 4. ��ư �Ӽ�
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return;
	}

	// 2. ���� ����

	DWORD						dwByte = 0;
	CUI::UIINFO					tInfo{};

	//wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();

	//ReadFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(CUI::UIINFO), &dwByte, nullptr);

		if (0 == dwByte)	// ���̻� ���� �����Ͱ� ���� ���
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
	// 3. ���� �Ҹ�
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


