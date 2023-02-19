#include "stdafx.h"
#include "KyoujuroJumpAttackState.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroMoveState.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;

CJumpAttackState::CJumpAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CJumpAttackState::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CJumpAttackState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START: // 공격 모션
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CJumpAttackState(TYPE_LOOP);
			break;
		case Client::CKyoujuroState::TYPE_LOOP: // 떨어지는 모션
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());;
			break;
		case Client::CKyoujuroState::TYPE_END: // 착지 모션
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_LOOP:
		Jump(pKyoujuro, fTimeDelta);

	if (m_bNextAnim == true)
		return new CJumpAttackState(TYPE_END);
		break;
	}


	return nullptr;
}

CKyoujuroState * CJumpAttackState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	if (m_eStateType == CKyoujuroState::TYPE_START)
	{
		CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();


		_vector vCollPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] += 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pKyoujuro->Get_SphereCollider();
		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-30 * pKyoujuro->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					m_pTarget->Set_Hp(_int(-pKyoujuro->Get_PlayerInfo().iDmg * pKyoujuro->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.5f, true);
					pKyoujuro->Set_Combo(1);
					pKyoujuro->Set_ComboTime(0.f);
				}

				_int iDest = rand() % 5;
				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
				switch (iDest)
				{
				case 0:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
					break;
				case 1:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTarget);
					break;
				case 2:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTarget);
					break;
				case 3:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTarget);
					break;
				case 4:
					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTarget);
					break;
				default:
					break;
				}


				RELEASE_INSTANCE(CEffect_Manager);
				m_bHit = true;
			}

		}


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pKyoujuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pKyoujuro->Get_NavigationCom()->Cheak_Cell(vPos))
				pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyoujuro->Get_NavigationCom());
		}

	}
	if (m_eStateType == TYPE_END)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);


	if (!m_bEffect && m_eStateType == TYPE_START)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_JUMPATK_MAIN, pKyoujuro);
		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}


	return nullptr;
}

void CJumpAttackState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_JUMPATTACK);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMPATTACK);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_JUMPATTACK, false);
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_JUMP_LOOP_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_LOOP_END);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_JUMP_LOOP_END, true);
		Initialize_value(pKyoujuro);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_JUMP_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_END);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_JUMP_LOOP_END, false);
		break;
	}
}

void CJumpAttackState::Exit(CKyoujuro* pKyoujuro)
{
	m_pCollBox->Set_Dead();
}

void CJumpAttackState::Jump(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pKyoujuro->Get_NavigationHeight().y;
	pKyoujuro->Get_Transform()->Set_Jump(true);

	static _float fGravity = -100.f;
	static _float fVelocity = 0.f; 
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{ 
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;
		pKyoujuro->Get_Transform()->Set_Jump(false);
		_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;

		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pKyoujuro);

		RELEASE_INSTANCE(CEffect_Manager);
	}
	else
	{
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpAttackState::Initialize_value(CKyoujuro* pKyoujuro)
{
	m_vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pKyoujuro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}

