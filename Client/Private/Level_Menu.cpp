#include "stdafx.h"
#include "Level_Menu.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "SoundMgr.h"
#include "Level_SelectChar.h"
#include "Level_StroyMenu.h"

CLevel_Menu::CLevel_Menu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Menu::Initialize()
{
	g_iLevel = LEVEL_MENU;
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	_uint iNum = 1;		
	pUI_Manager->Add_Menu();
	Ready_Layer_InkEff();
	pUI_Manager->Set_LevelResultOn(false);
	pUI_Manager->RankInfo_ZeroMemory(0);
	pUI_Manager->RankInfo_ZeroMemory(1);
	RELEASE_INSTANCE(CUI_Manager);

	if (FAILED(__super::Initialize()))
		return E_FAIL;
	CSoundMgr::Get_Instance()->BGM_Stop();
	CSoundMgr::Get_Instance()->PlayBGM(TEXT("ModeSel.wav"), g_fBGM);
	return S_OK;
}

void CLevel_Menu::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	
	if (pUI_Manager->Get_MenuCursor() != nullptr)
	{
		if (pUI_Manager->Get_MenuCursor()->Get_SelectVS())
		{
			pUI_Manager->Set_BattleTypeCheck(true);
			if (FAILED(pGameInstance->Open_Level(LEVEL_SELECTCHAR, CLevel_SelectChar::Create(m_pDevice, m_pContext))))
				return;
			CSoundMgr::Get_Instance()->BGM_Stop();
		}
		else if (pUI_Manager->Get_MenuCursor()->Get_SelectStoryMenu())
		{
			pUI_Manager->Set_BattleTypeCheck(false);
			if (FAILED(pGameInstance->Open_Level(LEVEL_STORYMENU, CLevel_StoryMenu::Create(m_pDevice, m_pContext))))
				return;
			CSoundMgr::Get_Instance()->BGM_Stop();
		}
	}


	Safe_Release(pGameInstance);

	RELEASE_INSTANCE(CUI_Manager);
}

void CLevel_Menu::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_Menu::Ready_Layer_InkEff()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_uint iNum = 1;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InkEff"), LEVEL_MENU, TEXT("Layer_MenuUI"), &iNum)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_Menu * CLevel_Menu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Menu*	pInstance = new CLevel_Menu(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Menu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Menu::Free()
{
	__super::Free();


}
