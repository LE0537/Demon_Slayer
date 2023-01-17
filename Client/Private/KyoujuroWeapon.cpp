#include "stdafx.h"
#include "..\Public\KyoujuroWeapon.h"

#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "VIBuffer_Trail.h"
#include "SoundMgr.h"
#include "Data_Manager.h"

CKyoujuroWeapon::CKyoujuroWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CKyoujuroWeapon::CKyoujuroWeapon(const CKyoujuroWeapon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CKyoujuroWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKyoujuroWeapon::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_WeaponDesc, pArg, sizeof(WEAPONDESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(XMVectorSet(0.033f, 0.033f, 0.033f, 0.f));

	m_pTransformCom->Turn2(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.0f));
	m_pTransformCom->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-20.0f));
	//m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(5.0f));



	return S_OK;
}

void CKyoujuroWeapon::Tick(_float fTimeDelta)
{
	if (m_bRender)
	{
		_matrix		SocketMatrix = m_WeaponDesc.pSocket->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_WeaponDesc.SocketPivotMatrix) * XMLoadFloat4x4(m_WeaponDesc.pParentWorldMatrix);

		SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
		SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
		SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);


		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix);

		_matrix	matCollBox = XMLoadFloat4x4(&m_CombinedWorldMatrix);
		_vector vUp = matCollBox.r[1];
		matCollBox.r[3] += XMVector3Normalize(vUp) * 2.f;

		m_pOBBCom->Update(matCollBox);

		if (g_bDebug)
		{
			m_pRendererCom->Add_Debug(m_pOBBCom);
		}
	}

}

void CKyoujuroWeapon::Late_Tick(_float fTimeDelta)
{


}

HRESULT CKyoujuroWeapon::Render()
{
	if (m_bRender)
	{
		if (nullptr == m_pShaderCom)
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

	}
	return S_OK;
}

HRESULT CKyoujuroWeapon::Render_ShadowDepth()
{
	return E_NOTIMPL;
}

HRESULT CKyoujuroWeapon::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/

	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("KyoujuroWeapon"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(50.f, 160.f, 50.f);
	ColliderDesc.vPosition = _float3(0.f, 30.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CKyoujuroWeapon::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_CombinedWorldMatrix)));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}


CKyoujuroWeapon * CKyoujuroWeapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CKyoujuroWeapon*	pInstance = new CKyoujuroWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CKyoujuroWeapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CKyoujuroWeapon::Clone(void * pArg)
{
	CKyoujuroWeapon*	pInstance = new CKyoujuroWeapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CKyoujuroWeapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKyoujuroWeapon::Free()
{
	__super::Free();

	Safe_Release(m_WeaponDesc.pSocket);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pRendererCom);
}
