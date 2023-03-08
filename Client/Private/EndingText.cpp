#include "stdafx.h"
#include "..\Public\EndingText.h"

#include "GameInstance.h"
#include "Data_Manager.h"
#include "UI_Manager.h"
#include "Tanjiro.h"
#include "Layer.h"
CEndingText::CEndingText(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEndingText::CEndingText(const CEndingText & rhs)
	: CGameObj(rhs)
{
}
HRESULT CEndingText::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEndingText::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tMyDesc, pArg, sizeof m_tMyDesc);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_tMyDesc.vPos));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scale(XMVectorSet(1.f, 1.f, 3.f, 0.f));

	return S_OK;
}

void CEndingText::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CEndingText::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}

}

HRESULT CEndingText::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 7)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CEndingText::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	const LIGHTDESC* pLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW);
	if (nullptr != pLightDesc)
	{
		_vector			vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDirection);
		_vector			vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDiffuse);
		_vector			vLightUp = { 0.f, 1.f, 0.f ,0.f };
		_matrix			matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProjOrigin)), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

HRESULT CEndingText::Delete_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_Components.erase(iter);

	return S_OK;
}

HRESULT CEndingText::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORMDESC));
	tTransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tTransformDesc.fSpeedPerSec = 20.f;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Ready_ModelComponent()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEndingText::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);





	return S_OK;
}

HRESULT CEndingText::Ready_ModelComponent()
{
	switch (m_tMyDesc.iModelIndex)
	{
	case 0:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, L"Prototype_Component_Model_Frame", (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, L"Prototype_Component_Model_Shader", (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, L"Prototype_Component_Model_Animation", (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, L"Prototype_Component_Model_Effect", (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case 4:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, L"Prototype_Component_Model_UI", (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		break;
	}
	

	return S_OK;
}

CEndingText * CEndingText::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEndingText*	pInstance = new CEndingText(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEndingText"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CEndingText::Clone(void * pArg)
{
	CEndingText*	pInstance = new CEndingText(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEndingText"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEndingText::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
