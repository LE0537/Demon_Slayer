#include "stdafx.h"
#include "RankIcon.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "FadeUIEff.h"

CRankIcon::CRankIcon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CRankIcon::CRankIcon(const CRankIcon & rhs)
	: CUI(rhs)
{
}

HRESULT CRankIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRankIcon::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
	{
		m_fSizeX = m_ThrowUIinfo.vScale.x * 1.5f;
		m_fSizeY = m_ThrowUIinfo.vScale.y * 1.5f;
		m_fX = m_ThrowUIinfo.vPos.x + 20.f;
	}
	else
	{
		m_fSizeX = m_ThrowUIinfo.vScale.x * 1.2f;
		m_fSizeY = m_ThrowUIinfo.vScale.y * 1.2f;
		m_fX = m_ThrowUIinfo.vPos.x;
	}

	m_fY = m_ThrowUIinfo.vPos.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360)
		m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_ThrowUIinfo.vRot));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	CUI_Manager*  pUI_Manager = GET_INSTANCE(CUI_Manager);
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
	{
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, -500.f, 100.f)));
		if (!m_ThrowUIinfo.bPlyCheck)
			m_iRankScore = pUI_Manager->Get_RankInfo(0).iRankScore;
		else
			m_iRankScore = pUI_Manager->Get_RankInfo(1).iRankScore;

		if (m_iRankScore >= 0 && m_iRankScore < 1000)
			m_iImgNum = 0;
		else if (m_iRankScore >= 1000 && m_iRankScore < 2000)
			m_iImgNum = 1;
		else if (m_iRankScore >= 2000 && m_iRankScore < 3000)
			m_iImgNum = 2;
		else if (m_iRankScore >= 3000 && m_iRankScore < 4000)
			m_iImgNum = 3;
		else if (m_iRankScore >= 5000)
			m_iImgNum = 4;
	}
	else
	{
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

		m_iRankScore = pUI_Manager->Get_RankInfo(0).iRankScore;

		if (m_iRankScore >= 0 && m_iRankScore < 500)
			m_iImgNum = 0;
		else if (m_iRankScore >= 500 && m_iRankScore < 1000)
			m_iImgNum = 1;
		else if (m_iRankScore >= 1000 && m_iRankScore < 1500)
			m_iImgNum = 2;
		else if (m_iRankScore >= 1500 && m_iRankScore < 2000)
			m_iImgNum = 3;
		else if (m_iRankScore >= 2000)
			m_iImgNum = 4;

	}

	pUI_Manager->Set_RankIcon(this);

	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

void CRankIcon::Tick(_float fTimeDelta)
{
	CUI_Manager*  pUI_Manager = GET_INSTANCE(CUI_Manager);
	if (m_bZoomStart && m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
	{
		if (m_fSizeX >= m_ThrowUIinfo.vScale.x * 0.5f && m_fSizeY >= m_ThrowUIinfo.vScale.y * 0.5f && !m_bMinusCheck)
		{
			m_fSizeX -= 20.f;
			m_fSizeY -= 20.f;
		}
		else
		{
			m_bMinusCheck = true;
			if (m_fSizeX <= m_ThrowUIinfo.vScale.x && m_fSizeY <= m_ThrowUIinfo.vScale.y)
			{
				m_fSizeX += 20.f;
				m_fSizeY += 20.f;
			}
			else
				m_bZoomEnd = true;

		}
	}
	else if(m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY)
	{
		if (pUI_Manager->Get_FadeSwitch()&& !m_bZoomEnd)
		{
			m_fFadeTime += 0.05f;
			m_fSizeX -= 5.f;
			m_fSizeY -= 5.f;
			if (m_fFadeTime >= 1.f)
			{
				m_fFadeTime = 1.f;
				m_fSizeX = m_ThrowUIinfo.vScale.x;
				m_fSizeY = m_ThrowUIinfo.vScale.y;
				m_bZoomEnd = true;
			}
		}
		if (m_bZoomEnd)
			m_fStopTime += fTimeDelta;

		if (m_fStopTime >= 1.f)
		{
			m_fFadeTime -= 0.05f;

			if (m_fFadeTime <= 0.f)
				pUI_Manager->Set_AdvResult(true);
		}
		
	}
	
	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));
	if(m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -50.f, 1.f));
	else
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
	RELEASE_INSTANCE(CUI_Manager);
}

void CRankIcon::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
	{
		if(m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
		else
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);

	}
}

HRESULT CRankIcon::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
	{
		if (!m_ThrowUIinfo.bReversal)
			m_pShaderCom->Begin();
		else
			m_pShaderCom->Begin(1);
	}
	else if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY)
	{
		m_pShaderCom->Begin(12);
	}

	if(m_bZoomStart && m_ThrowUIinfo.iLevelIndex == LEVEL_GAMERESULT)
		m_pVIBufferCom->Render();
	else 
		m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CRankIcon::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_RankIcon"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRankIcon::SetUp_ShaderResources()
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

CRankIcon * CRankIcon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRankIcon*	pInstance = new CRankIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRankIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRankIcon::Clone(void * pArg)
{
	CRankIcon*	pInstance = new CRankIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRankIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRankIcon::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
