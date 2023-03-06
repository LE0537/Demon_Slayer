#include "stdafx.h"
#include "QuiestKeyUI.h"
#include "GameInstance.h"
#include "MsgTextBase.h"
#include "UI_Manager.h"
#include "Door.h"

CQuiestKeyUI::CQuiestKeyUI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CQuiestKeyUI::CQuiestKeyUI(const CQuiestKeyUI & rhs)
	: CUI(rhs)
{
}

HRESULT CQuiestKeyUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQuiestKeyUI::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (g_iLevel == LEVEL_ADVRUI)
	{
		memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));
		m_iImgNum = m_ThrowUIinfo.iLayerNum;
		m_fSizeX = m_ThrowUIinfo.vScale.x;
		m_fSizeY = m_ThrowUIinfo.vScale.y;
		m_fX = m_ThrowUIinfo.vPos.x;
		m_fY = m_ThrowUIinfo.vPos.y;
	}
	else
	{
		memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));
		memcpy(&m_iLayerNum, pArg, sizeof(_uint));
		if (m_ThrowUIinfo.iLayerNum == 1)
		{
			m_iImgNum = 1;
			m_fSizeX = m_ThrowUIinfo.vScale.x;
			m_fSizeY = m_ThrowUIinfo.vScale.y;
			m_fX = m_ThrowUIinfo.vPos.x;
			m_fY = m_ThrowUIinfo.vPos.y;
		}
		else if(m_iLayerNum >= 0 && m_iLayerNum <= 50)
		{
			m_iImgNum = 2;
			m_fSizeX = 50.f;
			m_fSizeY = 50.f;
			m_fX = 830.f;
			m_fY = 360.f;
		}
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	
	return S_OK;
}

void CQuiestKeyUI::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);


	if (g_iLevel == LEVEL_BATTLEENMU)
		m_bDead = true;

	if (g_iLevel == LEVEL_ADVRUI)
	{
		if (m_ThrowUIinfo.iLayerNum == 0 || m_ThrowUIinfo.iLayerNum == 1)
		{
			if (!pUI_Manager->Get_MsgOnOff())
			{
				m_fFadeTime += 0.2f;
				if (m_fFadeTime >= 1.f)
					m_fFadeTime = 1.f;
			}
			else
			{
				m_fFadeTime -= 0.2f;
				if (m_fFadeTime <= 0.f)
					m_fFadeTime = 0.f;
			}
		}
		else
		{
			if (pUI_Manager->Get_InteractionOnOff())
			{
				m_fFadeTime += 0.2f;
				if (m_fFadeTime >= 1.f)
					m_fFadeTime = 1.f;
			}
			else
			{
				m_fFadeTime -= 0.2f;
				if (m_fFadeTime <= 0.f)
					m_fFadeTime = 0.f;
			}
		}
	}
	else if(g_iLevel == LEVEL_ADVAKAZA)
	{
		if (m_iLayerNum >= 0 && m_iLayerNum <= 50)
		{
			if (pUI_Manager->Get_InteractionOnOff())
			{
				m_fFadeTime += 0.2f;
				if (m_fFadeTime >= 1.f)
				{
					m_fFadeTime = 1.f;
					m_bFadeInCheck = true;
				}
			}

			if (m_bFadeInCheck && !pUI_Manager->Get_InteractionOnOff())
			{
				m_fFadeTime -= 0.2f;
				if (m_fFadeTime <= 0.f)
				{
					m_fFadeTime = 0.f;
					m_bDead = true;
				}
			}
		}
		else if (m_ThrowUIinfo.iLayerNum == 1)
		{
			if (!pUI_Manager->Get_MsgOnOff())
			{
				m_fFadeTime += 0.2f;
				if (m_fFadeTime >= 1.f)
					m_fFadeTime = 1.f;
			}
			else
			{
				m_fFadeTime -= 0.2f;
				if (m_fFadeTime <= 0.f)
					m_fFadeTime = 0.f;
			}
		}	
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CQuiestKeyUI::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);
}

HRESULT CQuiestKeyUI::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	
	if(g_iLevel == LEVEL_ADVRUI)
		m_pVIBufferCom->Render();
	else if (g_iLevel == LEVEL_ADVAKAZA)
	{
		if(m_ThrowUIinfo.iLayerNum == 1)
			m_pVIBufferCom->Render();

		for (auto iter : pUI_Manager->Get_vecDoorInfo())
		{
			if (iter.iModelIndex == m_iLayerNum && dynamic_cast<CDoor*>(iter.pDoor)->Get_ColCheck())
				m_pVIBufferCom->Render();
		}
	}

	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

HRESULT CQuiestKeyUI::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestKeyUI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuiestKeyUI::SetUp_ShaderResources()
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

CQuiestKeyUI * CQuiestKeyUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CQuiestKeyUI*	pInstance = new CQuiestKeyUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CQuiestKeyUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuiestKeyUI::Clone(void * pArg)
{
	CQuiestKeyUI*	pInstance = new CQuiestKeyUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CQuiestKeyUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuiestKeyUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
