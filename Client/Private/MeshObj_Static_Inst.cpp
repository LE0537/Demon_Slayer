#include "stdafx.h"
#include "..\Public\MeshObj_Static_Inst.h"

#include "GameInstance.h"

CMeshObj_Static_Inst::CMeshObj_Static_Inst(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMeshObj_Static_Inst::CMeshObj_Static_Inst(const CMeshObj_Static_Inst & rhs)
	: CGameObj(rhs)
{
}

void CMeshObj_Static_Inst::Change_Model(_uint iObjNumber, const _tchar * pModel_PrototypeTag)
{
	if (FAILED(Delete_Component(TEXT("Com_Model"))))
		return;

	Safe_Release(m_pModelCom);

	__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, pModel_PrototypeTag, (CComponent**)&m_pModelCom);
	if (nullptr == m_pModelCom)
		return;
}

HRESULT CMeshObj_Static_Inst::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMeshObj_Static_Inst::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tMyDesc, pArg, sizeof m_tMyDesc);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	vector<VTXMATRIX>		vecMatrix;
	for (_uint i = 0; i < m_tMyDesc.iNumInstancing; ++i)
	{
		VTXMATRIX	VtxMatrix;
		memcpy(&VtxMatrix, &m_tMyDesc.pWorld[i], sizeof VtxMatrix);

		vecMatrix.push_back(VtxMatrix);
	}

	m_pModelCom->Update_Instancing(vecMatrix, 1.f / 60.f);

	return S_OK;
}

void CMeshObj_Static_Inst::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CMeshObj_Static_Inst::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	}
}

HRESULT CMeshObj_Static_Inst::Render()
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

HRESULT CMeshObj_Static_Inst::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	_vector			vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
	_vector			vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
	_vector			vLightUp = { 0.f, 1.f, 0.f ,0.f };
	_matrix			matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
/*
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;
*/
	}

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

HRESULT CMeshObj_Static_Inst::Delete_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_Components.erase(iter);

	return S_OK;
}

HRESULT CMeshObj_Static_Inst::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Ready_ModelComponent()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshObj_Static_Inst::SetUp_ShaderResources()
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

HRESULT CMeshObj_Static_Inst::Ready_ModelComponent()
{
	_tchar	pPrototypeTag_Model[MAX_PATH] = L"";
	CModel::MESHINSTANCINGDESC tMeshInstancingDesc;
	tMeshInstancingDesc.iNumMeshInstancing = m_tMyDesc.iNumInstancing;
	switch (m_tMyDesc.iModelIndex)
	{
	case 2001: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BigTree1_Instancing"); break;
	case 2002: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BigTree2_Instancing"); break;
	case 2003: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BigTree3_Instancing"); break;

	case 2004: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Cliff1_Instancing"); break;
	case 2005: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Cliff2_Instancing"); break;
	case 2006: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Cliff3_Instancing"); break;
	case 2007: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Cliff_Small_Instancing"); break;

	case 2008: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Grass1_Instancing"); break;
	case 2009: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Grass2_Instancing"); break;
	case 2010: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Grass3_Instancing"); break;
	case 2011: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Grass4_Instancing"); break;

	case 2012: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Hill_Far1_Instancing"); break;
	case 2013: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Hill_Far2_Instancing"); break;

	case 2014: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_SpiderWeb1_Instancing"); break;
	case 2015: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_SpiderWeb2_Instancing"); break;
	case 2016: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_SpiderWeb3_Instancing"); break;

	case 2017: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_RuiGround_Instancing"); break;
	}


	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, pPrototypeTag_Model, (CComponent**)&m_pModelCom, &tMeshInstancingDesc)))
		return E_FAIL;

	return S_OK;
}

CMeshObj_Static_Inst * CMeshObj_Static_Inst::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMeshObj_Static_Inst*	pInstance = new CMeshObj_Static_Inst(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshObj_Static_Inst"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMeshObj_Static_Inst::Clone(void * pArg)
{
	CMeshObj_Static_Inst*	pInstance = new CMeshObj_Static_Inst(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshObj_Static_Inst"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshObj_Static_Inst::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
