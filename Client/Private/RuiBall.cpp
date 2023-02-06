#include "stdafx.h"
#include "..\Public\RuiBall.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
CRuiBall::CRuiBall(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiBall::CRuiBall(const CRuiBall & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiBall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiBall::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(RUISHOOTINFO));

	_vector vLook = m_ShootInfo.pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLook.m128_f32[1] += 1.5f;

	_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vPos.m128_f32[1] += 1.5f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->LookAt(vLook);

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_SHOTBALL_MAIN, this);
	RELEASE_INSTANCE(CEffect_Manager);
	return S_OK;
}

void CRuiBall::Tick(_float fTimeDelta)
{
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_fDeadTime += fTimeDelta;
	if (m_fDeadTime > 3.9f)
		m_pEffect->Set_Dead();
	if (m_fDeadTime > 4.f)
		Set_Dead();
}

void CRuiBall::Late_Tick(_float fTimeDelta)
{
	if (m_bEffectDead)
	{
		Set_Dead();
		return;
	}
	CCollider*	pMyCollider = m_pOBBCom;
	CCollider*	pTargetCollider = m_ShootInfo.pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider) && !m_bHit)
	{
		_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		//vPos.m128_f32[1] = 0.f;
		m_ShootInfo.pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

		if (m_ShootInfo.pTarget->Get_PlayerInfo().bGuard)
		{
			m_ShootInfo.pTarget->Get_GuardHit(0);
		}
		else
		{
			m_ShootInfo.pTarget->Set_Hp(-15);
			m_ShootInfo.pTarget->Take_Damage(0.1f, false);
			m_ShootInfo.pTarget->Get_BattleTarget()->Set_Combo(1);
			m_ShootInfo.pTarget->Get_BattleTarget()->Set_ComboTime(0.f);
		}

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_ShootInfo.pTarget);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bHit = true;
		m_pEffect->Set_Dead();
		m_bEffectDead = true;
	}

	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CRuiBall::Render()
{
	return S_OK;
}

HRESULT CRuiBall::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiBall::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 50.f;
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



CRuiBall * CRuiBall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiBall*	pInstance = new CRuiBall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiBall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiBall::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiBall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiBall::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}
