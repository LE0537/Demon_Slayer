#include "stdafx.h"
#include "..\Public\KyoujuroSkill.h"

#include "GameInstance.h"
#include "Effect_Manager.h"
CKyoujuroSkill::CKyoujuroSkill(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CKyoujuroSkill::CKyoujuroSkill(const CKyoujuroSkill & rhs)
	: CCollBox(rhs)
{
}

HRESULT CKyoujuroSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKyoujuroSkill::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	*(CKyoujuroSkill**)pArg = this;

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_5TIGER_MAIN, this);
	RELEASE_INSTANCE(CEffect_Manager);

	return S_OK;
}

void CKyoujuroSkill::Tick(_float fTimeDelta)
{
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fTime += fTimeDelta;

	if(m_fTime > 5.f)
	{ 
		m_pEffect->Set_Dead();
		
	}
}

void CKyoujuroSkill::Late_Tick(_float fTimeDelta)
{

	if (m_fTime > 5.1f)
	{
		
		Set_Dead();
	}
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CKyoujuroSkill::Render()
{
	return S_OK;
}

HRESULT CKyoujuroSkill::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CKyoujuroSkill::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 15.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(5.f, 10.f, 5.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 6.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CKyoujuroSkill * CKyoujuroSkill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CKyoujuroSkill*	pInstance = new CKyoujuroSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CKyoujuroSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CKyoujuroSkill::Clone(void * pArg)
{
	CGameObject*	pInstance = new CKyoujuroSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CKyoujuroSkill"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKyoujuroSkill::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

