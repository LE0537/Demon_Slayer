#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_SelectChar.h"
#include "Level_GamePlay.h"     
#include "Level_GameResult.h"
#include "Level_Menu.h"
#include "Level_StroyMenu.h"
#include "Level_AdvRui.h"
#include "Level_SelectMap.h"
#include "UI_Manager.h"
#include "SoundMgr.h"
//MiniGame
#include "Mini_Player.h"
#include "Mini_Monster.h"
#include "Mini_Tail.h"
#include "Item.h"
#include "Mini_Result.h"
CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	g_iLevel = LEVEL_LOADING;
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;


	if (m_eNextLevel == LEVEL_LOGO)
	{
		
		m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevel);
		if (nullptr == m_pLoader)
			return E_FAIL;
	}	

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_eNextLevel == LEVEL_LOGO && m_bMiniGameIni && !g_bMiniGame && m_bMiniGameStart)
	{
		Create_Item(fTimeDelta);
		m_bPlayerDead = false;
	}
	if (m_eNextLevel == LEVEL_LOGO && g_bMiniGame && m_bMiniGameStart)
	{
		if (m_bPlayerDead)
		{
			CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Mini_Player"), LEVEL_LOADING, TEXT("Layer_MINI"), &m_pMiniGame_Player)))
				return;

			RELEASE_INSTANCE(CGameInstance);
			g_bMiniGame = false;
		}
		if(!m_bPlayerDead)
			m_bPlayerDead = true;
		
	}
	if (m_eNextLevel == LEVEL_LOGO && !m_bMiniGameIni)
	{
		m_bMiniGameIni = true;
		Create_MiniGame();
	}
	if(m_eNextLevel == LEVEL_LOGO && !m_bMiniGameStart)
	{ 
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
		if (pGameInstance->Key_Down(DIK_F))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Mini_Player"), LEVEL_LOADING, TEXT("Layer_MINI"), &m_pMiniGame_Player)))
				return;
			m_bMiniGameStart = true;
			m_pMini_Result->Set_Start(true);
			Load_Score();
		}
		RELEASE_INSTANCE(CGameInstance);
	}
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Key_Down(DIK_RETURN))
	{
		Save_Score();
	}
	RELEASE_INSTANCE(CGameInstance);
	if (m_eNextLevel == LEVEL_LOGO && m_bNextLevel)
	{
		/* �ؽ�Ʈ������ ���� �غ� ������ ���� �ؽ�Ʈ������ �Ҵ��Ѵ�. */
		CLevel*			pNewLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_SELECTCHAR:
			pNewLevel = CLevel_SelectChar::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_GAMEPLAY:
			pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_GAMERESULT:
			pNewLevel = CLevel_GameResult::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_MENU:
			pNewLevel = CLevel_Menu::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_STORYMENU:
			pNewLevel = CLevel_StoryMenu::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_ADVRUI:
			pNewLevel = CLevel_AdvRui::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_SELECTMAP:
			pNewLevel = CLevel_SelectMap::Create(m_pDevice, m_pContext);
			break;
		}

		if (nullptr == pNewLevel)
			return;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		if (nullptr == pGameInstance)
			return;
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
			return;

		Safe_Release(pGameInstance);
				
	}

}

void CLevel_Loading::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());
}

HRESULT CLevel_Loading::Create_MiniGame()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mini_Player"),
		CMini_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mini_Monster"),
		CMini_Monster::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mini_Tail"),
		CMini_Tail::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item"),
		CItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MIni_Result"),
		CMini_Result::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MIni_Result"), LEVEL_LOADING, TEXT("Layer_MINI_Result"), &m_pMini_Result)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Create_Item(_float fTimeDelta)
{
	m_fCreateItemTime += fTimeDelta;
	if (m_fCreateItemTime > 1.f)
	{
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_LOADING, TEXT("Layer_MINI"), &m_pMiniGame_Player)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		m_fCreateItemTime = 0.f;
	}
	m_fCreateMonsterTime += fTimeDelta;
	if (m_fCreateMonsterTime > 2.f)
	{
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Mini_Monster"), LEVEL_LOADING, TEXT("Layer_MINI"), &m_pMiniGame_Player)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);
		m_fCreateMonsterTime = 0.f;
	}
	return S_OK;
}

void CLevel_Loading::Save_Score()
{
	string FilePath = "RankScore";

	string temp = "../Data/MiniGameData/";

	string dat = ".dat";

	FilePath = temp + FilePath + dat;

	wchar_t RealPath[256] = { 0 };

	for (int i = 0; i < FilePath.size(); i++)
	{
		RealPath[i] = FilePath[i];
	}


	HANDLE		hFile = CreateFile(RealPath,			// ���� ��ο� �̸� ���
		GENERIC_WRITE,				// ���� ���� ��� (GENERIC_WRITE ���� ����, GENERIC_READ �б� ����)
		NULL,						// �������, ������ �����ִ� ���¿��� �ٸ� ���μ����� ������ �� ����� ���ΰ�, NULL�� ��� �������� �ʴ´�
		NULL,						// ���� �Ӽ�, �⺻��	
		CREATE_ALWAYS,				// ���� ���, CREATE_ALWAYS�� ������ ���ٸ� ����, �ִٸ� ���� ����, OPEN_EXISTING ������ ���� ��쿡�� ����
		FILE_ATTRIBUTE_NORMAL,		// ���� �Ӽ�(�б� ����, ���� ��), FILE_ATTRIBUTE_NORMAL �ƹ��� �Ӽ��� ���� �Ϲ� ���� ����
		NULL);						// �������� ������ �Ӽ��� ������ ���ø� ����, �츮�� ������� �ʾƼ� NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// �˾� â�� ������ִ� ����� �Լ�
		// 1. �ڵ� 2. �˾� â�� �������ϴ� �޽��� 3. �˾� â �̸� 4. ��ư �Ӽ�
		MessageBox(g_hWnd, TEXT("Save File"), TEXT("Fail"), MB_OK);
		return;
	}

	// 2. ���� ����

	DWORD		dwByte = 0;
	_int		iRankScore = m_pMini_Result->Get_RankScore();

	WriteFile(hFile, &iRankScore, sizeof(_int), &dwByte, nullptr);
	


	// 3. ���� �Ҹ�
	CloseHandle(hFile);

	m_bNextLevel = m_pLoader->Get_Finished();
}

void CLevel_Loading::Load_Score()
{
	HANDLE		hFile = CreateFile(L"../Data/MiniGameData/RankScore.dat",			// ���� ��ο� �̸� ���
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
		MessageBox(g_hWnd, TEXT("Load File"), TEXT("Fail"), MB_OK);
		return;
	}

	// 2. ���� ����

	DWORD		dwByte = 0;
	_int		iRankScore = 0;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	while (true)
	{
		ReadFile(hFile, &iRankScore, sizeof(_int), &dwByte, nullptr);

		if (0 == dwByte)	// ���̻� ���� �����Ͱ� ���� ���
			break;
		
		m_pMini_Result->Set_RankScore(iRankScore);
	}

	Safe_Release(pGameInstance);
	// 3. ���� �Ҹ�
	CloseHandle(hFile);
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
