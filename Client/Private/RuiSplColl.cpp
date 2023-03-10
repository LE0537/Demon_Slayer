#include "stdafx.h"
#include "..\Public\RuiSplColl.h"

#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "Rui.h"
CRuiSplColl::CRuiSplColl(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiSplColl::CRuiSplColl(const CRuiSplColl & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiSplColl::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiSplColl::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(RUISPLINFO));

	_vector vPos;


	vPos = m_ShootInfo.pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vPos.m128_f32[1] = 1.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_FRIENDCOM_START, this);
	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_COLL_FRIENDCOM_MAIN, this);
	RELEASE_INSTANCE(CEffect_Manager);



	return S_OK;
}

void CRuiSplColl::Tick(_float fTimeDelta)
{
	Set_EffectTime(fTimeDelta);
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fDeadTime += fTimeDelta;
	if (m_fDeadTime > 5.f)
		Set_Dead();
}

void CRuiSplColl::Late_Tick(_float fTimeDelta)
{
	m_fMove += fTimeDelta;

	if (m_fMove > 1.2f && m_fMove < 1.4f)
	{
		if (m_iHit == 0)
		{

			CCollider*	pMyCollider = m_pOBBCom;
			CCollider*	pTargetCollider = m_ShootInfo.pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = m_ShootInfo.pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_ShootInfo.pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_ShootInfo.pTarget->Get_PlayerInfo().bGuard && m_ShootInfo.pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
				{
					m_ShootInfo.pTarget->Get_GuardHit(0);
					m_ShootInfo.pTarget->Set_GuardHp(-1);
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
					//	m_ShootInfo.pTarget->Take_Damage(0.1f, false);
					dynamic_cast<CRui*>(m_ShootInfo.pPlayer)->Set_HitSpl(true);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_ShootInfo.pTarget);

				RELEASE_INSTANCE(CEffect_Manager);
				m_fDelay = 0.f;
				++m_iHit;
			}

		}
	}


	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CRuiSplColl::Render()
{
	return S_OK;
}

HRESULT CRuiSplColl::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiSplColl::Ready_Components()
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



CRuiSplColl * CRuiSplColl::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiSplColl*	pInstance = new CRuiSplColl(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiSplColl"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiSplColl::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiSplColl(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiSplColl"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiSplColl::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}
