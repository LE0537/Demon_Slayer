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
#include "UI_Manager.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	if(eNextLevel != LEVEL_MENU)
		pUIManager->Add_Loading();

	RELEASE_INSTANCE(CUI_Manager);
	
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	if (true == m_pLoader->Get_Finished())
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
