#include "stdafx.h"
#include "HpBarDeco.h"
#include "GameInstance.h"

CHpBarDeco::CHpBarDeco(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CHpBarDeco::CHpBarDeco(const CHpBarDeco & rhs)
	: CGameObj(rhs)
{
}

HRESULT CHpBarDeco::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHpBarDeco::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_iImgNum, pArg, sizeof(_uint));

	m_fSizeX = 612.f;
	m_fSizeY = 24.f;
	m_fY = 88.f;

	if (0 == m_iImgNum)
		m_fX = 376.f;
	else
		m_fX = 914.f;
	
	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (m_iImgNum == 0)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX * 0.65f, m_fSizeY, 0.f, 1.f));
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);
		m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX * 0.62f, m_fSizeY, 0.f, 1.f));
	}

	
		

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f)));
	

	return S_OK;
}

void CHpBarDeco::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
}

void CHpBarDeco::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CHpBarDeco::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iImgNum);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CHpBarDeco::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HpBarDeco"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHpBarDeco::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CHpBarDeco * CHpBarDeco::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHpBarDeco*	pInstance = new CHpBarDeco(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHpBarDeco"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CHpBarDeco::Clone(void * pArg)
{
	CHpBarDeco*	pInstance = new CHpBarDeco(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHpBarDeco"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHpBarDeco::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
