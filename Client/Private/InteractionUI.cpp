#include "stdafx.h"
#include "InteractionUI.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Camera_Dynamic.h"

CInteractionUI::CInteractionUI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CInteractionUI::CInteractionUI(const CInteractionUI & rhs)
	: CUI(rhs)
{
}

HRESULT CInteractionUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInteractionUI::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 4.f;
	m_fSizeY = 0.5f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	return S_OK;
}

void CInteractionUI::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float4 vPos; 
	XMStoreFloat4(&vPos, pUI_Manager->Get_1P()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(vPos.x, vPos.y + 2.f, vPos.z, vPos.w));

	//dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"));
	/*dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->
	

	m_pTransformCom->LookAt(vCarmeraPos);*/

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CInteractionUI::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CInteractionUI::Render()
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

	m_pVIBufferCom->Render();

	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

HRESULT CInteractionUI::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_InteractionUI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInteractionUI::SetUp_ShaderResources()
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

CInteractionUI * CInteractionUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CInteractionUI*	pInstance = new CInteractionUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CInteractionUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CInteractionUI::Clone(void * pArg)
{
	CInteractionUI*	pInstance = new CInteractionUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CInteractionUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInteractionUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
