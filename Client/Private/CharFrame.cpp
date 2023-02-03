#include "stdafx.h"
#include "CharFrame.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CCharFrame::CCharFrame(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CCharFrame::CCharFrame(const CCharFrame & rhs)
	: CUI(rhs)
{
}

HRESULT CCharFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharFrame::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	Set_Info();
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	pUI_Manager->Set_SelectFrame(this, m_ThrowUIinfo.iLayerNum);

	RELEASE_INSTANCE(CUI_Manager);

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

void CCharFrame::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 0.f, 1.f));
}

void CCharFrame::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCharFrame::Render()
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

void CCharFrame::Set_Info()
{
	if (m_ThrowUIinfo.iLayerNum == 0)
	{
		m_SelectInfo.strName = TEXT("탄지로");
		m_SelectInfo.bOni = false;
		m_SelectInfo.iFrameNum = m_ThrowUIinfo.iLayerNum;
	}
	else if (m_ThrowUIinfo.iLayerNum == 1)
	{
		m_SelectInfo.strName = TEXT("쿄주로");
		m_SelectInfo.bOni = false;
		m_SelectInfo.iFrameNum = m_ThrowUIinfo.iLayerNum;
	}
	else if (m_ThrowUIinfo.iLayerNum == 2)
	{
		m_SelectInfo.strName = TEXT("루이");
		m_SelectInfo.bOni = true;
		m_SelectInfo.iFrameNum = m_ThrowUIinfo.iLayerNum;
	}
	else if (m_ThrowUIinfo.iLayerNum == 3)
	{
		m_SelectInfo.strName = TEXT("아카자");
		m_SelectInfo.bOni = true;
		m_SelectInfo.iFrameNum = m_ThrowUIinfo.iLayerNum;
	}
	else if (m_ThrowUIinfo.iLayerNum == 4)
	{
		m_SelectInfo.strName = TEXT("네즈코");
		m_SelectInfo.bOni = false;
		m_SelectInfo.iFrameNum = m_ThrowUIinfo.iLayerNum;
	}
	else if (m_ThrowUIinfo.iLayerNum == 5)
	{
		m_SelectInfo.strName = TEXT("시노부");
		m_SelectInfo.bOni = false;
		m_SelectInfo.iFrameNum = m_ThrowUIinfo.iLayerNum;
	}
}

HRESULT CCharFrame::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelFrame"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharFrame::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (m_ThrowUIinfo.iLayerNum == 0)
		m_iImgNum = 0;
	else if (m_ThrowUIinfo.iLayerNum == 1)
		m_iImgNum = 0;
	else if (m_ThrowUIinfo.iLayerNum == 2)
		m_iImgNum = 1;
	else if (m_ThrowUIinfo.iLayerNum == 3)
		m_iImgNum = 1;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;

	return S_OK;
}

CCharFrame * CCharFrame::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCharFrame*	pInstance = new CCharFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCharFrame"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCharFrame::Clone(void * pArg)
{
	CCharFrame*	pInstance = new CCharFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCharFrame"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharFrame::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
