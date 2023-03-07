#include "stdafx.h"
#include "..\Public\Item.h"

#include "GameInstance.h"
#include "UI_Manager.h"
#include "Mini_Player.h"
CItem::CItem(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CItem::CItem(const CItem & rhs)
	: CGameObj(rhs)
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void * pArg)
{
	m_pPlayer = *(CGameObj**)pArg;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;
	m_fX = _float(rand() % 1180 + 50);
	m_fY = _float(rand() % 620 + 50);

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	_int iDest = rand() % 4;
	_int iRandScore = 0;
	switch (iDest)
	{
	case 0:
		m_iTextrueNum = 5;
		iRandScore = rand() % 20 + 21;
		m_iScore = iRandScore;
		break;
	case 1:
		m_iTextrueNum = 6;
		iRandScore = rand() % 30 + 31;
		m_iScore = iRandScore;
		break;
	case 2:
		m_iTextrueNum = 7;
		iRandScore = rand() % 50 + 51;
		m_iScore = iRandScore;
		break;
	case 3:
		m_iTextrueNum = 8;
		iRandScore = rand() % 10 + 11;
		m_iScore = iRandScore;
		break;
	default:
		break;
	}
	
	return S_OK;
}

void CItem::Tick(_float fTimeDelta)
{
	if (g_bMiniGame)
		return;
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - vPos));
	if (fDist < 40.f)
	{
		dynamic_cast<CMini_Player*>(m_pPlayer)->Set_Eat();
		dynamic_cast<CMini_Player*>(m_pPlayer)->Set_Score(m_iScore);
		Set_Dead();
	}
	m_fDeadTime += fTimeDelta;
	if (m_fDeadTime > 6.f)
		Set_Dead();
	
}

void CItem::Late_Tick(_float fTimeDelta)
{
	if (g_bMiniGame)
		Set_Dead();

	if (m_fDeadTime > 4.5f)
	{
		m_fRenderTime += fTimeDelta;
		if (m_fRenderTime > 0.1f)
		{
			m_bRenderItem = !m_bRenderItem;
			m_fRenderTime = 0.f;
		}
	}
	
	if (nullptr != m_pRendererCom && !m_bRenderItem)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	
}

HRESULT CItem::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	return S_OK;
}



HRESULT CItem::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 100.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MiniGame"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CItem::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iTextrueNum))))
		return E_FAIL;

	m_pShaderCom->Begin();
	m_pVIBufferCom->Render();

	return S_OK;
}

CItem * CItem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CItem*	pInstance = new CItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CItem::Clone(void * pArg)
{
	CItem*	pInstance = new CItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem::Free()
{
	__super::Free();


}
