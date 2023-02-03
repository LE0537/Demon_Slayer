#include "stdafx.h"
#include "ShinobuJumpMoveAttackState.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"
#include "ShinobuMoveState.h"
using namespace Shinobu;

CJumpMoveAttackState::CJumpMoveAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CShinobuState * CJumpMoveAttackState::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CJumpMoveAttackState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START: // 공격 모션
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CJumpMoveAttackState(TYPE_LOOP);
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
			return new CJumpMoveAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CShinobuState * CJumpMoveAttackState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpMoveAttackState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_MOVE_ATTACK_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_0, 0.01f);
		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_MOVE_ATTACK_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, true);
		Initialize_value(pShinobu);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_MOVE_ATTACK_2);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, false);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_JUMP_MOVE_ATTACK_2, 0.01f);
		break;
	}
}

void CJumpMoveAttackState::Exit(CShinobu* pShinobu)
{
}

void CJumpMoveAttackState::Jump(CShinobu* pShinobu, _float fTimeDelta)
{
	static _float fGravity = 100.f;
	static _float fVelocity = 0.f;
	

	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

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


	_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetX(vecPos, m_vPosition.x);
	vecPos = XMVectorSetY(vecPos, m_vPosition.y);
	vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, m_vPosition.x);
		vecPos = XMVectorSetY(vecPos, m_vPosition.y);
		vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveAttackState::Initialize_value(CShinobu* pShinobu)
{
	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pShinobu->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

