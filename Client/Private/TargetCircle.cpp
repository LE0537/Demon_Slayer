#include "stdafx.h"
#include "..\Public\TargetCircle.h"
#include "GameInstance.h"


CTargetCircle::CTargetCircle(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CTargetCircle::CTargetCircle(const CTargetCircle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTargetCircle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTargetCircle::Initialize(void * pArg)
{

	if (pArg != nullptr)
	{
		if (FAILED(Ready_Components()))
			return E_FAIL;

		ZeroMemory(&m_tRangeCircleInfo, sizeof(m_tRangeCircleInfo));
		m_tRangeCircleInfo.vPosition = ((CTargetCircle::tagRangeCircleDesc*)(pArg))->vPosition;

		_vector vPosition = XMLoadFloat3(&m_tRangeCircleInfo.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);

		_float3 vScale = { 1.f,1.f,1.f };

		m_pTransformCom->Set_Scale(XMLoadFloat3(&vScale));

		//m_pTransformCom->Set_Rotation(_float3(90.f,0.f,0.f));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_iShaderPass = 0;
	}


	return S_OK;
}

void CTargetCircle::Tick(_float fTimeDelta)
{
	m_fCircleDeadTime += fTimeDelta;

	if (m_fCircleDeadTime >= 1.5f)
		m_bDead = true;



}

void CTargetCircle::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTargetCircle::Render()
{
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


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_iShaderPass)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTargetCircle::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 15.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_TargetCircle"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTargetCircle::SetUp_ShaderResources()
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

CTargetCircle * CTargetCircle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTargetCircle*	pInstance = new CTargetCircle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTargetCircle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTargetCircle::Clone(void * pArg)
{
	CTargetCircle*	pInstance = new CTargetCircle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTargetCircle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTargetCircle::Free()
{
	__super::Free();


	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
