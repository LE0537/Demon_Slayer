#include "stdafx.h"
#include "..\Public\Level_SelectChar.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "SelP1Cursor.h"
#include "SelP2Cursor.h"

CLevel_SelectChar::CLevel_SelectChar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_SelectChar::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_SelectChar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

	if (!m_bCreateUI)
	{
		pUIManager->Add_Select_CharUI();
		m_bCreateUI = true;
	}

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CSelP1Cursor* pSel_P1Cursor = dynamic_cast<CSelP1Cursor*>(pUIManager->Get_1PCursor());
	CSelP2Cursor* pSel_P2Cursor = dynamic_cast<CSelP2Cursor*>(pUIManager->Get_2PCursor());

	if (pSel_P1Cursor != nullptr && pSel_P2Cursor != nullptr)
	{
		if (pSel_P1Cursor->Get_SelComple() && pSel_P2Cursor->Get_SelComple())
		{
			_uint i1p = pUIManager->Get_1PChar()->Get_ImgNum();
			_uint i2p = pUIManager->Get_2PChar()->Get_ImgNum();
			_uint i1p_2 = pUIManager->Get_1P_2Char()->Get_ImgNum();
			_uint i2p_2 = pUIManager->Get_2P_2Char()->Get_ImgNum();

			pUIManager->Set_Sel1P(i1p);
			pUIManager->Set_Sel2P(i2p);

			if (!pUIManager->Get_1PCursor()->Get_SelectUIInfo().bOni)
				pUIManager->Set_Sel1P_2(i1p_2);
			else
				pUIManager->Set_Sel1P_2(99);

			if (!pUIManager->Get_2PCursor()->Get_SelectUIInfo().bOni)
				pUIManager->Set_Sel2P_2(i2p_2);
			else
				pUIManager->Set_Sel2P_2(99);

			if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
				return;
		}
	}
	
	Safe_Release(pGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CLevel_SelectChar::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}



CLevel_SelectChar * CLevel_SelectChar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_SelectChar*	pInstance = new CLevel_SelectChar(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_SelectChar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_SelectChar::Free()
{
	__super::Free();


}
