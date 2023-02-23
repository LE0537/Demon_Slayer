#include "stdafx.h"
#include "..\Public\RuiBigBall.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
CRuiBigBall::CRuiBigBall(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiBigBall::CRuiBigBall(const CRuiBigBall & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiBigBall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiBigBall::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(RUIBIGBALLINFO));

	_vector vLook = m_ShootInfo.pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLook.m128_f32[1] += 1.5f;

	_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vPos.m128_f32[1] += 1.5f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->LookAt(vLook);

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_JUMPBALL_MAIN, this);
	RELEASE_INSTANCE(CEffect_Manager);

	return S_OK;
}

void CRuiBigBall::Tick(_float fTimeDelta)
{
	Set_EffectTime(fTimeDelta);
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_fDeadTime += fTimeDelta;
	//if (m_fDeadTime > 3.9f)
	//	m_pEffect->Set_Dead();
	if (m_fDeadTime > 4.f)
		Set_Dead();
}

void CRuiBigBall::Late_Tick(_float fTimeDelta)
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

		if (m_ShootInfo.pTarget->Get_PlayerInfo().bGuard && m_ShootInfo.pTarget->Get_PlayerInfo().iGuard > 0)
		{
			m_ShootInfo.pTarget->Get_GuardHit(0);
			m_ShootInfo.pTarget->Set_GuardHp(_int(-30 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
			if (m_ShootInfo.pTarget->Get_PlayerInfo().iGuard <= 0)
			{
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_ShootInfo.pTarget);
				RELEASE_INSTANCE(CEffect_Manager);
				m_ShootInfo.pTarget->Set_ResetGuardHp();
				m_ShootInfo.pTarget->Set_GuardTime(2.f);
			}
		}
		else
		{
			m_ShootInfo.pTarget->Set_Hp(_int(-60 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
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

HRESULT CRuiBigBall::Render()
{
	return S_OK;
}

HRESULT CRuiBigBall::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiBigBall::Ready_Components()
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
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CRuiBigBall * CRuiBigBall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiBigBall*	pInstance = new CRuiBigBall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiBigBall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiBigBall::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiBigBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiBigBall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiBigBall::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}
