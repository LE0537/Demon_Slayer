#include "stdafx.h"
#include "SelNameShadow.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "1PMainOnBase.h"
#include "2PMainOnBase.h"
#include "SelP1Cursor.h"
#include "SelP2Cursor.h"

CSelNameShadow::CSelNameShadow(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CSelNameShadow::CSelNameShadow(const CSelNameShadow & rhs)
	: CUI(rhs)
{
}

HRESULT CSelNameShadow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSelNameShadow::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x * 1.5f;
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

void CSelNameShadow::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	C1PMainOnBase* p1PMain = dynamic_cast<C1PMainOnBase*>(pUI_Manager->Get_Sel1PMain(0));
	C1PMainOnBase* p1PMain_1 = dynamic_cast<C1PMainOnBase*>(pUI_Manager->Get_Sel1PMain(1));
	C2PMainOnBase* p2PMain = dynamic_cast<C2PMainOnBase*>(pUI_Manager->Get_Sel2PMain(0));
	C2PMainOnBase* p2PMain_1 = dynamic_cast<C2PMainOnBase*>(pUI_Manager->Get_Sel2PMain(1));

	if (p1PMain != nullptr && p2PMain != nullptr)
	{
		if (m_ThrowUIinfo.iLayerNum == 0)
		{
			m_fMainPos.x = p1PMain->Get_fX();
			m_fMainPos.y = p1PMain->Get_fY();
		}
		else if (m_ThrowUIinfo.iLayerNum == 1)
		{
			m_fMainPos.x = p2PMain->Get_fX();
			m_fMainPos.y = p2PMain->Get_fY();
		}
		else if (m_ThrowUIinfo.iLayerNum == 2)
		{
			m_fMainPos.x = p1PMain_1->Get_fX();
			m_fMainPos.y = p1PMain_1->Get_fY();
		}
		else if (m_ThrowUIinfo.iLayerNum == 3)
		{
			m_fMainPos.x = p2PMain_1->Get_fX();
			m_fMainPos.y = p2PMain_1->Get_fY();
		}
		

		m_fX = m_fMainPos.x;
		m_fY = m_fMainPos.y - 3.f;
	}
	

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CSelNameShadow::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);
}

HRESULT CSelNameShadow::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(10);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	CSelP1Cursor* p1PCursor = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor());
	CSelP2Cursor* p2PCursor = dynamic_cast<CSelP2Cursor*>(pUI_Manager->Get_2PCursor());

	if(p1PCursor->Get_FirstSelCheck() && !p1PCursor->Get_SecondSelCheck() && m_ThrowUIinfo.iLayerNum == 0)
		m_pVIBufferCom->Render();
	if(!p1PCursor->Get_SelectUIInfo().bOni && p1PCursor->Get_SelectUIInfoSecond().bOni && m_ThrowUIinfo.iLayerNum == 2)
		m_pVIBufferCom->Render();

	if (p2PCursor->Get_FirstSelCheck() && !p2PCursor->Get_SecondSelCheck() && m_ThrowUIinfo.iLayerNum == 1)
		m_pVIBufferCom->Render();
	if (!p2PCursor->Get_SelectUIInfo().bOni && p2PCursor->Get_SelectUIInfoSecond().bOni && m_ThrowUIinfo.iLayerNum == 3)
		m_pVIBufferCom->Render();

	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

HRESULT CSelNameShadow::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelNameShdow"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSelNameShadow::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CSelNameShadow * CSelNameShadow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSelNameShadow*	pInstance = new CSelNameShadow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSelNameShadow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CSelNameShadow::Clone(void * pArg)
{
	CSelNameShadow*	pInstance = new CSelNameShadow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSelNameShadow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSelNameShadow::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
