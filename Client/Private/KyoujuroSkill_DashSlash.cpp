#include "stdafx.h"
#include "KyoujuroSkill_DashSlash.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CSkill_DashSlashState::CSkill_DashSlashState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;
	
	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CSkill_DashSlashState::HandleInput(CKyoujuro * pKyojuro)
{

	return nullptr;
}

CKyoujuroState * CSkill_DashSlashState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	if (pKyojuro->Get_Model()->Get_End(pKyojuro->Get_AnimIndex()))
	{
		pKyojuro->Get_Model()->Set_End(pKyojuro->Get_AnimIndex());
		return new CIdleState();
	}


	return nullptr;
}

CKyoujuroState * CSkill_DashSlashState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pKyojuro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pKyojuro->Get_Transform()->Set_PlayerLookAt(vLooAt);

	m_fTime += fTimeDelta;

	if (m_fTime < 0.7f)
	{
		pKyojuro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 2.f);

		if (!m_bHit && m_fTime > 0.2f)
		{
			_vector vCollPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(vLooAt);
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float4 vTagetPos;
				XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = 0.f;
				m_pTarget->Get_Transform()->LookAt(vPos);
				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-25);
					m_pTarget->Take_Damage(0.2f, false);
				}
				_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
			}
		}


		CCollider*	pMyCollider = pKyojuro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pKyojuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 2.f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		}
	}
	else if (m_fTime >= 0.7f)
	{
		m_fHitTime += fTimeDelta;
		if (m_iHit < 3 && m_fHitTime > 0.1f)
		{
			_vector vCollPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(vLooAt);
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_float4 vTagetPos;
				XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vPos.m128_f32[1] = 0.f;
				m_pTarget->Get_Transform()->LookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-25);
					m_pTarget->Take_Damage(0.25f, false);
				}

				_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);
				m_fHitTime = 0.f;
				++m_iHit;
			}
		}
		CCollider*	pMyCollider = pKyojuro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pKyojuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_DashSlashState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_DASHSLASH;

	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_DASHSLASH);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_DASHSLASH);

}

void CSkill_DashSlashState::Exit(CKyoujuro * pKyojuro)
{
	pKyojuro->Get_Model()->Reset_Anim(CKyoujuro::ANIMID::ANIM_SKILL_DASHSLASH);
	m_pCollBox->Set_Dead();
}

