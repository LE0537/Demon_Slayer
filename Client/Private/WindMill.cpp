#include "stdafx.h"
#include "..\Public\WindMill.h"

#include "GameInstance.h"

CWindMill::CWindMill(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CWindMill::CWindMill(const CWindMill & rhs)
	: CCollBox(rhs)
{
}

HRESULT CWindMill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWindMill::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	*(CWindMill**)pArg = this;

	return S_OK;
}

void CWindMill::Tick(_float fTimeDelta)
{
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CWindMill::Late_Tick(_float fTimeDelta)
{
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CWindMill::Render()
{
	return S_OK;
}

HRESULT CWindMill::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CWindMill::Ready_Components()
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


	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(10.f, 5.f, 10.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CWindMill * CWindMill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CWindMill*	pInstance = new CWindMill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CWindMill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CWindMill::Clone(void * pArg)
{
	CGameObject*	pInstance = new CWindMill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CWindMill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWindMill::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

