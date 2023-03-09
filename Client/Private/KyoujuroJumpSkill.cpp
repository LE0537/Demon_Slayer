#include "stdafx.h"
#include "..\Public\KyoujuroJumpSkill.h"

#include "GameInstance.h"
#include "Effect_Manager.h"

CKyoujuroJumpSkill::CKyoujuroJumpSkill(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CKyoujuroJumpSkill::CKyoujuroJumpSkill(const CKyoujuroJumpSkill & rhs)
	: CCollBox(rhs)
{
}

HRESULT CKyoujuroJumpSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKyoujuroJumpSkill::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	*(CKyoujuroJumpSkill**)pArg = this;

	return S_OK;
}

void CKyoujuroJumpSkill::Tick(_float fTimeDelta)
{
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());


	m_fDeadTime += fTimeDelta;
	//if (m_fDeadTime > 3.9f)
	//	m_pEffect->Set_Dead();
	if (m_fDeadTime > 1.3f)
	{

		Set_Dead();
	}
}

void CKyoujuroJumpSkill::Late_Tick(_float fTimeDelta)
{
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CKyoujuroJumpSkill::Render()
{
	return S_OK;
}

HRESULT CKyoujuroJumpSkill::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CKyoujuroJumpSkill::Ready_Components()
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
	ColliderDesc.vScale = _float3(8.f, 15.f, 8.f);
	ColliderDesc.vPosition = _float3(0.f, -3.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CKyoujuroJumpSkill * CKyoujuroJumpSkill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CKyoujuroJumpSkill*	pInstance = new CKyoujuroJumpSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CKyoujuroJumpSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CKyoujuroJumpSkill::Clone(void * pArg)
{
	CGameObject*	pInstance = new CKyoujuroJumpSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CKyoujuroJumpSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKyoujuroJumpSkill::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

