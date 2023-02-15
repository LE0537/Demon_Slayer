#include "stdafx.h"
#include "Level_SelectMap.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include"SoundMgr.h"
#include "SelMapCursor.h"

CLevel_SelectMap::CLevel_SelectMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_SelectMap::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Add_Select_Map();

	RELEASE_INSTANCE(CUI_Manager);

	CSoundMgr::Get_Instance()->PlayBGM(TEXT("CharSel.wav"), fBGM);
	
	return S_OK;
}

void CLevel_SelectMap::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_bool bCheck = dynamic_cast<CSelMapCursor*>(pUIManager->Get_SelMapCursor())->Get_MapSelectEnd();

	if (bCheck)
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}

	if (pGameInstance->Key_Down(DIK_Q))
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_SELECTCHAR))))
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
