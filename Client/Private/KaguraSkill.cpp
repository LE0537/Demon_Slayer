#include "stdafx.h"
#include "..\Public\KaguraSkill.h"

#include "GameInstance.h"

CKaguraSkill::CKaguraSkill(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CKaguraSkill::CKaguraSkill(const CKaguraSkill & rhs)
	: CCollBox(rhs)
{
}

HRESULT CKaguraSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKaguraSkill::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	*(CKaguraSkill**)pArg = this;

	return S_OK;
}

void CKaguraSkill::Tick(_float fTimeDelta)
{
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CKaguraSkill::Late_Tick(_float fTimeDelta)
{
	if (g_bCollBox)
	{
		m_pRendererCom->Add_CollBox(m_pOBBCom);
	}
}

HRESULT CKaguraSkill::Render()
{
	return S_OK;
}

HRESULT CKaguraSkill::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CKaguraSkill::Ready_Components()
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
	ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CKaguraSkill * CKaguraSkill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CKaguraSkill*	pInstance = new CKaguraSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CKaguraSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CKaguraSkill::Clone(void * pArg)
{
	CGameObject*	pInstance = new CKaguraSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CKaguraSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKaguraSkill::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

