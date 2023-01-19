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
		else if (sLoadName == "Change_Select")
			SELECT_LOADDATALIST.push_back(CUI::LOADUIINFO(tInfo));
	}

	RELEASE_INSTANCE(CGameInstance);
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

	//for (auto iter : P1_P_LOADDATALIST)
	//	Safe_Release(iter);

	//P1_P_LOADDATALIST.clear();
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

	/*for (auto iter : P1_O_LOADDATALIST)
		Safe_Release(iter);

	P1_O_LOADDATALIST.clear();*/
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

	/*for (auto iter : P2_P_LOADDATALIST)
		Safe_Release(iter);

	P2_P_LOADDATALIST.clear();*/
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

void CUI_Manager::Add_Select_CharUI()
{
	for (auto iter : SELECT_LOADDATALIST)
	{
		m_ThrowInfo.bReversal = iter.bReversal;
		m_ThrowInfo.iTextureNum = iter.iTextureNum;
		m_ThrowInfo.vPos = iter.vPos;
		m_ThrowInfo.vRot = iter.vRot;
		m_ThrowInfo.vScale = iter.vScale;
		m_ThrowInfo.iLevelIndex = LEVEL_SELECTCHAR;

		SELECT_DATALIST.push_back(m_ThrowInfo);
	}

	for (auto iter : SELECT_DATALIST)
		Add_SelectUI(iter);
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
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharFrameShadow"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 18:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharFrame"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
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
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_P1Cursor"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 21:
	{
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
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_1P_Icon"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 24:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_2P_Icon"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
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
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_1P_MainOnBase"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 27:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_2P_MainOnBase"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}
	case 28:
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CharNameUI"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &iter)))
			return E_FAIL;
		break;
	}

	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

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