#include "stdafx.h"
#include "UltStockEff.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SoundMgr.h"

CUltStockEff::CUltStockEff(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CUltStockEff::CUltStockEff(const CUltStockEff & rhs)
	: CUI(rhs)
{
}

HRESULT CUltStockEff::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUltStockEff::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	if (m_ThrowUIinfo.iTextureNum == 24)
		m_iImgNum = 0;
	else if (m_ThrowUIinfo.iTextureNum == 25)
		m_iImgNum = 1;
	else if (m_ThrowUIinfo.iTextureNum == 26)
		m_iImgNum = 2;
	else if (m_ThrowUIinfo.iTextureNum == 27)
		m_iImgNum = 3;
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


	return S_OK;
}

void CUltStockEff::Tick(_float fTimeDelta)
{
	if (m_iImgNum != 3)
	{
		if (m_fFadeTime >= 0.5f)
			m_bFadeCheck = true;
		else if (m_fFadeTime <= 0.f)
		{
			m_bFadeCheck = false;
			m_bZoomCheck = true;
			m_bSqureEffCheck = true;
		}

		if (m_bZoomCheck)
		{
			m_fSizeX = m_ThrowUIinfo.vScale.x;
			m_fSizeY = m_ThrowUIinfo.vScale.y;
			m_bZoomCheck = false;
		}

		if (!m_bFadeCheck)
		{
			if (m_iImgNum != 0)
			{
				m_fSizeX += m_ThrowUIinfo.vScale.x * 0.008f;
				m_fSizeY += m_ThrowUIinfo.vScale.y * 0.008f;
			}

			m_fFadeTime += fTimeDelta * 0.5f;
		}
		else
			m_fFadeTime -= fTimeDelta;

		if (m_bSqureEffCheck && m_iImgNum == 0)
		{
			m_fSizeX += m_ThrowUIinfo.vScale.x * 0.012f;
			m_fSizeY += m_ThrowUIinfo.vScale.y * 0.012f;
		}
	}
	else
	{
		CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

		if (m_bRenderCheck)
		{
			if (m_fFadeTime >= 0.5f)
				m_bFadeCheck = true;
			else if (m_fFadeTime <= 0.f)
			{
				m_fSizeX = m_ThrowUIinfo.vScale.x;
				m_fSizeY = m_ThrowUIinfo.vScale.y;
				m_bFadeCheck = false;
				m_bRenderCheck = false;
			}

			if (!m_bFadeCheck)
				m_fFadeTime += fTimeDelta * 0.5f;
			else
				m_fFadeTime -= fTimeDelta;

			m_fSizeX += m_ThrowUIinfo.vScale.x * 0.02f;
			m_fSizeY += m_ThrowUIinfo.vScale.y * 0.02f;
		}

		if (!m_ThrowUIinfo.bPlyCheck)
		{
			if (m_iUnicCount < pUI_Manager->Get_1P()->Get_PlayerInfo().iUnicCount)
			{
				m_bRenderCheck = true;
				CSoundMgr::Get_Instance()->PlayEffect(TEXT("UltNumUp.wav"), fEFFECT);
			}
			m_iUnicCount = pUI_Manager->Get_1P()->Get_PlayerInfo().iUnicCount;
		}
		else
		{
			if (m_iUnicCount < pUI_Manager->Get_2P()->Get_PlayerInfo().iUnicCount)
			{
				CSoundMgr::Get_Instance()->PlayEffect(TEXT("UltNumUp.wav"), fEFFECT);
				m_bRenderCheck = true;
			}
			m_iUnicCount = pUI_Manager->Get_2P()->Get_PlayerInfo().iUnicCount;
		}

		RELEASE_INSTANCE(CUI_Manager);
	}
	


	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
}

void CUltStockEff::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUltStockEff::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if(!m_ThrowUIinfo.pTarget->Get_PlayerInfo().bOni)
		m_pShaderCom->Begin(26);
	else 
		m_pShaderCom->Begin(27);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (pUI_Manager->Get_BattleTypeCheck())
	{
		if (!m_ThrowUIinfo.bPlyCheck)
		{
			if (pUI_Manager->Get_1P()->Get_PlayerInfo().iPowerIndex == 0 && pUI_Manager->Get_1P()->Get_PlayerInfo().iUnicCount > 0)
				m_pVIBufferCom->Render();
		}
		else
		{
			if (pUI_Manager->Get_2P()->Get_PlayerInfo().iPowerIndex == 0 && pUI_Manager->Get_2P()->Get_PlayerInfo().iUnicCount > 0)
				m_pVIBufferCom->Render();
		}
	}
	else
	{
		if (!m_ThrowUIinfo.bPlyCheck)
		{
			if (pUI_Manager->Get_1P()->Get_PlayerInfo().iPowerIndex > 0)
				m_pVIBufferCom->Render();
		}
	}
	
	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

HRESULT CUltStockEff::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockEff"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUltStockEff::SetUp_ShaderResources()
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

CUltStockEff * CUltStockEff::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUltStockEff*	pInstance = new CUltStockEff(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUltStockEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUltStockEff::Clone(void * pArg)
{
	CUltStockEff*	pInstance = new CUltStockEff(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUltStockEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUltStockEff::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
