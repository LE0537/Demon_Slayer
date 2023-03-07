#include "stdafx.h"
#include "MGameHeart.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CMGameHeart::CMGameHeart(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CMGameHeart::CMGameHeart(const CMGameHeart & rhs)
	: CUI(rhs)
{
}

HRESULT CMGameHeart::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMGameHeart::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_iLayerNum, pArg, sizeof(_uint));

	m_fSizeX = 50.f;
	m_fSizeY = 50.f;
	m_fY = 130.f;
	if (m_iLayerNum == 0)
		m_fX = 80.f;
	else if(m_iLayerNum == 1)
		m_fX = 130.f;
	else if (m_iLayerNum == 2)
		m_fX = 180.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	pUI_Manager->Set_HeartUI(this, m_iLayerNum);
	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

void CMGameHeart::Tick(_float fTimeDelta)
{

}

void CMGameHeart::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom && m_bRender)
	{
		CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
		if(m_iLayerNum <= pUI_Manager->Get_1P()->Get_Heart())
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
		RELEASE_INSTANCE(CUI_Manager);
	}
}

HRESULT CMGameHeart::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin();
	
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMGameHeart::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MGameHeart"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMGameHeart::SetUp_ShaderResources()
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

CMGameHeart * CMGameHeart::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMGameHeart*	pInstance = new CMGameHeart(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMGameHeart"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMGameHeart::Clone(void * pArg)
{
	CMGameHeart*	pInstance = new CMGameHeart(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMGameHeart"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMGameHeart::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
