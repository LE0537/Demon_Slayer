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
	
	if (m_eNextLevel == LEVEL_LOGO && m_bMiniGameIni && !g_bMiniGame)
	{
		Create_Item(fTimeDelta);
		m_bPlayerDead = false;
	}
	if (m_eNextLevel == LEVEL_LOGO && g_bMiniGame)
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
	
	if (m_eNextLevel == LEVEL_LOGO && true == m_pLoader->Get_Finished())
	{
		/* 넥스트레벨에 대한 준비가 끝나면 실제 넥스트레벨을 할당한다. */
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Mini_Player"), LEVEL_LOADING, TEXT("Layer_MINI"),&m_pMiniGame_Player)))
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
	if (m_fCreateMonsterTime > 3.f)
	{
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Mini_Monster"), LEVEL_LOADING, TEXT("Layer_MINI"), &m_pMiniGame_Player)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);
		m_fCreateMonsterTime = 0.f;
	}
	return S_OK;
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
