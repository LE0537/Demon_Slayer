#include "stdafx.h"
#include "InkEff.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CInkEff::CInkEff(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CInkEff::CInkEff(const CInkEff & rhs)
	: CUI(rhs)
{
}

HRESULT CInkEff::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInkEff::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 1280.f;
	m_fSizeY = 720.f;
	m_fX = 640.f;
	m_fY = 360.f;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
	return S_OK;
}

void CInkEff::Tick(_float fTimeDelta)
{
	CUI_Manager*		pUI_Manager = GET_INSTANCE(CUI_Manager);

	m_fSpriteTime += fTimeDelta;

	if (m_fSpriteTime >= 0.05f)
	{
		++m_iFrame;
		m_fSpriteTime = 0.f;
	}

	if (m_iFrame == 47 && m_iImgNum == 0 && !m_bDownCheck)
	{
		pUI_Manager->Add_Menu();
		m_iFrame = 0;
		m_bDownCheck = true;
	}

	if (m_bDownCheck && m_iFrame == 47)
		m_bDead = true;
	

	RELEASE_INSTANCE(CUI_Manager);
}

void CInkEff::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);
}

HRESULT CInkEff::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(13);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CInkEff::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_InkEff"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInkEff::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (m_iImgNum == 0)
	{
		m_iNumTextureU = 6;
		m_iNumTextureV = 8;
	}
	if (m_iImgNum == 1)
	{

	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_iFrame", &m_iFrame, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iNumTexU", &m_iNumTextureU, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iNumTexV", &m_iNumTextureV, sizeof(_uint))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_bInkEffDownCheck", &m_bDownCheck, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;

	return S_OK;
}

CInkEff * CInkEff::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CInkEff*	pInstance = new CInkEff(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CInkEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CInkEff::Clone(void * pArg)
{
	CInkEff*	pInstance = new CInkEff(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CInkEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInkEff::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
