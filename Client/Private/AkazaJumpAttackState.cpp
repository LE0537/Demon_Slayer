#include "stdafx.h"
#include "AkazaJumpAttackState.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"
#include "AkazaMoveState.h"
using namespace Akaza;

CJumpAttackState::CJumpAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CAkazaState * CJumpAttackState::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CJumpAttackState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START: // 공격 모션
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CJumpAttackState(TYPE_LOOP);
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
		return new CJumpAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CAkazaState * CJumpAttackState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpAttackState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMPATTACK);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMPATTACK);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMPATTACK, false);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_LOOP_END);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_LOOP_END);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_LOOP_END, true);
		Initialize_value(pAkaza);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_END);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_END);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_LOOP_END, false);
		break;
	}
}

void CJumpAttackState::Exit(CAkaza* pAkaza)
{
}

void CJumpAttackState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f; 
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{ 
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpAttackState::Initialize_value(CAkaza* pAkaza)
{
	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pAkaza->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}

