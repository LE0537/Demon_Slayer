#include "stdafx.h"
#include "NezukoJumpAttackState.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"
#include "NezukoMoveState.h"
using namespace Nezuko;

CJumpAttackState::CJumpAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CNezukoState * CJumpAttackState::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CJumpAttackState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START: // 공격 모션
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CJumpAttackState(TYPE_LOOP);
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
		return new CJumpAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CNezukoState * CJumpAttackState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpAttackState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_JUMPATTACK);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMPATTACK);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMPATTACK, false);
		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_JUMP_LOOP_END);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_LOOP_END);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMP_LOOP_END, true);
		Initialize_value(pNezuko);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_JUMP_END);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_END);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMP_LOOP_END, false);
		break;
	}
}

void CJumpAttackState::Exit(CNezuko* pNezuko)
{
}

void CJumpAttackState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f; 
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{ 
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpAttackState::Initialize_value(CNezuko* pNezuko)
{
	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}

