#include "stdafx.h"
#include "LogoBackEff.h"
#include "GameInstance.h"

CLogoBackEff::CLogoBackEff(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CLogoBackEff::CLogoBackEff(const CLogoBackEff & rhs)
	: CUI(rhs)
{
}

HRESULT CLogoBackEff::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogoBackEff::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = 1280.f;
	m_fSizeY = 480.f;
	m_fX = 640.f;
	m_fY = 480.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360)
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, m_ThrowUIinfo.vRot));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));


	return S_OK;
}

void CLogoBackEff::Tick(_float fTimeDelta)
{
	if (m_ThrowUIinfo.iLayerNum != 0)
		m_bDead = true;
	if (m_fUvMoveTime < 1.f)
		m_fUvMoveTime += fTimeDelta * 0.07f;
	else
		m_fUvMoveTime = 0.f;

	if (!m_bAlphaValueCheck)
		m_fAlphaTime += fTimeDelta * 0.1f;

	else if (m_bAlphaValueCheck)
		m_fAlphaTime -= fTimeDelta * 0.1f;

	if (m_fAlphaTime > 0.5f)
		m_bAlphaValueCheck = true;
	else if (m_fAlphaTime < 1.f)
		m_bAlphaValueCheck = false;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
}

void CLogoBackEff::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CLogoBackEff::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(7);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CLogoBackEff::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LogoBackEff"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogoBackEff::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_fUvMoveTime", &m_fUvMoveTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlphaTime", &m_fAlphaTime, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CLogoBackEff * CLogoBackEff::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLogoBackEff*	pInstance = new CLogoBackEff(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CLogoBackEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CLogoBackEff::Clone(void * pArg)
{
	CLogoBackEff*	pInstance = new CLogoBackEff(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CLogoBackEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogoBackEff::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
