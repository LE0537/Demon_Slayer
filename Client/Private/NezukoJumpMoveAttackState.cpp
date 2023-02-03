#include "stdafx.h"
#include "NezukoJumpMoveAttackState.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"
#include "NezukoMoveState.h"
using namespace Nezuko;

CJumpMoveAttackState::CJumpMoveAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CNezukoState * CJumpMoveAttackState::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CJumpMoveAttackState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START: // 공격 모션
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CJumpMoveAttackState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP: // 떨어지는 모션
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());;
			break;
		case Client::CNezukoState::TYPE_END: // 착지 모션
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_LOOP:
		Jump(pNezuko, fTimeDelta);

		if (m_bNextAnim == true)
			return new CJumpMoveAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CNezukoState * CJumpMoveAttackState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpMoveAttackState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_MOVE_ATTACK_0);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_0, 0.01f);
		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_MOVE_ATTACK_1);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, true);
		Initialize_value(pNezuko);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_MOVE_ATTACK_2);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, false);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_JUMP_MOVE_ATTACK_2, 0.01f);
		break;
	}
}

void CJumpMoveAttackState::Exit(CNezuko* pNezuko)
{
}

void CJumpMoveAttackState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	static _float fGravity = 100.f;
	static _float fVelocity = 0.f;
	

	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

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


	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetX(vecPos, m_vPosition.x);
	vecPos = XMVectorSetY(vecPos, m_vPosition.y);
	vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, m_vPosition.x);
		vecPos = XMVectorSetY(vecPos, m_vPosition.y);
		vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveAttackState::Initialize_value(CNezuko* pNezuko)
{
	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

