#include "stdafx.h"
#include "ShinobuJumpAttackState.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"
#include "ShinobuMoveState.h"
#include "Effect_Manager.h"
using namespace Shinobu;

CJumpAttackState::CJumpAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CShinobuState * CJumpAttackState::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CJumpAttackState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START: // 공격 모션
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CJumpAttackState(TYPE_LOOP);
			break;
		case Client::CShinobuState::TYPE_LOOP: // 떨어지는 모션
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());;
			break;
		case Client::CShinobuState::TYPE_END: // 착지 모션
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_LOOP:
		Jump(pShinobu, fTimeDelta);

	if (m_bNextAnim == true)
		return new CJumpAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CShinobuState * CJumpAttackState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (m_eStateType == CShinobuState::TYPE_START)
	{
		CCharacters* m_pTarget = pShinobu->Get_BattleTarget();


		_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] += 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pShinobu->Get_SphereCollider();
		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-pShinobu->Get_PlayerInfo().iDmg);
					m_pTarget->Take_Damage(0.5f, true);
					pShinobu->Set_Combo(1);
					pShinobu->Set_ComboTime(0.f);
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
			_float fSpeed = pShinobu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPos))
				pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pShinobu->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pShinobu->Get_NavigationCom());
		}

	}
	if (m_eStateType == TYPE_END)
		pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpAttackState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMPATTACK);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMPATTACK);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMPATTACK, false);
		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_LOOP_END);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_LOOP_END);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_LOOP_END, true);
		Initialize_value(pShinobu);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_END);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_END);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_LOOP_END, false);
		break;
	}
}

void CJumpAttackState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead();
}

void CJumpAttackState::Jump(CShinobu* pShinobu, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f; 
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{ 
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpAttackState::Initialize_value(CShinobu* pShinobu)
{
	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pShinobu->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}

