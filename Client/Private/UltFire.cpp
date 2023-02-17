#include "stdafx.h"
#include "UltFire.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUltFire::CUltFire(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CUltFire::CUltFire(const CUltFire & rhs)
	: CUI(rhs)
{
}

HRESULT CUltFire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUltFire::Initialize(void * pArg)
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

void CUltFire::Tick(_float fTimeDelta)
{
	m_fSpriteTime += fTimeDelta;

	if (m_fSpriteTime >= 0.1f)
	{
		++m_iFrame;
		m_fSpriteTime = 0.f;
	}
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (!m_ThrowUIinfo.bPlyCheck)
	{
		m_iPowerIndex = pUI_Manager->Get_1P()->Get_PlayerInfo().iPowerIndex;

		switch (m_iPowerIndex)
		{
		case 1:
			if (!pUI_Manager->Get_1P()->Get_PlayerInfo().bOni)
				m_iImgNum = 0;
			else
				m_iImgNum = 2;
			break;
		case 2:
			if (!pUI_Manager->Get_1P()->Get_PlayerInfo().bOni)
				m_iImgNum = 1;
			else
				m_iImgNum = 3;
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
		case 1:
			if (!pUI_Manager->Get_2P()->Get_PlayerInfo().bOni)
				m_iImgNum = 0;
			else
				m_iImgNum = 2;
			break;
		case 2:
			if (!pUI_Manager->Get_2P()->Get_PlayerInfo().bOni)
				m_iImgNum = 1;
			else
				m_iImgNum = 3;
			break;
		default:
			break;
		}
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CUltFire::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUltFire::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(14);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (pUI_Manager->Get_BattleTypeCheck())
	{
		if (!m_ThrowUIinfo.bPlyCheck)
		{
			if (pUI_Manager->Get_1P()->Get_PlayerInfo().iPowerIndex > 0)
				m_pVIBufferCom->Render();
		}
		else
		{
			if (pUI_Manager->Get_2P()->Get_PlayerInfo().iPowerIndex > 0)
				m_pVIBufferCom->Render();
		}
	}
	else if (!m_ThrowUIinfo.bPlyCheck)
	{
		if (pUI_Manager->Get_1P()->Get_PlayerInfo().iPowerIndex > 0)
			m_pVIBufferCom->Render();
	}

	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

HRESULT CUltFire::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_UltGaugeFire"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUltFire::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	m_iNumTextureU = 3;
	m_iNumTextureV = 2;

	if (FAILED(m_pShaderCom->Set_RawValue("g_iFrame", &m_iFrame, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iNumTexU", &m_iNumTextureU, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iNumTexV", &m_iNumTextureV, sizeof(_uint))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;

	return S_OK;
}

CUltFire * CUltFire::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUltFire*	pInstance = new CUltFire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUltFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUltFire::Clone(void * pArg)
{
	CUltFire*	pInstance = new CUltFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUltFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUltFire::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
