#include "stdafx.h"
#include "Level_SelectMap.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include"SoundMgr.h"
#include "SelMapCursor.h"
#include "WindowLeft.h"
#include "WindowRight.h"
#include "Level_GamePlay.h"
#include "Level_SelectChar.h"

CLevel_SelectMap::CLevel_SelectMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_SelectMap::Initialize()
{
	g_iLevel == LEVEL_SELECTMAP;
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Add_Select_Map();

	RELEASE_INSTANCE(CUI_Manager);
	
	return S_OK;
}

void CLevel_SelectMap::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CWindowLeft* pWindowLeft = dynamic_cast<CWindowLeft*>(pUIManager->Get_Window(0));
	CWindowRight* pWindowRight = dynamic_cast<CWindowRight*>(pUIManager->Get_Window(1));


	if (pWindowLeft->Get_CloseCheck() && pWindowRight->Get_CloseCheck())
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_GAMEPLAY, CLevel_GamePlay::Create(m_pDevice, m_pContext))))
			return;
	}
	

	if (pGameInstance->Key_Down(DIK_Q))
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_SELECTCHAR, CLevel_SelectChar::Create(m_pDevice, m_pContext))))
			return;
	}
	
	Safe_Release(pGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CLevel_SelectMap::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

CLevel_SelectMap * CLevel_SelectMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_SelectMap*	pInstance = new CLevel_SelectMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_SelectMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_SelectMap::Free()
{
	__super::Free();


}
