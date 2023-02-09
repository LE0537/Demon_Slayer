#include "stdafx.h"
#include "TanjiroKaguraJumpSkill_Move.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CKaguraJumpSkill_MoveState::CKaguraJumpSkill_MoveState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CTanjiroState * CKaguraJumpSkill_MoveState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CKaguraJumpSkill_MoveState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		if (m_bNextAnim == true)
			return new CIdleState();
	}

	if (pTanjiro->Get_Model()->Get_CurrentTime() <= 53.f)
		Move(pTanjiro, fTimeDelta);
	else
		Jump(pTanjiro, fTimeDelta);




	return nullptr;
}

CTanjiroState * CKaguraJumpSkill_MoveState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	if (!m_bLook)
	{
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vLook, vLooAt);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);
		m_bLook = true;
	}
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
	m_fMove += fTimeDelta;


	if (m_fMove <= 0.2f && m_iMove < 5)
	{
		m_fDelay = 0.f;
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
	}
	else if (m_fMove > 0.4f && m_iMove < 5)
	{
		m_fMove = 0.f;
		++m_iMove;
	}
	if (m_iMove < 4 && m_fMove > 0.2f && m_fDelay < 0.1f)
	{
		_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
	//	vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가


		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

			if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
			{
				m_pTarget->Get_GuardHit(0);
				m_pTarget->Set_GuardHp(-50);
				if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
				{
					m_pTarget->Set_ResetGuardHp();
					m_pTarget->Set_GuardTime(2.f);
				}
			}
			else
			{
				m_pTarget->Set_Hp(-20);
				m_pTarget->Take_Damage(0.3f, false);
				pTanjiro->Set_Combo(1);
				pTanjiro->Set_ComboTime(0.f);
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
			m_fDelay = 1.f;
		}
	}
	else if (m_iMove >= 4 && m_fMove > 0.3f && m_fDelay < 0.1f)
	{
		_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
	//	vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가


		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

			if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
			{
				m_pTarget->Get_GuardHit(0);
				m_pTarget->Set_GuardHp(-50);
				if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
				{
					m_pTarget->Set_ResetGuardHp();
					m_pTarget->Set_GuardTime(2.f);
				}
			}
			else
			{
				m_pTarget->Set_Hp(-20);
				m_pTarget->Take_Damage(0.6f, false);
				pTanjiro->Set_Combo(1);
				pTanjiro->Set_ComboTime(0.f);
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

			m_fDelay = 1.f;
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
		vPos.m128_f32[1] = 0.f;
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		else
			pTanjiro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pTanjiro->Get_NavigationCom());
	}

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CKaguraJumpSkill_MoveState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_JUMP;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_MOVE);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_KAGURA_MOVE);
	Initialize_value(pTanjiro);
}

void CKaguraJumpSkill_MoveState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead();
	pTanjiro->Reset_SkillHit();
}

void CKaguraJumpSkill_MoveState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vTranslationTargetPos = XMVectorSetY(vTargetPosition, XMVectorGetY(vMyPosition));
	_float fDistance = XMVectorGetX(XMVector3Length(vTranslationTargetPos - vMyPosition));
	_vector vDirection = XMVector3Normalize(vTranslationTargetPos - vMyPosition);

	static _float fGravity = 9.8f;

	m_vVelocity.x += fGravity * fTimeDelta;
	//m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(vDirection) *   m_vVelocity.x  * fTimeDelta;
	m_vPosition.z += XMVectorGetZ(vDirection) *   m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (fDistance <= 3.f)
	{
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
	}
	else
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CKaguraJumpSkill_MoveState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	pTanjiro->Get_Transform()->Set_Jump(true);

	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));



	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		pTanjiro->Get_Transform()->Set_Jump(false);
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

			m_bNextAnim = true;
		}
	}
	else
	{
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}

}

void CKaguraJumpSkill_MoveState::Initialize_value(CTanjiro * pTanjiro)
{
	m_vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pShinobu->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
;
}

