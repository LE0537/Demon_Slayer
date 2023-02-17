#include "stdafx.h"
#include "UltStockFrame.h"
#include "GameInstance.h"
#include "UI_Manager.h"
CUltStockFrame::CUltStockFrame(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CUltStockFrame::CUltStockFrame(const CUltStockFrame & rhs)
	: CUI(rhs)
{
}

HRESULT CUltStockFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUltStockFrame::Initialize(void * pArg)
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

void CUltStockFrame::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (!m_ThrowUIinfo.bPlyCheck)
	{
		m_iPowerIndex = pUI_Manager->Get_1P()->Get_PlayerInfo().iPowerIndex;
		
		switch (m_iPowerIndex)
		{
		case 0:
			if(!pUI_Manager->Get_1P()->Get_PlayerInfo().bOni)
				m_iImgNum = 0;
			else
				m_iImgNum = 1;
			break;
		case 1:
			if (!pUI_Manager->Get_1P()->Get_PlayerInfo().bOni)
				m_iImgNum = 2;
			else
				m_iImgNum = 4;
			break;
		case 2:
			if (!pUI_Manager->Get_1P()->Get_PlayerInfo().bOni)
				m_iImgNum = 3;
			else
				m_iImgNum = 5;
			break;
		default:
			break;
		}
	}
	else if (m_ThrowUIinfo.bPlyCheck)
	{
		m_iPowerIndex = pUI_Manager->Get_2P()->Get_PlayerInfo().iPowerIndex;

		switch (m_iPowerIndex)
		{
		case 0:
			if (!pUI_Manager->Get_2P()->Get_PlayerInfo().bOni)
				m_iImgNum = 0;
			else
				m_iImgNum = 1;
			break;
		case 1:
			if (!pUI_Manager->Get_2P()->Get_PlayerInfo().bOni)
				m_iImgNum = 2;
			else
				m_iImgNum = 4;
			break;
		case 2:
			if (!pUI_Manager->Get_2P()->Get_PlayerInfo().bOni)
				m_iImgNum = 3;
			else
				m_iImgNum = 5;
			break;
		default:
			break;
		}
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CUltStockFrame::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUltStockFrame::Render()
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

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (pUI_Manager->Get_BattleTypeCheck())
		m_pVIBufferCom->Render();
	else if (!m_ThrowUIinfo.bPlyCheck)
		m_pVIBufferCom->Render();

	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

HRESULT CUltStockFrame::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltStockFrame"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUltStockFrame::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;

	return S_OK;
}

CUltStockFrame * CUltStockFrame::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUltStockFrame*	pInstance = new CUltStockFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUltStockFrame"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUltStockFrame::Clone(void * pArg)
{
	CUltStockFrame*	pInstance = new CUltStockFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUltStockFrame"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUltStockFrame::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
