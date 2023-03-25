#include "stdafx.h"
#include "..\Public\RuiMoveSkill.h"

#include "GameInstance.h"
#include "Effect_Manager.h"
CRuiMoveSkill::CRuiMoveSkill(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiMoveSkill::CRuiMoveSkill(const CRuiMoveSkill & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiMoveSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiMoveSkill::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	*(CRuiMoveSkill**)pArg = this;

	return S_OK;
}

void CRuiMoveSkill::Tick(_float fTimeDelta)
{

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());


}

void CRuiMoveSkill::Late_Tick(_float fTimeDelta)
{


	if (g_bCollBox)
	{
		m_pRendererCom->Add_CollBox(m_pOBBCom);
	}
}

HRESULT CRuiMoveSkill::Render()
{
	return S_OK;
}

HRESULT CRuiMoveSkill::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiMoveSkill::Ready_Components()
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

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(15.5f, 10.f, 15.5f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CRuiMoveSkill * CRuiMoveSkill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiMoveSkill*	pInstance = new CRuiMoveSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiMoveSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiMoveSkill::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiMoveSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiMoveSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiMoveSkill::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

