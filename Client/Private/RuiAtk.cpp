#include "stdafx.h"
#include "..\Public\RuiAtk.h"

#include "GameInstance.h"

CRuiAtk::CRuiAtk(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiAtk::CRuiAtk(const CRuiAtk & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiAtk::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiAtk::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	*(CRuiAtk**)pArg = this;

	return S_OK;
}

void CRuiAtk::Tick(_float fTimeDelta)
{
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CRuiAtk::Late_Tick(_float fTimeDelta)
{
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CRuiAtk::Render()
{
	return S_OK;
}

HRESULT CRuiAtk::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiAtk::Ready_Components()
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
	ColliderDesc.vScale = _float3(5.f, 3.f, 5.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CRuiAtk * CRuiAtk::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiAtk*	pInstance = new CRuiAtk(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiAtk"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiAtk::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiAtk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiAtk"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiAtk::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

