#include "stdafx.h"
#include "TanjiroJumpState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;

JumpState::JumpState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * JumpState::HandleInput(CTanjiro * pTanjiro)
{


	return nullptr;
}

CTanjiroState * JumpState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	
	m_fJumpTime += fTimeDelta;

	Jump(pTanjiro , m_fJumpTime);

	return nullptr;
}

CTanjiroState * JumpState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			m_eStateType = CTanjiroState::TYPE_LOOP;
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			m_eStateType = CTanjiroState::TYPE_END;
			break;
		case Client::CTanjiroState::TYPE_END:
			m_eStateType = CTanjiroState::TYPE_DEFAULT;
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			return new CIdleState();
			break;
		default:
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}




	return nullptr;
}

void JumpState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = CTanjiroState::STATE_JUMP;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_START);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_START);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
		break;
	default:
		break;
	}

	
}

void JumpState::Exit(CTanjiro * pTanjiro)
{
}



void JumpState::Jump(CTanjiro* pTanjiro, _float fTimeDelta)
{
	/*static _float fStartHeight = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fEndHeight = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fVelocity = 3.f;
	static _float fGravity = 2.f;


	_vector      vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;


	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
	}

	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);*/
}


