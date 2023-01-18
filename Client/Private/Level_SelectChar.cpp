#include "stdafx.h"
#include "..\Public\Level_SelectChar.h"

#include "GameInstance.h"
#include "Level_Loading.h"

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

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (pGameInstance->Key_Down(DIK_T))
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}
	Safe_Release(pGameInstance);



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
