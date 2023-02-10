#include "stdafx.h"
#include "Level_GameResult.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"


CLevel_GameResult::CLevel_GameResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GameResult::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Add_BattleResult();
	RELEASE_INSTANCE(CUI_Manager);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	/*if (FAILED(Ready_Lights()))
		return E_FAIL;
*/
	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		//return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	/*if (FAILED(Load_Map(L"Layer_BackGround", "11_Map_Rui")))
		return E_FAIL;*/

	return S_OK;
}

void CLevel_GameResult::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

	if (pGameInstance->Key_Down(DIK_T))
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_MENU))))
			return;
	}

	Safe_Release(pGameInstance);

	RELEASE_INSTANCE(CUI_Manager);
}

void CLevel_GameResult::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}

HRESULT CLevel_GameResult::Ready_Lights()
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

HRESULT CLevel_GameResult::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	
	_uint iFirstChar = pUIManager->Get_Result_Info().iFirstTarget;//첫번째캐릭터 번호
	_uint iSecondChar = pUIManager->Get_Result_Info().iSecondTarget; //서브캐릭터 번호


	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

HRESULT CLevel_GameResult::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(25.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = 500.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_GAMEPLAY, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GameResult::Load_Map(const _tchar * pLayerTag, char * pFileName)
{
	return E_NOTIMPL;
}



CLevel_GameResult * CLevel_GameResult::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GameResult*	pInstance = new CLevel_GameResult(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_GameResult"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GameResult::Free()
{
	__super::Free();


}
