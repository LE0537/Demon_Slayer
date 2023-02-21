#include "stdafx.h"
#include "..\Public\MeshObj_Smell.h"

#include "GameInstance.h"
#include "Data_Manager.h"

CMeshObj_Smell::CMeshObj_Smell(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMeshObj_Smell::CMeshObj_Smell(const CMeshObj_Smell & rhs)
	: CGameObj(rhs)
{
}

HRESULT CMeshObj_Smell::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMeshObj_Smell::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tMyDesc, pArg, sizeof m_tMyDesc);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_matrix		matWorld = XMLoadFloat4x4(&m_tMyDesc.matWorld);
	m_pTransformCom->Set_WorldMatrix(matWorld);

	(*(MESHOBJ_SMELLDESC**)pArg)->pMe = this;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMeshObj_Smell::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), fTimeDelta * 1.f / 83.f);

	/*if (pGameInstance->Key_Down(DIK_F5))
	{

		CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
		char cName[MAX_PATH];
		ZeroMemory(cName, sizeof(char) * MAX_PATH);
		if (2087 == m_tMyDesc.iModelIndex)
		{
			pData_Manager->TCtoC(TEXT("Smell1"), cName);
			ERR_MSG(L"Clear1");
		}
		if (2088 == m_tMyDesc.iModelIndex)
		{
			pData_Manager->TCtoC(TEXT("Smell2"), cName);
			ERR_MSG(L"Clear1");
		}
		if (2089 == m_tMyDesc.iModelIndex)
		{
			pData_Manager->TCtoC(TEXT("Smell3"), cName);
			ERR_MSG(L"Clear1");
		}
		pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_NONANIM);
		RELEASE_INSTANCE(CData_Manager);

	}*/

	RELEASE_INSTANCE(CGameInstance);


}

void CMeshObj_Smell::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix		matWorld = m_pTransformCom->Get_WorldMatrix();

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CMeshObj_Smell::Render()
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
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_SHININESS)))
			return E_FAIL;		//	Glow용 ( Max에서 glossiness 에 넣으면 됨.)

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 4)))
			return E_FAIL;
	}



	return S_OK;
}

HRESULT CMeshObj_Smell::Ready_Components()
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

	/* For.Com_NoiseTexture*/
	if (FAILED(__super::Add_Components(TEXT("Com_NoiseTexture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise_Smell"), (CComponent**)&m_pNoiseTextureCom)))
		return E_FAIL;


	/* For.Com_Model*/
	_tchar	pPrototypeTag_Model[MAX_PATH] = L"";
	switch (m_tMyDesc.iModelIndex)
	{
	case 2087: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Smell1");
	case 2088: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Smell2");
	case 2089: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Smell3");

	}

	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, pPrototypeTag_Model, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshObj_Smell::SetUp_ShaderResources()
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


	if (FAILED(m_pShaderCom->Set_RawValue("g_fGlowPower", &m_tMyDesc.fGlowPower, sizeof(_float))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentTime", &m_fAliveTime, sizeof(_float))))
		return E_FAIL;

	_float		fDistortionU = 0.01f;
	_float		fDistortionV = 0.005f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionU", &fDistortionU, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionV", &fDistortionV, sizeof(_float))))
		return E_FAIL;
	_float		fDistortionScale = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionScale", &fDistortionScale, sizeof(_float))))
		return E_FAIL;
	_float		fDistortionBias = 3.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionBias", &fDistortionBias, sizeof(_float))))
		return E_FAIL;

	_float		fMoveUV_U = 0.25f * m_fAliveTime;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMoveUV_U", &fMoveUV_U, sizeof(_float))))
		return E_FAIL;
	_float		fMoveUV_V = 1.f * m_fAliveTime;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMoveUV_V", &fMoveUV_V, sizeof(_float))))
		return E_FAIL;

	_int		iMulUV_U = 1;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iMulUV_U", &iMulUV_U, sizeof(_float))))
		return E_FAIL;
	_int		iMulUV_V = 1;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iMulUV_V", &iMulUV_V, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_NoiseTexture", m_pNoiseTextureCom->Get_SRV(1))))
		return E_FAIL;


	return S_OK;
}

CMeshObj_Smell * CMeshObj_Smell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMeshObj_Smell*	pInstance = new CMeshObj_Smell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshObj_Smell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMeshObj_Smell::Clone(void * pArg)
{
	CMeshObj_Smell*	pInstance = new CMeshObj_Smell(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshObj_Smell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshObj_Smell::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

	Safe_Release(m_pNoiseTextureCom);

}
