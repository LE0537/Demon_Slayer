#include "stdafx.h"
#include "Level_AdvRui.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"


CLevel_AdvRui::CLevel_AdvRui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_AdvRui::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	/*CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	pUI_Manager->Add_Adc_Menu();
	RELEASE_INSTANCE(CUI_Manager);*/

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLevel_AdvRui::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	
	Safe_Release(pGameInstance);

}

void CLevel_AdvRui::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_AdvRui::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;

	/* For.Point */
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_FIELDSHADOW;
	LightDesc.vDirection = _float4(-10.f, 150.f, -10.f, 1.f);
	LightDesc.vDiffuse = _float4(60.f, -20.f, 60.f, 1.f);
	LightDesc.vAmbient = _float4(0.f, 0.1f, 0.f, 0.f);

	_vector vLook = XMLoadFloat4(&LightDesc.vDiffuse) - XMLoadFloat4(&LightDesc.vDirection);

	if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	XMStoreFloat4(&LightDesc.vDirection, XMVector3Normalize(vLook));
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Player(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_AdvRui::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	return S_OK;
}

CLevel_AdvRui * CLevel_AdvRui::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_AdvRui*	pInstance = new CLevel_AdvRui(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_AdvRui"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_AdvRui::Free()
{
	__super::Free();


}
