#include "stdafx.h"
#include "KyoujuroJumpMoveAttackState.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroMoveState.h"
using namespace Kyoujuro;

CJumpMoveAttackState::CJumpMoveAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CKyoujuroState * CJumpMoveAttackState::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CJumpMoveAttackState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START: // 공격 모션
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CJumpMoveAttackState(TYPE_LOOP);
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
			return new CJumpMoveAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CKyoujuroState * CJumpMoveAttackState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpMoveAttackState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_MOVE_ATTACK_0);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_0, 0.01f);
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_1);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_MOVE_ATTACK_1);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, true);
		Initialize_value(pKyoujuro);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_JUMP_MOVE_ATTACK_2);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, false);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_JUMP_MOVE_ATTACK_2, 0.01f);
		break;
	}
}

void CJumpMoveAttackState::Exit(CKyoujuro* pKyoujuro)
{
}

void CJumpMoveAttackState::Jump(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	static _float fGravity = 100.f;
	static _float fVelocity = 0.f;
	

	m_vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

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


	_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetX(vecPos, m_vPosition.x);
	vecPos = XMVectorSetY(vecPos, m_vPosition.y);
	vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, m_vPosition.x);
		vecPos = XMVectorSetY(vecPos, m_vPosition.y);
		vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveAttackState::Initialize_value(CKyoujuro* pKyoujuro)
{
	m_vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pKyoujuro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

