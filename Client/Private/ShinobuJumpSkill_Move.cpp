#include "stdafx.h"
#include "ShinobuJumpSkill_Move.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"
#include "ShinobuMoveState.h"
using namespace Shinobu;

CJumpMoveSkillState::CJumpMoveSkillState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CShinobuState * CJumpMoveSkillState::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CJumpMoveSkillState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CJumpMoveSkillState(TYPE_LOOP);
			break;
		case Client::CShinobuState::TYPE_LOOP:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CJumpMoveSkillState(TYPE_DEFAULT);
			break;
		case Client::CShinobuState::TYPE_DEFAULT:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			
			break;
		case Client::CShinobuState::TYPE_CHANGE:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		break;
	case Client::CShinobuState::TYPE_LOOP:
		Jump(pShinobu, fTimeDelta);
		break;
	case Client::CShinobuState::TYPE_END:
		break;
	case Client::CShinobuState::TYPE_DEFAULT:
		Move(pShinobu, fTimeDelta);
		break;
	case Client::CShinobuState::TYPE_CHANGE:
		break;
	default:
		break;
	}

	if (TYPE_LOOP == m_eStateType)
	{
		if (m_bNextAnim == true)
			return new CJumpMoveSkillState(TYPE_END);
	}

	else if (TYPE_DEFAULT == m_eStateType)
	{
		if (m_bNextAnim == true)
				return new CJumpMoveSkillState(TYPE_CHANGE);
	}

	 






	return nullptr;
}

CShinobuState * CJumpMoveSkillState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	else
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpMoveSkillState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_JUMP_ATTACK;
	m_eStateId = STATE_JUMP;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_0);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_SKILL_MOVE_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_0, 0.01f);
		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_1);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_SKILL_MOVE_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_1, true);
		Initialize_value(pShinobu);
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_2);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_SKILL_MOVE_2);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_2, 0.01f);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_1);
		break;
	case Client::CShinobuState::TYPE_DEFAULT:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_3);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_SKILL_MOVE_3);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_3);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_3, true);
		break;
	case Client::CShinobuState::TYPE_CHANGE:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_4);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_JUMP_SKILL_MOVE_4);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_4);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_4, 0.01f);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_4);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_JUMP_SKILL_MOVE_3);
		break;
	default:
		break;
	}


}

void CJumpMoveSkillState::Exit(CShinobu* pShinobu)
{



}

void CJumpMoveSkillState::Jump(CShinobu* pShinobu, _float fTimeDelta)
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
		m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition)* 1.5f;
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition)* 5.f;
		m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition)* 1.5f;
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
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vecPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vecPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveSkillState::Move(CShinobu * pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));


	if (fDistance < 5.f)
	{
		//pShinobu->Get_Transform()->Go_Straight(fTimeDelta, pShinobu->Get_NavigationCom());
		m_bNextAnim = true;
	}
	else
	{
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 2.0f, pShinobu->Get_NavigationCom());
	}
}

void CJumpMoveSkillState::Initialize_value(CShinobu* pShinobu)
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

