#include "stdafx.h"
#include "TanjiroSkill_WindMill.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
#include "TanjiroDashState.h"
using namespace Tanjiro;


CSkill_WindMillState::CSkill_WindMillState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_WindMill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fHitTime = 0.13;
}

CTanjiroState * CSkill_WindMillState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CTanjiroState * CSkill_WindMillState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());


	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		return new CIdleState();
	}
	

	return nullptr;
}

CTanjiroState * CSkill_WindMillState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//vLooAt.m128_f32[1] = m_fCurrentPosY;
	//pTanjiro->Get_Transform()->LookAt(vLooAt);
	pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);
	_int iHit = pTanjiro->Get_WindMillHit();
	m_fTime += fTimeDelta;
	m_fHitTime += fTimeDelta;
	if (m_fTime < 0.4f)
	{
		pTanjiro->Get_Transform()->Go_StraightNoNavi(fTimeDelta * 0.5f);
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
		
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			
		//	m_pCollBox->Get_Transform()->LookAt(vLooAt);
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(vLooAt);
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가

			if (m_fHitTime > 0.13f && iHit < 3)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_float4 vTagetPos;
					XMStoreFloat4(&vTagetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
					_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard)
					{
						m_pTarget->Get_GuardHit(0);
					}
					else
					{
						m_pTarget->Set_Hp(-30);
						if (!m_bHit)
						{
							m_pTarget->Take_Damage(0.3f,true);
							m_bHit = true;
						}
					}

					_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();

					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, vTagetWorld);

					RELEASE_INSTANCE(CEffect_Manager);

					pTanjiro->Set_WindMillHit();
					m_fHitTime = 0.f;
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
	if (!m_bJump)
	{
		m_fJumpTime += 0.035f;
		Jump(pTanjiro, m_fJumpTime);
	}
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_WindMillState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_WINDMILL;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_WINDMILL);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_SKILL_WINDMILL, 0.2f);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_WINDMILL);

}
CTanjiroState* CSkill_WindMillState::Jump(CTanjiro* pTanjiro, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 12.5f;
	static _float fGravity = 15.f;


	_vector      vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}


	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CSkill_WindMillState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead();
	pTanjiro->Reset_WindMillHit();
}

