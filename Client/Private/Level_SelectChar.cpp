#include "stdafx.h"
#include "..\Public\Level_SelectChar.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "SelP1Cursor.h"
#include "SelP2Cursor.h"
#include "WindowLeft.h"
#include "WindowRight.h"
#include "SoundMgr.h"
#include "Level_SelectMap.h"
#include "Level_Menu.h"

CLevel_SelectChar::CLevel_SelectChar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_SelectChar::Initialize()
{
	g_iLevel = LEVEL_SELECTCHAR;
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Add_Select_CharUI();

	RELEASE_INSTANCE(CUI_Manager);

	CSoundMgr::Get_Instance()->BGM_Stop();
	CSoundMgr::Get_Instance()->PlayBGM(TEXT("CharSel.wav"), g_fBGM);
	
	return S_OK;
}

void CLevel_SelectChar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CSelP1Cursor* pSel_P1Cursor = dynamic_cast<CSelP1Cursor*>(pUIManager->Get_1PCursor());
	CSelP2Cursor* pSel_P2Cursor = dynamic_cast<CSelP2Cursor*>(pUIManager->Get_2PCursor());
	CWindowLeft* pWindowLeft = dynamic_cast<CWindowLeft*>(pUIManager->Get_Window(0));
	CWindowRight* pWindowRight = dynamic_cast<CWindowRight*>(pUIManager->Get_Window(1));

	if (pWindowLeft->Get_CloseCheck() && pWindowRight->Get_CloseCheck())
	{
		m_fDelayTime += fTimeDelta;
		if (m_fDelayTime >= 3.f)
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

			if (FAILED(pGameInstance->Open_Level(LEVEL_SELECTMAP, CLevel_SelectMap::Create(m_pDevice, m_pContext))))
				return;
			CSoundMgr::Get_Instance()->BGM_Stop();
			m_fDelayTime = 0.f;
		}
	}

	if (pGameInstance->Key_Down(DIK_Q) && !pSel_P1Cursor->Get_FirstSelCheck())
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_MENU, CLevel_Menu::Create(m_pDevice, m_pContext))))
			return;
	}
	
	Safe_Release(pGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CLevel_SelectChar::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


}
HRESULT CLevel_SelectChar::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = 500.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_SELECTCHAR, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
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
