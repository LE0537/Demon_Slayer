#include "stdafx.h"
#include "..\Public\AkazaShoot.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
CAkazaShoot::CAkazaShoot(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CAkazaShoot::CAkazaShoot(const CAkazaShoot & rhs)
	: CCollBox(rhs)
{
}

HRESULT CAkazaShoot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAkazaShoot::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(AKAZASHOOTINFO));

	_vector vLook = m_ShootInfo.pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLook.m128_f32[1] += 1.5f;

	_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vPos.m128_f32[1] += 1.5f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->LookAt(vLook);
	
	return S_OK;
}

void CAkazaShoot::Tick(_float fTimeDelta)
{
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_fDeadTime += fTimeDelta;
	if (m_fDeadTime > 4.f)
		Set_Dead();
}

void CAkazaShoot::Late_Tick(_float fTimeDelta)
{
	CCollider*	pMyCollider = m_pOBBCom;
	CCollider*	pTargetCollider = m_ShootInfo.pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider) && !m_bHit)
	{
		_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vPos.m128_f32[1] = 0.f;
		m_ShootInfo.pTarget->Get_Transform()->LookAt(vPos);

		if (m_ShootInfo.pTarget->Get_PlayerInfo().bGuard)
		{
			m_ShootInfo.pTarget->Get_GuardHit(0);
		}
		else
		{
			m_ShootInfo.pTarget->Set_Hp(-30);
			m_ShootInfo.pTarget->Take_Damage(0.1f, false);
		}

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_ShootInfo.pTarget);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bHit = true;
		Set_Dead();
	}
		
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CAkazaShoot::Render()
{
	return S_OK;
}

HRESULT CAkazaShoot::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CAkazaShoot::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 35.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CAkazaShoot * CAkazaShoot::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAkazaShoot*	pInstance = new CAkazaShoot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAkazaShoot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CAkazaShoot::Clone(void * pArg)
{
	CGameObject*	pInstance = new CAkazaShoot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAkazaShoot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAkazaShoot::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}
