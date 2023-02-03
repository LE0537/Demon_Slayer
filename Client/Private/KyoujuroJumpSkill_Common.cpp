#include "stdafx.h"
#include "KyoujuroJumpSkill_Common.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CJumpSkill_CommonState::CJumpSkill_CommonState(_float fPositionY)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_KyoujuroJumpSkill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;


	m_fPositionY = fPositionY;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CJumpSkill_CommonState::HandleInput(CKyoujuro * pKyojuro)
{

	return nullptr;
}

CKyoujuroState * CJumpSkill_CommonState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	
	pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	if (m_bOnGround == false)
		Jump(pKyojuro, fTimeDelta);

	pKyojuro->Set_JumpState(true);

	return nullptr;
}

CKyoujuroState * CJumpSkill_CommonState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	CCharacters* m_pTarget = pKyojuro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pKyojuro->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;
	
	if (m_fMove  > 0.4f)
	{
		m_fDelay += fTimeDelta;
		_vector vCollPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pKyojuro->Get_SphereCollider();
		if (m_fDelay > 0.15f && m_iHit < 6)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					if (m_iHit > 4)
					{
						m_pTarget->Set_Hp(-20);
						m_pTarget->Take_Damage(0.7f, true);
					}
					else
					{
						m_pTarget->Set_Hp(-10);
						m_pTarget->Take_Damage(0.2f, false);
					}
				}
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);

				++m_iHit;
			}

		}


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pKyojuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pKyojuro->Get_NavigationCom()->Cheak_Cell(vPos))
				pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pKyojuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyojuro->Get_NavigationCom());
		}
	}


	if (m_bOnGround == true)
	{
		m_pCollBox->Set_Dead();
		pKyojuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_SKILL_JUMPCOMMON, true);
		return new CIdleState();
	}


	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_JUMP_5TIGER_CHARGE, pKyojuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_JUMP_5TIGER_MAIN, pKyojuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_JUMP_5TIGER_AFTER, pKyojuro);
		//pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_JUMP_5TIGER_END, pKyojuro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}


	return nullptr;
}

void CJumpSkill_CommonState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_JUMPCOMMON;

	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_JUMPCOMMON);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_JUMPCOMMON);
	pKyojuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_SKILL_JUMPCOMMON);
	pKyojuro->Get_Model()->Set_FrameNum(CKyoujuro::ANIM_SKILL_JUMPCOMMON, 100);


	m_vPosition.x = XMVectorGetX(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.x = 0.f;
	m_vVelocity.y = 10.f;
	m_vVelocity.z = 0.f;


}

void CJumpSkill_CommonState::Exit(CKyoujuro * pKyojuro)
{
	m_pCollBox->Set_Dead();
}

CKyoujuroState* CJumpSkill_CommonState::Jump(CKyoujuro* pKyojuro, _float fTimeDelta)
{
	static _float fJump_Velocity = 10.f;
	static _float fGravity = -50.8f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;
	
	_vector vCurrentPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) < m_fPositionY)
	{
		_vector vPosition;
		m_vPosition.y = m_fPositionY;
		m_vVelocity.y = m_fPositionY;
		m_bOnGround = true;

		vPosition =  XMVectorSet(m_vPosition.x, m_vPosition.y ,m_vPosition.z, 1.f);
		pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
		pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION,vPosition);

	return nullptr;
}

