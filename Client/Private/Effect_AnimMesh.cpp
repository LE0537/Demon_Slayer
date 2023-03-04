#include "stdafx.h"
#include "..\Public\Effect_AnimMesh.h"
#include "GameInstance.h"
#include "Effect.h"

CEffect_AnimMesh::CEffect_AnimMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect_AnimMesh::CEffect_AnimMesh(const CEffect_AnimMesh & rhs)
	: CGameObj(rhs)
{
}

HRESULT CEffect_AnimMesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_AnimMesh::Initialize(void * pArg)
{
	m_pInfo = *(ANIM_MESH_INFO*)pArg;

	m_pParents = m_pInfo.pParents;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_CurrentAnimIndex(0);

	//m_pTransformCom->Set_Scale(XMVectorSet(0.01f, 100.f, 100.f, 0.f));

	return S_OK;
}

void CEffect_AnimMesh::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > m_pInfo.fLifeTime + m_pInfo.fStartTime) {
		Set_Dead();
	}
}

void CEffect_AnimMesh::Late_Tick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	m_pModelCom->Play_Animation(fTimeDelta);
}

HRESULT CEffect_AnimMesh::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		//if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			//return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pDiffuseTextureCom->Get_SRV(0))))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEffect_AnimMesh::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEffect_AnimMesh::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	_tchar			szRealPath[MAX_PATH];
	//wcscpy_s(szRealPath, m_pInfo.szMeshType);

	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Rengoku_Spl_078_FlameTrail01_11"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_DiffuseTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireTest"), (CComponent**)&m_pDiffuseTextureCom)))
		return E_FAIL;

	/*if (0 != wcscmp(m_pInfo.szMeshDiffuse, TEXT(""))) {
		_tchar			szMeshDiffuse[MAX_PATH];
		wcscpy_s(szMeshDiffuse, m_pInfo.szMeshType);

		
	}

	if (0 != wcscmp(m_pInfo.szMeshDissolve, TEXT(""))) {
		_tchar			szMeshDissolve[MAX_PATH];
		wcscpy_s(szMeshDissolve, m_pInfo.szMeshType);

		if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, szMeshDissolve, (CComponent**)&m_pDissolveTextureCom)))
			return E_FAIL;
	}

	if (0 != wcscmp(m_pInfo.szMeshMask, TEXT(""))) {
		_tchar			szMeshMask[MAX_PATH];
		wcscpy_s(szMeshMask, m_pInfo.szMeshType);

		if (FAILED(__super::Add_Components(TEXT("Com_MaskTexture"), LEVEL_STATIC, szMeshMask, (CComponent**)&m_pMaskTextureCom)))
			return E_FAIL;
	}*/

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_pInfo.vPosition.x, m_pInfo.vPosition.y, m_pInfo.vPosition.z, 1.f));

	_float3		vRotation = m_pInfo.vRotation;
	vRotation.x = XMConvertToRadians(vRotation.x);
	vRotation.y = XMConvertToRadians(vRotation.y);
	vRotation.z = XMConvertToRadians(vRotation.z);
	m_pTransformCom->RotationAll(vRotation);

	//m_fTurnSpeed = m_MeshInfo.fTurn;
	m_ParentsMtr = m_pParents->Get_Transform()->Get_World4x4();

	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_ParentsMtr) * m_pTransformCom->Get_WorldMatrix());

	return S_OK;
}

CEffect_AnimMesh * CEffect_AnimMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_AnimMesh*	pInstance = new CEffect_AnimMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect_AnimMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_AnimMesh::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect_AnimMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect_AnimMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_AnimMesh::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pDissolveTextureCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pMaskTextureCom);
}
