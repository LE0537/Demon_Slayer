#include "stdafx.h"
#include "Level_StroyMenu.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"


CLevel_StoryMenu::CLevel_StoryMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_StoryMenu::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	pUI_Manager->Add_Adc_Menu();
	RELEASE_INSTANCE(CUI_Manager);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_StoryMenu::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	_uint iStageNum = pUI_Manager->Get_AdvStageNum();

	//0¹ø ·çÀÌ¸Ê
	//1¹ø ¾ÆÄ«ÀÚ¸Ê
	if (pUI_Manager->Get_AdvMenuSelCheck())
	{
		switch (iStageNum)
		{
		case 0:
			if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_ADVRUI))))
				return;
			break;
		case 1:
			break;
		default:
			break;
		}
		pUI_Manager->Set_AdvMenuSelCheck(false);
	}
	Safe_Release(pGameInstance);

	RELEASE_INSTANCE(CUI_Manager);
}

void CLevel_StoryMenu::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

CLevel_StoryMenu * CLevel_StoryMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_StoryMenu*	pInstance = new CLevel_StoryMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_StoryMenu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_StoryMenu::Free()
{
	__super::Free();


}
