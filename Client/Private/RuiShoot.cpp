#include "stdafx.h"
#include "..\Public\RuiShoot.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
CRuiShoot::CRuiShoot(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiShoot::CRuiShoot(const CRuiShoot & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiShoot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiShoot::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(RUISHOOTINFO));

	_vector vLook = m_ShootInfo.pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook2 = vLook;
	vLook.m128_f32[1] = 0.f;
	
	_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPos2 = vPos;
	vPos.m128_f32[1] = 0.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->LookAt(vLook);

	switch (m_ShootInfo.iIndex)
	{
	case 1:
		m_pTransformCom->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(15.f));
		break;
	case 2:
		m_pTransformCom->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-15.f));
		break;
	case 3:
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos2);
		m_pTransformCom->LookAt(vLook2);
		break;
	case 4:
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos2);
		m_pTransformCom->LookAt(vLook2);
		m_pTransformCom->Turn2(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(15.f));
		break;
	case 5:
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos2);
		m_pTransformCom->LookAt(vLook2);
		m_pTransformCom->Turn2(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-15.f));
		break;
	default:
		break;
	}

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_SHOTNET_MAIN, this);
	RELEASE_INSTANCE(CEffect_Manager);

	return S_OK;
}

void CRuiShoot::Tick(_float fTimeDelta)
{
	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_fDeadTime += fTimeDelta;

	m_fEffectCreateTime += fTimeDelta;
	if (0.1f < m_fEffectCreateTime)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		//pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_SHOTNET_COLEVERYTIME, this);
		RELEASE_INSTANCE(CEffect_Manager);

		m_fEffectCreateTime = 0.f;
	}

	//if (m_fDeadTime > 3.9f)
	//	m_pEffect->Set_Dead();
	if (m_fDeadTime > 4.f)
		Set_Dead();
}

void CRuiShoot::Late_Tick(_float fTimeDelta)
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
		vPos.m128_f32[1] = 0.f;
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
			m_ShootInfo.pTarget->Set_Hp(_int(-30 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
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

HRESULT CRuiShoot::Render()
{
	return S_OK;
}

HRESULT CRuiShoot::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiShoot::Ready_Components()
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
	ColliderDesc.vScale = _float3(2.f, 10.f, 2.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CRuiShoot * CRuiShoot::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiShoot*	pInstance = new CRuiShoot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiShoot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiShoot::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiShoot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiShoot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiShoot::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}
