#include "stdafx.h"
#include "..\Public\CamLine.h"

#include "GameInstance.h"
#include "VIBuffer_Line.h"

CCamLine::CCamLine(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CCamLine::CCamLine(const CCamLine & rhs)
	: CGameObj(rhs)
{
}

void CCamLine::Set_Pos(_float fX, _float fY, _float fZ)
{
	m_vPos[2] = _float3(fX, fY, fZ);
	m_pVIBuffer_LineCom->Update(m_vPos[1], m_vPos[2]);
}

void CCamLine::Set_Pos(_float3 vPos[4])
{
	for (_int i = 0; i < 4; ++i)
		m_vPos[i] = vPos[i];

	m_pVIBuffer_LineCom->Update(m_vPos[1], m_vPos[2]);
}

HRESULT CCamLine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamLine::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CAMLINEDESC		tMyDesc;
	memcpy(&tMyDesc, pArg, sizeof(CAMLINEDESC));
	m_vColor = tMyDesc.vColor;
	for (_int i = 0; i < 4; ++i)
		m_vPos[i] = tMyDesc.vPos[i];

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	((CAMLINEDESC*)pArg)->pMe = this;

	return S_OK;
}

void CCamLine::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCamLine::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCamLine::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBuffer_LineCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin();
	m_pVIBuffer_LineCom->Render();



	//	Cube Cluster
	_matrix matWorld = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixTranslation(m_vPos[3].x, m_vPos[3].y, m_vPos[3].z);

	if (FAILED(m_pShader_ModelCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(matWorld), sizeof(_float4x4))))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(m_pShader_ModelCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader_ModelCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);
	if (FAILED(m_pShader_ModelCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float3))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShader_ModelCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShader_ModelCom, i, 5)))
			return E_FAIL;

	}
	//	m_pVIBuffer_CubeCom->Render();

	return S_OK;
}

HRESULT CCamLine::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxLine"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer*/
	m_pVIBuffer_LineCom = CVIBuffer_Line::Create(m_pDevice, m_pContext);



	/* For.Com_VIBufferCube */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBufferCube"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBuffer_CubeCom)))
		return E_FAIL;
	/* For.Com_ShaderCube */
	if (FAILED(__super::Add_Components(TEXT("Com_ShaderCube"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTex"), (CComponent**)&m_pShader_CubeCom)))
		return E_FAIL;


	/* For.Com_Model */
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Sphere2"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_ShaderModel */
	if (FAILED(__super::Add_Components(TEXT("Com_ShaderModel"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShader_ModelCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCamLine::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_matrix	matWorld = XMMatrixIdentity();
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	_float fValue = 1.f / 8.f;
	_vector vSplinePos = XMVectorCatmullRom(XMLoadFloat3(&m_vPos[0]), XMLoadFloat3(&m_vPos[1]), XMLoadFloat3(&m_vPos[2]), XMLoadFloat3(&m_vPos[3]), fValue);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSplinePos1", &vSplinePos, sizeof(_float3))))
		return E_FAIL;
	fValue = 2.f / 8.f;
	vSplinePos = XMVectorCatmullRom(XMLoadFloat3(&m_vPos[0]), XMLoadFloat3(&m_vPos[1]), XMLoadFloat3(&m_vPos[2]), XMLoadFloat3(&m_vPos[3]), fValue);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSplinePos2", &vSplinePos, sizeof(_float3))))
		return E_FAIL;
	fValue = 3.f / 8.f;
	vSplinePos = XMVectorCatmullRom(XMLoadFloat3(&m_vPos[0]), XMLoadFloat3(&m_vPos[1]), XMLoadFloat3(&m_vPos[2]), XMLoadFloat3(&m_vPos[3]), fValue);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSplinePos3", &vSplinePos, sizeof(_float3))))
		return E_FAIL;
	fValue = 4.f / 8.f;
	vSplinePos = XMVectorCatmullRom(XMLoadFloat3(&m_vPos[0]), XMLoadFloat3(&m_vPos[1]), XMLoadFloat3(&m_vPos[2]), XMLoadFloat3(&m_vPos[3]), fValue);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSplinePos4", &vSplinePos, sizeof(_float3))))
		return E_FAIL;
	fValue = 5.f / 8.f;
	vSplinePos = XMVectorCatmullRom(XMLoadFloat3(&m_vPos[0]), XMLoadFloat3(&m_vPos[1]), XMLoadFloat3(&m_vPos[2]), XMLoadFloat3(&m_vPos[3]), fValue);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSplinePos5", &vSplinePos, sizeof(_float3))))
		return E_FAIL;
	fValue = 6.f / 8.f;
	vSplinePos = XMVectorCatmullRom(XMLoadFloat3(&m_vPos[0]), XMLoadFloat3(&m_vPos[1]), XMLoadFloat3(&m_vPos[2]), XMLoadFloat3(&m_vPos[3]), fValue);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSplinePos6", &vSplinePos, sizeof(_float3))))
		return E_FAIL;
	fValue = 7.f / 8.f;
	vSplinePos = XMVectorCatmullRom(XMLoadFloat3(&m_vPos[0]), XMLoadFloat3(&m_vPos[1]), XMLoadFloat3(&m_vPos[2]), XMLoadFloat3(&m_vPos[3]), fValue);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSplinePos7", &vSplinePos, sizeof(_float3))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float3))))
		return E_FAIL;

	return S_OK;
}

CCamLine * CCamLine::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamLine*	pInstance = new CCamLine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamLine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCamLine::Clone(void * pArg)
{
	CCamLine*	pInstance = new CCamLine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamLine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamLine::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

	Safe_Release(m_pVIBuffer_LineCom);

	Safe_Release(m_pShader_CubeCom);
	Safe_Release(m_pVIBuffer_CubeCom);
	Safe_Release(m_pShader_ModelCom);
	Safe_Release(m_pModelCom);
}
