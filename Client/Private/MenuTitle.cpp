#include "stdafx.h"
#include "MenuTitle.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CMenuTitle::CMenuTitle(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CMenuTitle::CMenuTitle(const CMenuTitle & rhs)
	: CUI(rhs)
{
}

HRESULT CMenuTitle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMenuTitle::Initialize(void * pArg)
{
	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360)
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, m_ThrowUIinfo.vRot));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));
	

	return S_OK;
}

void CMenuTitle::Tick(_float fTimeDelta)
{
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	
	_float fMenuCursorX = pUIManager->Get_MenuCursor()->Get_fX();
	_float fMenuCursorY = pUIManager->Get_MenuCursor()->Get_fY();
	
	if (m_ThrowUIinfo.iTextureNum == 11)
	{
		if (fMenuCursorX == 55.f && fMenuCursorY == 167.f)
		{
			m_iImgNum = 1;
			m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY * 1.2f, 0.f, 1.f));
		}
		else
		{
			m_iImgNum = 0;
			m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));
		}
	}
	else if (m_ThrowUIinfo.iTextureNum == 16)
	{
		if (fMenuCursorX == 75.f && fMenuCursorY == 252.f)
			m_iImgNum = 1;
		else
			m_iImgNum = 0;

		
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CMenuTitle::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CMenuTitle::Render()
{
	if (m_ThrowUIinfo.iTextureNum == 11)
	{
		if (nullptr == m_pShaderCom ||
			nullptr == m_pVIBufferCom)
			return E_FAIL;

		if (FAILED(SetUp_ShaderResources()))
			return E_FAIL;

		if (!m_ThrowUIinfo.bReversal)
			m_pShaderCom->Begin();
		else
			m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}
	else if (m_ThrowUIinfo.iTextureNum == 16)
	{
		if (nullptr == m_pShaderCom ||
			nullptr == m_pVIBufferCom1)
			return E_FAIL;

		if (FAILED(SetUp_ShaderResources()))
			return E_FAIL;

		if (!m_ThrowUIinfo.bReversal)
			m_pShaderCom->Begin();
		else
			m_pShaderCom->Begin(1);

		m_pVIBufferCom1->Render();
	}



	return S_OK;
}

HRESULT CMenuTitle::Ready_Components()
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
	if (m_ThrowUIinfo.iTextureNum == 11)
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ModeTitle_Adv"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
	}
	else if (m_ThrowUIinfo.iTextureNum == 16)
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ModeTitle_Vs"), (CComponent**)&m_pTextureCom1)))
			return E_FAIL;

		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer1"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom1)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMenuTitle::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (m_ThrowUIinfo.iTextureNum == 11)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
			return E_FAIL;
	}
	else if (m_ThrowUIinfo.iTextureNum == 16)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_iImgNum))))
			return E_FAIL;
	}
	

	return S_OK;
}

CMenuTitle * CMenuTitle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMenuTitle*	pInstance = new CMenuTitle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMenuTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMenuTitle::Clone(void * pArg)
{
	CMenuTitle*	pInstance = new CMenuTitle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMenuTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMenuTitle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	if (m_ThrowUIinfo.iTextureNum == 11)
	{
		Safe_Release(m_pTextureCom);
		Safe_Release(m_pVIBufferCom);
	}
	else if (m_ThrowUIinfo.iTextureNum == 16)
	{
		Safe_Release(m_pTextureCom1);
		Safe_Release(m_pVIBufferCom1);
	}
	
	Safe_Release(m_pRendererCom);
}
