#include "stdafx.h"
#include "..\Public\RuiDadJump.h"

#include "GameInstance.h"

CRuiDadJump::CRuiDadJump(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiDadJump::CRuiDadJump(const CRuiDadJump & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiDadJump::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiDadJump::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	*(CRuiDadJump**)pArg = this;

	return S_OK;
}

void CRuiDadJump::Tick(_float fTimeDelta)
{
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CRuiDadJump::Late_Tick(_float fTimeDelta)
{
	if (g_bCollBox)
	{
		m_pRendererCom->Add_CollBox(m_pOBBCom);
	}
}

HRESULT CRuiDadJump::Render()
{
	return S_OK;
}

HRESULT CRuiDadJump::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiDadJump::Ready_Components()
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
	ColliderDesc.vScale = _float3(8.f, 8.f, 8.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CRuiDadJump * CRuiDadJump::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiDadJump*	pInstance = new CRuiDadJump(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiDadJump"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiDadJump::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiDadJump(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiDadJump"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiDadJump::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

