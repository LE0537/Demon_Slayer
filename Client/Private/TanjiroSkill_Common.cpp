#include "stdafx.h"
#include "TanjiroSkill_Common.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CSkill_CommonState::CSkill_CommonState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_WindMill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CTanjiroState * CSkill_CommonState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CSkill_CommonState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CSkill_CommonState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);
	//pTanjiro->Get_Transform()->LookAt(vLooAt);
	_int iHit = pTanjiro->Get_SkillHit();
	m_fDelay += fTimeDelta;
	if (m_fDelay > 0.3f)
	{
		m_fTime += fTimeDelta;
		m_fHitTime += fTimeDelta;
	}
	if (m_fTime < 0.35f)
	{
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pTanjiro->Get_NavigationCom());
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
		if (m_eStateType != Client::CTanjiroState::TYPE_START)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
																							 //m_pCollBox->Get_Transform()->LookAt(vLooAt);
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(vLooAt);
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가

			if (m_fHitTime > 0.07f && iHit < 5)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_float4 vTagetPos;
					XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
					_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					vPos.m128_f32[1] = 0.f;
					m_pTarget->Get_Transform()->LookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard)
					{
						m_pTarget->Get_GuardHit(0);
					}
					else
					{
						m_pTarget->Set_Hp(-15);
						m_pTarget->Take_Damage(0.1f, false);
					}

					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

					RELEASE_INSTANCE(CEffect_Manager);

					pTanjiro->Set_SkillHit();
					m_fHitTime = 0.f;
				}
			}
		}

		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pTanjiro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;

			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_CommonState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_COMMON);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_COMMON);

}

void CSkill_CommonState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead();
	pTanjiro->Reset_SkillHit();
}

