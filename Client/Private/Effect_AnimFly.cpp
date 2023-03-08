#include "stdafx.h"
#include "..\Public\Effect_AnimFly.h"
#include "GameInstance.h"
#include "Effect.h"

CEffect_AnimFly::CEffect_AnimFly(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect_AnimFly::CEffect_AnimFly(const CEffect_AnimFly & rhs)
	: CGameObj(rhs)
{
}

HRESULT CEffect_AnimFly::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_AnimFly::Initialize(void * pArg)
{
	m_pInfo = *(ANIM_FLYDESC*)pArg;

	m_pParents = m_pInfo.pParents;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_CurrentAnimIndex(0);

	//m_pTransformCom->Set_Scale(XMVectorSet(0.01f, 100.f, 100.f, 0.f));

	return S_OK;
}

void CEffect_AnimFly::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (4.9f <= m_fTime)
		m_bRender = false;
	else if (2.f <= m_fTime)
		m_bRender = true;

	if (m_fTime > m_pInfo.fLifeTime + m_pInfo.fStartTime) {
		Set_Dead();
	}
}

void CEffect_AnimFly::Late_Tick(_float fTimeDelta)
{
	if(true == m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	m_pModelCom->Play_Animation(fTimeDelta);
}

HRESULT CEffect_AnimFly::Render()
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
		//	return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pDiffuseTextureCom->Get_SRV(0))))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 7)))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEffect_AnimFly::SetUp_ShaderResources()
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

HRESULT CEffect_AnimFly::Ready_Components()
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

	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Shinobu_Spl_Butterfly"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	//	³ªºñ ¸ö¶×¾Æ¸®, ³¯°³
	if (FAILED(__super::Add_Components(TEXT("Com_DiffuseTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_T_N0052_V00_C00_0_Body_Ac"), (CComponent**)&m_pDiffuseTextureCom)))
		return E_FAIL;

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

CEffect_AnimFly * CEffect_AnimFly::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_AnimFly*	pInstance = new CEffect_AnimFly(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect_AnimFly"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_AnimFly::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect_AnimFly(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect_AnimFly"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_AnimFly::Free()
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
