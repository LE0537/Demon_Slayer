#include "stdafx.h"
#include "..\Public\RuiSphere.h"

#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
CRuiSphere::CRuiSphere(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiSphere::CRuiSphere(const CRuiSphere & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiSphere::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiSphere::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(RUISPHEREINFO));
	
	_vector vPos;

	switch (m_ShootInfo.iIndex)
	{
	case 0:
		vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vPos.m128_f32[1] = 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
		break;
	case 1:
		vPos = m_ShootInfo.pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vPos.m128_f32[1] = 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
		break;

	default:
		break;
	}
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_FRIENDCOM_START, this);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_FRIENDCOM_MAIN, this);
		RELEASE_INSTANCE(CEffect_Manager);
		
	
	
	return S_OK; 
}

void CRuiSphere::Tick(_float fTimeDelta)
{

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fDeadTime += fTimeDelta;
	if (m_fDeadTime > 5.f)
		Set_Dead();
}

void CRuiSphere::Late_Tick(_float fTimeDelta)
{
	m_fMove += fTimeDelta;

	if (m_fMove > 0.6f)
		m_fDelay += fTimeDelta;
	if (m_fDelay > 0.1f && m_iHit < 4)
	{

		CCollider*	pMyCollider = m_pOBBCom;
		CCollider*	pTargetCollider = m_ShootInfo.pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
				CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.1f);
				RELEASE_INSTANCE(CGameInstance);
				m_ShootInfo.pTarget->Set_Hp(_int(-15 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
				m_ShootInfo.pTarget->Take_Damage(0.1f, false);
				m_ShootInfo.pPlayer->Set_Combo(1);
				m_ShootInfo.pPlayer->Set_ComboTime(1.f);
			}

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_ShootInfo.pTarget);

			RELEASE_INSTANCE(CEffect_Manager);
			m_fDelay = 0.f;
			++m_iHit;
		}

	}
	if (m_fMove > 1.1f && !m_bHit && m_fMove < 1.3f)
	{

		CCollider*	pMyCollider = m_pOBBCom;
		CCollider*	pTargetCollider = m_ShootInfo.pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
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
				CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
				dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_DOWN, 0.2f);
				RELEASE_INSTANCE(CGameInstance);
				m_ShootInfo.pTarget->Set_Hp(_int(-30 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
				m_ShootInfo.pTarget->Player_UpperDown(CCharacters::HIT_TYPE::HIT_BOUND, 15.f, 20.f, 5.f);
				m_ShootInfo.pPlayer->Set_Combo(1);
				m_ShootInfo.pPlayer->Set_ComboTime(1.f);
			}

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_ShootInfo.pTarget);

			RELEASE_INSTANCE(CEffect_Manager);

			m_bHit = true;
		}

	}

	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CRuiSphere::Render()
{
	return S_OK;
}

HRESULT CRuiSphere::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiSphere::Ready_Components()
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
	ColliderDesc.vScale = _float3(15.f, 10.f, 15.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CRuiSphere * CRuiSphere::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiSphere*	pInstance = new CRuiSphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiSphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiSphere::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiSphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiSphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiSphere::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}

