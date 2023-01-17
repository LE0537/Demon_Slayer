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
	CUI::LOADUIINFO				tInfo{};

	//wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();

	//ReadFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(CUI::LOADUIINFO), &dwByte, nullptr);

		if (0 == dwByte)	// ���̻� ���� �����Ͱ� ���� ���
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
	// 3. ���� �Ҹ�
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


