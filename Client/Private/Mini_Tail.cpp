#include "stdafx.h"
#include "..\Public\Mini_Tail.h"

#include "GameInstance.h"
#include "UI_Manager.h"

CMini_Tail::CMini_Tail(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMini_Tail::CMini_Tail(const CMini_Tail & rhs)
	: CGameObj(rhs)
{
}

HRESULT CMini_Tail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMini_Tail::Initialize(void * pArg)
{
	*(CMini_Tail**)pArg = this;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;
	m_fX = 240.f;
	m_fY = 260.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	_int iDest = rand() % 3;

	switch (iDest)
	{
	case 0:
		m_iTextrueNum = 2;
		break;
	case 1:
		m_iTextrueNum = 3;
		break;
	case 2:
		m_iTextrueNum = 4;
		break;
	default:
		break;
	}
	return S_OK;
}

void CMini_Tail::Tick(_float fTimeDelta)
{
	if (g_bMiniGame)
		return;
}

void CMini_Tail::Late_Tick(_float fTimeDelta)
{
	if (g_bMiniGame)
		Set_Dead();
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CMini_Tail::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	return S_OK;
}



HRESULT CMini_Tail::Ready_Components()
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

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MiniGame"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMini_Tail::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iTextrueNum))))
		return E_FAIL;

	m_pShaderCom->Begin();
	m_pVIBufferCom->Render();

	return S_OK;
}

CMini_Tail * CMini_Tail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMini_Tail*	pInstance = new CMini_Tail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMini_Tail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMini_Tail::Clone(void * pArg)
{
	CMini_Tail*	pInstance = new CMini_Tail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMini_Tail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMini_Tail::Free()
{
	__super::Free();


}
