#include "stdafx.h"
#include "..\Public\MeshObj_Static.h"

#include "GameInstance.h"

CMeshObj_Static::CMeshObj_Static(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMeshObj_Static::CMeshObj_Static(const CMeshObj_Static & rhs)
	: CGameObj(rhs)
{
}

void CMeshObj_Static::Change_Model(_uint iObjNumber, const _tchar * pModel_PrototypeTag)
{
	if (FAILED(Delete_Component(TEXT("Com_Model"))))
		return;

	Safe_Release(m_pModelCom);

	__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, pModel_PrototypeTag, (CComponent**)&m_pModelCom);
	if (nullptr == m_pModelCom)
		return;
}

HRESULT CMeshObj_Static::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMeshObj_Static::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tMyDesc, pArg, sizeof m_tMyDesc);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_matrix		matWorld = XMLoadFloat4x4(&m_tMyDesc.matWorld);
	m_pTransformCom->Set_WorldMatrix(matWorld);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMeshObj_Static::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CMeshObj_Static::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMeshObj_Static::Render()
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
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}



	return S_OK;
}

HRESULT CMeshObj_Static::Delete_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_Components.erase(iter);

	return S_OK;
}

HRESULT CMeshObj_Static::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORMDESC));
	tTransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tTransformDesc.fSpeedPerSec = 5.f;

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

HRESULT CMeshObj_Static::SetUp_ShaderResources()
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

HRESULT CMeshObj_Static::Ready_ModelComponent()
{
	_tchar	pPrototypeTag_Model[MAX_PATH] = L"";
	switch (m_tMyDesc.iModelIndex)
	{
	case 2001: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BigTree1"); break;
	case 2002: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BigTree2"); break;
	case 2003: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BigTree3"); break;

	case 2004: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Cliff1"); break;
	case 2005: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Cliff2"); break;
	case 2006: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Cliff3"); break;
	case 2007: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Cliff_Small"); break;

	case 2008: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Grass1"); break;
	case 2009: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Grass2"); break;
	case 2010: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Grass3"); break;
	case 2011: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Grass4"); break;

	case 2012: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Hill_Far1"); break;
	case 2013: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Hill_Far2"); break;

	case 2014: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_SpiderWeb1"); break;
	case 2015: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_SpiderWeb2"); break;
	case 2016: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_SpiderWeb3"); break;

	case 2017: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_RuiGround"); break;
	}


	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, pPrototypeTag_Model, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

CMeshObj_Static * CMeshObj_Static::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMeshObj_Static*	pInstance = new CMeshObj_Static(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshObj_Static"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMeshObj_Static::Clone(void * pArg)
{
	CMeshObj_Static*	pInstance = new CMeshObj_Static(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshObj_Static"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshObj_Static::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
