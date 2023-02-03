#include "stdafx.h"
#include "AkazaJumpMoveAttackState.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"
#include "AkazaMoveState.h"
using namespace Akaza;

CJumpMoveAttackState::CJumpMoveAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CAkazaState * CJumpMoveAttackState::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CJumpMoveAttackState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START: // 공격 모션
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CJumpMoveAttackState(TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP: // 떨어지는 모션
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());;
			break;
		case Client::CAkazaState::TYPE_END: // 착지 모션
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_LOOP:
		Jump(pAkaza, fTimeDelta);

		if (m_bNextAnim == true)
			return new CJumpMoveAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CAkazaState * CJumpMoveAttackState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpMoveAttackState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_MOVE_ATTACK_0);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_0, 0.01f);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_MOVE_ATTACK_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, true);
		Initialize_value(pAkaza);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_MOVE_ATTACK_2);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, false);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_JUMP_MOVE_ATTACK_2, 0.01f);
		break;
	}
}

void CJumpMoveAttackState::Exit(CAkaza* pAkaza)
{
}

void CJumpMoveAttackState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fGravity = 100.f;
	static _float fVelocity = 0.f;
	

	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

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


	_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetX(vecPos, m_vPosition.x);
	vecPos = XMVectorSetY(vecPos, m_vPosition.y);
	vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, m_vPosition.x);
		vecPos = XMVectorSetY(vecPos, m_vPosition.y);
		vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveAttackState::Initialize_value(CAkaza* pAkaza)
{
	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pAkaza->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

