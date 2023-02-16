#include "stdafx.h"
#include "ResultCloud.h"
#include "GameInstance.h"

CResultCloud::CResultCloud(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CResultCloud::CResultCloud(const CResultCloud & rhs)
	: CUI(rhs)
{
}

HRESULT CResultCloud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CResultCloud::Initialize(void * pArg)
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

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
	{
		if (m_ThrowUIinfo.iTextureNum == 2)
			m_iImgNum = 0;
		else if (m_ThrowUIinfo.iTextureNum == 3)
			m_iImgNum = 1;
		else if (m_ThrowUIinfo.iTextureNum == 4)
			m_iImgNum = 2;
		else if (m_ThrowUIinfo.iTextureNum == 5)
			m_iImgNum = 4;
		else if (m_ThrowUIinfo.iTextureNum == 6)
			m_iImgNum = 3;
	}
	else if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTMAP)
	{
		if (m_ThrowUIinfo.iTextureNum == 4)
			m_iImgNum = 0;
		else if (m_ThrowUIinfo.iTextureNum == 5)
			m_iImgNum = 1;
		else if (m_ThrowUIinfo.iTextureNum == 6)
			m_iImgNum = 2;
		else if (m_ThrowUIinfo.iTextureNum == 7)
			m_iImgNum = 3;
		else if (m_ThrowUIinfo.iTextureNum == 8)
			m_iImgNum = 4;
	}

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
	{
		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, -500.f, 100.f)));

		if (m_ThrowUIinfo.iTextureNum == 4)
		{
			if (m_ThrowUIinfo.iLayerNum == 0)
				m_iMoveCount = 25;
			else if (m_ThrowUIinfo.iLayerNum == 1)
				m_iMoveCount = 50;
			else if (m_ThrowUIinfo.iLayerNum == 2)
				m_iMoveCount = 75;
			else if (m_ThrowUIinfo.iLayerNum == 3)
				m_iMoveCount = 0;
		}
		else if (m_ThrowUIinfo.iTextureNum == 5)
		{
			if (m_ThrowUIinfo.iLayerNum == 0)
				m_iMoveCount = 0;
			else if (m_ThrowUIinfo.iTextureNum == 1)
				m_iMoveCount = 25;
		}
	}
	else if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTMAP)
	{
		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));
		if (m_ThrowUIinfo.iTextureNum == 6)
		{
			if(m_ThrowUIinfo.iLayerNum == 0)
				m_iMoveCount = 25;
			else if(m_ThrowUIinfo.iLayerNum == 2)
				m_iMoveCount = 45;
			else if (m_ThrowUIinfo.iLayerNum == 4)
				m_iMoveCount = 75;
		}
		else if (m_ThrowUIinfo.iTextureNum == 8)
		{
			if (m_ThrowUIinfo.iLayerNum == 0)
				m_iMoveCount = 25;
			else if (m_ThrowUIinfo.iLayerNum == 1 || m_ThrowUIinfo.iLayerNum == 2)
				m_iMoveCount = 55;
		}
	}
	return S_OK;
}

void CResultCloud::Tick(_float fTimeDelta)
{
	//m_fMoveTime += fTimeDelta;

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
	{
		Level_ResultMove();
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -50.f, 1.f));
	}
	else if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTMAP)
	{
		Level_MapSelectMove();
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
	}

	
	
	
}

void CResultCloud::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CResultCloud::Render()
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

	return S_OK;
}

void CResultCloud::Level_ResultMove()
{
	if (m_ThrowUIinfo.iTextureNum == 4)
	{
		if (m_ThrowUIinfo.iLayerNum == 1 || m_ThrowUIinfo.iLayerNum == 3)
		{
			if (m_iMoveCount >= 100)
				m_bMoveCheck = true;
			else if (m_iMoveCount <= 0)
				m_bMoveCheck = false;

			if (!m_bMoveCheck)
			{
				m_iMoveCount += 1;
				m_fX += 0.3f;
			}
			else
			{
				m_iMoveCount -= 1;
				m_fX -= 0.3f;
			}
		}
		else if (m_ThrowUIinfo.iLayerNum == 0 || m_ThrowUIinfo.iLayerNum == 2)
		{
			if (m_iMoveCount >= 100)
				m_bMoveCheck = true;
			else if (m_iMoveCount <= 0)
				m_bMoveCheck = false;

			if (!m_bMoveCheck)
			{
				m_iMoveCount += 1;
				m_fY -= 0.3f;
			}
			else
			{
				m_iMoveCount -= 1;
				m_fY += 0.3f;
			}
		}
	}
	else if (m_ThrowUIinfo.iTextureNum == 5)
	{
		if (m_iMoveCount >= 100)
			m_bMoveCheck = true;
		else if (m_iMoveCount <= 0)
			m_bMoveCheck = false;

		if (!m_bMoveCheck)
		{
			m_iMoveCount += 1;
			m_fX += 0.3f;
		}
		else
		{
			m_iMoveCount -= 1;
			m_fX -= 0.3f;
		}
	}
}

void CResultCloud::Level_MapSelectMove()
{
	if (m_ThrowUIinfo.iTextureNum == 6)
	{
		if (m_ThrowUIinfo.iLayerNum == 0)
		{
			if (m_iMoveCount >= 100)
				m_bMoveCheck = true;
			else if (m_iMoveCount <= 0)
				m_bMoveCheck = false;

			if (!m_bMoveCheck)
			{
				m_iMoveCount += 1;
				m_fY -= 0.3f;
			}
			else
			{
				m_iMoveCount -= 1;
				m_fY += 0.3f;
			}
		}
		else if (m_ThrowUIinfo.iLayerNum == 2 || m_ThrowUIinfo.iLayerNum == 4)
		{
			if (m_iMoveCount >= 100)
				m_bMoveCheck = true;
			else if (m_iMoveCount <= 0)
				m_bMoveCheck = false;

			if (!m_bMoveCheck)
			{
				m_iMoveCount += 1;
				m_fX += 0.3f;
			}
			else
			{
				m_iMoveCount -= 1;
				m_fX -= 0.3f;
			}
		}
	}
	else if (m_ThrowUIinfo.iTextureNum == 8)
	{
		if (m_ThrowUIinfo.iLayerNum == 0 || m_ThrowUIinfo.iLayerNum == 1 || m_ThrowUIinfo.iLayerNum == 2)
		{
			if (m_iMoveCount >= 100)
				m_bMoveCheck = true;
			else if (m_iMoveCount <= 0)
				m_bMoveCheck = false;

			if (!m_bMoveCheck)
			{
				m_iMoveCount += 1;
				m_fX += 0.3f;
			}
			else
			{
				m_iMoveCount -= 1;
				m_fX -= 0.3f;
			}
		}
	}
}

HRESULT CResultCloud::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultCloud"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CResultCloud::SetUp_ShaderResources()
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

CResultCloud * CResultCloud::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CResultCloud*	pInstance = new CResultCloud(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CResultCloud"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CResultCloud::Clone(void * pArg)
{
	CResultCloud*	pInstance = new CResultCloud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CResultCloud"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CResultCloud::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
