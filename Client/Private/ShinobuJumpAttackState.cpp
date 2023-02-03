#include "stdafx.h"
#include "ShinobuJumpAttackState.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"
#include "ShinobuMoveState.h"
using namespace Shinobu;

CJumpAttackState::CJumpAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
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

