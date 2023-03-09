#include "stdafx.h"
#include "..\Public\Mini_Result.h"

#include "GameInstance.h"
#include "UI_Manager.h"
#include "Level_Loading.h"

CMini_Result::CMini_Result(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMini_Result::CMini_Result(const CMini_Result & rhs)
	: CGameObj(rhs)
{
}

HRESULT CMini_Result::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMini_Result::Initialize(void * pArg)
{
	*(CMini_Result**)pArg = this;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 1400.f;
	m_fSizeY = 15.f;
	m_fX = 640.f;
	m_fY = 715.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	m_fSizeX = 1280.f;
	m_fSizeY = 7.f;
	m_fX = 640.f;
	m_fY = 715.f;

	m_pTransformCom2->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 1.f));
	m_pTransformCom2->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	return S_OK;
}

void CMini_Result::Tick(_float fTimeDelta)
{
}

void CMini_Result::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);
}

HRESULT CMini_Result::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	return S_OK;
}



HRESULT CMini_Result::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 100.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform2"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom2, &TransformDesc)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Shader2"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"), (CComponent**)&m_pShaderCom2)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MiniGame"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer2"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom2)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMini_Result::SetUp_ShaderResources()
{
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(9))))
		return E_FAIL;

	m_pShaderCom->Begin();
	m_pVIBufferCom->Render();
	_float fMax = 100.f;
	if (FAILED(m_pShaderCom2->Set_RawValue("g_WorldMatrix", &m_pTransformCom2->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom2->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom2->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxBar", &fMax, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurBar", &g_fLoading, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom2->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(10))))
		return E_FAIL;

	m_pShaderCom2->Begin(3);
	m_pVIBufferCom2->Render();

	wstring szName = TEXT("");
	_vector vPos, vScale;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (!m_bStart)
	{
		szName = TEXT("Game Start");
		vPos = { 390.f,260.f,0.f,1.f };
		vScale = { 2.5f,2.5f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);

		szName = TEXT("Press  ' F '  Key");
		vPos = { 500.f,400.f,0.f,1.f };
		vScale = { 1.f,1.f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);
	}
	else if (m_bPlayerDead)
	{
		szName = TEXT("Game Over");
		vPos = { 390.f,260.f,0.f,1.f };
		vScale = { 2.5f,2.5f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);

		szName = TEXT("ReStart  Press  ' F '  Key");
		vPos = { 430.f,400.f,0.f,1.f };
		vScale = { 1.f,1.f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);
	}
	if (m_bStart)
	{
		szName = TEXT("Score :");
		vPos = { 1050.f,10.f,0.f,1.f };
		vScale = { 0.7f,0.7f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);

		szName = to_wstring(m_pTarget->Get_Score());
		vPos = { 1170.f,12.f,0.f,1.f };
		vScale = { 0.7f,0.7f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);
	
		szName = TEXT("Rank Score :");
		vPos = { 20.f,10.f,0.f,1.f };
		vScale = { 0.7f,0.7f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);

		szName = to_wstring(m_iRankScore);
		vPos = { 180.f,12.f,0.f,1.f };
		vScale = { 0.7f,0.7f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);

	}
	if (g_fLoading >= 100.f)
	{
		szName = TEXT("Start DemonSlayer  Press  ' Enter '  Key");
		vPos = { 480.f,680.f,0.f,1.f };
		vScale = { 0.5f,0.5f,1.f,1.f };

		pGameInstance->Render_Font(TEXT("Font_Nexon"), szName.c_str(), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), vScale);

	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMini_Result * CMini_Result::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMini_Result*	pInstance = new CMini_Result(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMini_Result"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMini_Result::Clone(void * pArg)
{
	CMini_Result*	pInstance = new CMini_Result(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMini_Result"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMini_Result::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom2);
	Safe_Release(m_pShaderCom2);
	Safe_Release(m_pVIBufferCom2);

}
