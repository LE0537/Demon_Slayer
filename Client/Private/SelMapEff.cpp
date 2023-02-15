#include "stdafx.h"
#include "SelMapEff.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SelMapCursor.h"

CSelMapEff::CSelMapEff(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CSelMapEff::CSelMapEff(const CSelMapEff & rhs)
	: CUI(rhs)
{
}

HRESULT CSelMapEff::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSelMapEff::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360)
		m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_ThrowUIinfo.vRot));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	if (m_ThrowUIinfo.iTextureNum == 22)
		m_iImgNum = 0;
	else if (m_ThrowUIinfo.iTextureNum == 23)
		m_iImgNum = 1;

	return S_OK;
}

void CSelMapEff::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	m_bMapSelectCheck = dynamic_cast<CSelMapCursor*>(pUI_Manager->Get_SelMapCursor())->Get_MapSelectCheck();
	m_iSelNum = dynamic_cast<CSelMapCursor*>(pUI_Manager->Get_SelMapCursor())->Get_FrameNum();

	if (m_bMapSelectCheck)
	{
		if (m_ThrowUIinfo.iTextureNum == 22)
		{
			if (m_fFadeTime <= 0.f && !m_bEffEnd)
				m_bFadeCheck = false;
			else if (m_fFadeTime >= 1.f)
				m_bFadeCheck = true;

			if(!m_bFadeCheck)
				m_fFadeTime += 0.1f;
			else if (m_bFadeCheck)
			{
				m_fFadeTime -= 0.1f;

				if (m_fFadeTime <= 0.f)
					m_bEffEnd = true;
			}
		}
		else
		{
			m_fSizeX += 5.f;
			m_fSizeY += 5.f;

			if (m_fFadeTime <= 0.f && !m_bEffEnd)
				m_bFadeCheck = false;
			else if (m_fFadeTime >= 0.5f)
				m_bFadeCheck = true;

			if (!m_bFadeCheck)
				m_fFadeTime += 0.05f;
			else if (m_bFadeCheck)
			{
				m_fFadeTime -= 0.05f;

				if (m_fFadeTime <= 0.f)
				{
					m_bEffEnd = true;
					dynamic_cast<CSelMapCursor*>(pUI_Manager->Get_SelMapCursor())->Set_MapSelectEnd(true);
				}
			}
		}
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CSelMapEff::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CSelMapEff::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	if (m_bMapSelectCheck)
	{
		if(m_iSelNum == m_ThrowUIinfo.iLayerNum)
			m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CSelMapEff::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelMapEff"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSelMapEff::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fFadeTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;

	return S_OK;
}

CSelMapEff * CSelMapEff::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSelMapEff*	pInstance = new CSelMapEff(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSelMapEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CSelMapEff::Clone(void * pArg)
{
	CSelMapEff*	pInstance = new CSelMapEff(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSelMapEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSelMapEff::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
