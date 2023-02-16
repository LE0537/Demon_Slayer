#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "LogoButton.h"
#include "SoundMgr.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

	pUIManager->Load_Data("P1_Person_BtlUI");
	pUIManager->Load_Data("P1_Oni_BtlUI");
	pUIManager->Load_Data("P2_Person_BtlUI");
	pUIManager->Load_Data("P2_Oni_BtlUI");
	pUIManager->Load_Data("BattleUI");
	pUIManager->Load_Data("1P_Combo");
	pUIManager->Load_Data("2P_Combo");
	pUIManager->Load_Data("BattleResult");
	pUIManager->Load_Data("Char_Select");
	pUIManager->Load_Data("LogoTitle");
	pUIManager->Load_Data("Menu");
	pUIManager->Load_Data("Loading");
	pUIManager->Load_Data("Adc_Menu");
	pUIManager->Load_Data("MapSelect");

	CSoundMgr::Get_Instance()->PlayBGM(TEXT("Title.wav"), fBGM);
	CSoundMgr::Get_Instance()->PlayEffect(TEXT("LogoStart.wav"), fEFFECT);
	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

	if (!m_bCreateUI)
	{
		pUIManager->Add_Logo_Title();
		m_bCreateUI = true;
	}
	
	if (pUIManager->Get_LevelMenuOn())
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_MENU))))
			return;
		CSoundMgr::Get_Instance()->BGM_Stop();
		pUIManager->Set_LevelMenuOn(false);
	}
	Safe_Release(pGameInstance);

	RELEASE_INSTANCE(CUI_Manager);
}

void CLevel_Logo::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}



CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();


}
