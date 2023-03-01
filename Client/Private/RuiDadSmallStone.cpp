#include "stdafx.h"
#include "..\Public\RuiDadSmallStone.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiDad.h"
CRuiDadSmallStone::CRuiDadSmallStone(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiDadSmallStone::CRuiDadSmallStone(const CRuiDadSmallStone & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiDadSmallStone::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiDadSmallStone::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(RUIDADSMALLSTONE));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_ShootInfo.vPos));
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	_vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	switch (m_ShootInfo.iIndex)
	{
	case 0:
		vPos += vLook * 10.f;
		m_pTransformCom->LookAt(vPos);
		break;
	case 1:
		vPos -= vLook * 10.f;
		m_pTransformCom->LookAt(vPos);
		break;
	case 2:
		vPos += vRight * 10.f;
		m_pTransformCom->LookAt(vPos);
		break;
	case 3:
		vPos -= vRight * 10.f;
		m_pTransformCom->LookAt(vPos);
		break;
	default:
		break;
	}

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_THROWING_SMALLSTONE, this);

	RELEASE_INSTANCE(CEffect_Manager);

	return S_OK;
}

void CRuiDadSmallStone::Tick(_float fTimeDelta)
{
	Set_EffectTime(fTimeDelta);
	if (m_bDeadTick)
	{
		Set_Dead();
		return;
	}
	Throw(fTimeDelta);
	
	m_fJumpTime += 0.04f;
	Jump(fTimeDelta + m_fJumpTime);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CRuiDadSmallStone::Late_Tick(_float fTimeDelta)
{

	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CRuiDadSmallStone::Render()
{
	return S_OK;
}

HRESULT CRuiDadSmallStone::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiDadSmallStone::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 25.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CRuiDadSmallStone::Ready_Throw(_float fTimeDelta)
{

}

void CRuiDadSmallStone::Throw(_float fTimeDelta)
{
	CCollider*	pMyCollider = m_pOBBCom;
	CCollider*	pTargetCollider = m_ShootInfo.pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	m_pTransformCom->Go_StraightNoNavi(fTimeDelta);

	if (pMyCollider->Collision(pTargetCollider) && !m_bHit)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

		m_ShootInfo.pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

		if (m_ShootInfo.pTarget->Get_PlayerInfo().bGuard && m_ShootInfo.pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
		{
			m_ShootInfo.pTarget->Get_GuardHit(0);
			m_ShootInfo.pTarget->Set_GuardHp(_int(-25 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
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
			m_ShootInfo.pTarget->Set_Hp(_int(-20 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
			m_ShootInfo.pTarget->Take_Damage(0.35f, false);
			m_ShootInfo.pTarget->Get_BattleTarget()->Set_Combo(1);
			m_ShootInfo.pTarget->Get_BattleTarget()->Set_ComboTime(0.f);
		}

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_ShootInfo.pTarget);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bHit = true;

	}


}

void CRuiDadSmallStone::Jump(_float fTimeDelta)
{
	
	m_fCurrentPosY = 0.f;

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 25.f;
	static _float fGravity = 40.f;

	_vector      vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_pEffect->Set_Dead();
		m_bDeadTick = true;
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_THROWING_GROUND4, this);

		RELEASE_INSTANCE(CEffect_Manager);
	}
	else
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	


	return;
}



CRuiDadSmallStone * CRuiDadSmallStone::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiDadSmallStone*	pInstance = new CRuiDadSmallStone(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiDadSmallStone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiDadSmallStone::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiDadSmallStone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiDadSmallStone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiDadSmallStone::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}
