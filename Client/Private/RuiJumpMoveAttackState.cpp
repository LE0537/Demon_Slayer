#include "stdafx.h"
#include "RuiJumpMoveAttackState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"
#include "RuiMoveState.h"
#include "Effect_Manager.h"
using namespace Rui;

CJumpMoveAttackState::CJumpMoveAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiState * CJumpMoveAttackState::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CJumpMoveAttackState::Tick(CRui* pRui, _float fTimeDelta)
{

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START: // 공격 모션
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CJumpMoveAttackState(TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP: // 떨어지는 모션
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());;
			break;
		case Client::CRuiState::TYPE_END: // 착지 모션
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_LOOP:
		Jump(pRui, fTimeDelta);

		if (m_bNextAnim == true)
			return new CJumpMoveAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CRuiState * CJumpMoveAttackState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	if (m_eStateType == CRuiState::TYPE_END)
	{
		CCharacters* m_pTarget = pRui->Get_BattleTarget();


		_vector vCollPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pRui->Get_SphereCollider();
		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					m_pTarget->Set_Hp(-pRui->Get_PlayerInfo().iDmg);
					m_pTarget->Take_Damage(0.5f, false);
					pRui->Set_Combo(1);
					pRui->Set_ComboTime(1.f);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
			}

		}


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pRui->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pRui->Get_NavigationCom()->Cheak_Cell(vPos))
				pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pRui->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRui->Get_NavigationCom());
		}

	}

	if (m_eStateType == TYPE_END)
		pRui->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pRui->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpMoveAttackState::Enter(CRui* pRui)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_MOVE_ATTACK_0);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_0, 0.01f);
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_1);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_MOVE_ATTACK_1);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, true);
		Initialize_value(pRui);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_MOVE_ATTACK_2);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, false);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_2, 0.01f);
		break;
	}
}

void CJumpMoveAttackState::Exit(CRui* pRui)
{
	m_pCollBox->Set_Dead();
}

void CJumpMoveAttackState::Jump(CRui* pRui, _float fTimeDelta)
{
	static _float fGravity = 100.f;
	static _float fVelocity = 0.f;
	

	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;
	 


	if (m_bRange == true)
	{
		//m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition);
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition) * 5.f;
		//m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition);
	}
	else if (m_bRange == false)
	{
		m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition)* 3.f;
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition)* 5.f;
		m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition)* 3.f;
	}


	_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetX(vecPos, m_vPosition.x);
	vecPos = XMVectorSetY(vecPos, m_vPosition.y);
	vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, m_vPosition.x);
		vecPos = XMVectorSetY(vecPos, m_vPosition.y);
		vecPos = XMVectorSetZ(vecPos, m_vPosition.z);
		if (pRui->Get_NavigationCom()->Cheak_Cell(vecPos))
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		if (pRui->Get_NavigationCom()->Cheak_Cell(vecPos))
			pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveAttackState::Initialize_value(CRui* pRui)
{
	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRui->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

