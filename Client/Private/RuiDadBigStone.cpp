#include "stdafx.h"
#include "..\Public\RuiDadBigStone.h"

#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiDad.h"
#include "RuiDadSmallStone.h"
CRuiDadBigStone::CRuiDadBigStone(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollBox(pDevice, pContext)
{
}

CRuiDadBigStone::CRuiDadBigStone(const CRuiDadBigStone & rhs)
	: CCollBox(rhs)
{
}

HRESULT CRuiDadBigStone::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiDadBigStone::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ShootInfo, pArg, sizeof(RUIDADBIGSTONE));


	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_THROWING_BIGSTONE,this );
	
	RELEASE_INSTANCE(CEffect_Manager);

	
	return S_OK;
}

void CRuiDadBigStone::Tick(_float fTimeDelta)
{
	if (!m_bThrow)
	{
		m_fCurrentDuration = dynamic_cast<CRuiDad*>(m_ShootInfo.pPlayer)->Get_Model()->Get_CurrentTime_Index(10);

		if (m_fCurrentDuration < 75.f)
			Ready_Throw(fTimeDelta);
		else if (m_fCurrentDuration >= 75.f)
			m_bThrow = true;
	}
	if(m_bThrow)
		Throw(fTimeDelta);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CRuiDadBigStone::Late_Tick(_float fTimeDelta)
{
	
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CRuiDadBigStone::Render()
{
	return S_OK;
}

HRESULT CRuiDadBigStone::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CRuiDadBigStone::Ready_Components()
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

void CRuiDadBigStone::Ready_Throw(_float fTimeDelta)
{
	CHierarchyNode*		pBody = dynamic_cast<CRuiDad*>(m_ShootInfo.pPlayer)->Get_Model()->Get_BonePtr("C_Spine_3");
	CHierarchyNode*		pRightHand = dynamic_cast<CRuiDad*>(m_ShootInfo.pPlayer)->Get_Model()->Get_BonePtr("R_Hand_1_Lct");
	CHierarchyNode*		pLeftHand = dynamic_cast<CRuiDad*>(m_ShootInfo.pPlayer)->Get_Model()->Get_BonePtr("L_Hand_1_Lct");

	_matrix matPivot = XMLoadFloat4x4(&dynamic_cast<CRuiDad*>(m_ShootInfo.pPlayer)->Get_Model()->Get_PivotFloat4x4());
	_matrix matWorld = XMLoadFloat4x4(m_ShootInfo.pPlayer->Get_Transform()->Get_World4x4Ptr());
	_matrix matBody = pBody->Get_CombinedTransformationMatrix() * matPivot * matWorld;
	_matrix matRightHand = pRightHand->Get_CombinedTransformationMatrix() * matPivot * matWorld;
	_matrix matLeftHand = pLeftHand->Get_CombinedTransformationMatrix() * matPivot * matWorld;

	_vector BodyPos = matBody.r[3];
	_vector RightHandPos = matRightHand.r[3];
	_vector LeftHandPos = matLeftHand.r[3];

	_vector vPos = RightHandPos;
	_vector vLook = RightHandPos - LeftHandPos;

	_float fDist = XMVectorGetX(XMVector3Length(vLook));

	vPos -= XMVector3Normalize(vLook) * (fDist * 0.5f);

	_vector vPosLook = XMVector3Normalize(vPos - BodyPos);

	vPos += vPosLook * 3.f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

void CRuiDadBigStone::Throw(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPos = m_ShootInfo.pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook = vTargetPos - vPos;
	if (!m_bLook)
	{
		XMStoreFloat4(&m_vTargetLook, XMVector3Normalize(vLook));
		m_bLook = true;
	}

	vPos += XMLoadFloat4(&m_vTargetLook) * 35.f * fTimeDelta;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

//	_float fDist = XMVectorGetX(XMVector3Length(vLook));
	if (vPos.m128_f32[1] < 0.1f)
	{
		m_pEffect->Set_Dead();
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_THROWING_GROUND3, this);

		RELEASE_INSTANCE(CEffect_Manager);


		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		CRuiDadSmallStone::RUIDADSMALLSTONE	tInfo;
		tInfo.pPlayer = m_ShootInfo.pPlayer;
		tInfo.pTarget = m_ShootInfo.pTarget;
		XMStoreFloat4(&tInfo.vPos,m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
		for (_int i = 0; i < 4; ++i)
		{
			tInfo.iIndex = i;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RuiDadSmallStone"), LEVEL_GAMEPLAY, TEXT("Layer_CollBox"), &tInfo)))
				return;
		}
		RELEASE_INSTANCE(CGameInstance);
		Set_Dead();
	}

	CCollider*	pMyCollider = m_pOBBCom;
	CCollider*	pTargetCollider = m_ShootInfo.pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider) && !m_bHit)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		
		m_ShootInfo.pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

		if (m_ShootInfo.pTarget->Get_PlayerInfo().bGuard && m_ShootInfo.pTarget->Get_PlayerInfo().iGuard > 0)
		{
			m_ShootInfo.pTarget->Get_GuardHit(0);
			m_ShootInfo.pTarget->Set_GuardHp(_int(-100 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
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
			m_ShootInfo.pTarget->Set_Hp(_int(-70 * m_ShootInfo.pPlayer->Get_PlayerInfo().fPowerUp));
			m_ShootInfo.pTarget->Take_Damage(0.7f, false);
			m_ShootInfo.pTarget->Get_BattleTarget()->Set_Combo(1);
			m_ShootInfo.pTarget->Get_BattleTarget()->Set_ComboTime(0.f);
		}

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_ShootInfo.pTarget);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bHit = true;
	
	}


}

void CRuiDadBigStone::Throw_End(_float fTimeDelta)
{
}



CRuiDadBigStone * CRuiDadBigStone::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiDadBigStone*	pInstance = new CRuiDadBigStone(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiDadBigStone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRuiDadBigStone::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiDadBigStone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiDadBigStone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiDadBigStone::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOBBCom);
}
