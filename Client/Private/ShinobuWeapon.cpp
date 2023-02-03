#include "stdafx.h"
#include "..\Public\ShinobuWeapon.h"

#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "VIBuffer_Trail.h"
#include "SoundMgr.h"
#include "Data_Manager.h"

CShinobuWeapon::CShinobuWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CShinobuWeapon::CShinobuWeapon(const CShinobuWeapon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CShinobuWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShinobuWeapon::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_WeaponDesc, pArg, sizeof(WEAPONDESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(XMVectorSet(0.033f, 0.033f, 0.033f, 0.f));

	m_pTransformCom->Turn2(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.0f));
	//	m_pTransformCom->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.0f));
	//	m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(10.0f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, -500.f, 100.f)));


	return S_OK;
}

void CShinobuWeapon::Tick(_float fTimeDelta)
{
	//if (m_bMenu && !m_bTrue)
	//{
	//	_vector		vLook = { 0.3f,0.f,-1.f,0.f };

	//	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	//	_vector		vRight = XMVector3Cross(vAxisY, vLook);

	//	_vector		vUp = XMVector3Cross(vLook, vRight);

	//	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));
	//	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
	//	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	//	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	//	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, -500.f, 100.f)));

	//	m_bTrue = true;
	//}

	_matrix		SocketMatrix = m_WeaponDesc.pSocket->Get_CombinedTransformationMatrix() *
		XMLoadFloat4x4(&m_WeaponDesc.SocketPivotMatrix) * XMLoadFloat4x4(m_WeaponDesc.pParentWorldMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);


	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix);

	_matrix	matCollBox = XMLoadFloat4x4(&m_CombinedWorldMatrix);
	_vector vUp = matCollBox.r[1];
	matCollBox.r[3] += XMVector3Normalize(vUp) * 2.f;

}

void CShinobuWeapon::Late_Tick(_float fTimeDelta)
{


}

HRESULT CShinobuWeapon::Render()
{

	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (!m_bMenu)
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom2, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Render(m_pShaderCom2, i, 0)))
				return E_FAIL;
		}
	}


	return S_OK;
}

HRESULT CShinobuWeapon::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_CombinedWorldMatrix)));

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
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

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CShinobuWeapon::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader2"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxUIModel"), (CComponent**)&m_pShaderCom2)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;



	/* For.Com_Model*/

	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("ShinobuWeapon"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CShinobuWeapon::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_CombinedWorldMatrix)));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (!m_bMenu)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom2->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom2->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom2->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
			return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}


CShinobuWeapon * CShinobuWeapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CShinobuWeapon*	pInstance = new CShinobuWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CShinobuWeapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CShinobuWeapon::Clone(void * pArg)
{
	CShinobuWeapon*	pInstance = new CShinobuWeapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CShinobuWeapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShinobuWeapon::Free()
{
	__super::Free();

	Safe_Release(m_WeaponDesc.pSocket);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pShaderCom2);
	Safe_Release(m_pRendererCom);
}
