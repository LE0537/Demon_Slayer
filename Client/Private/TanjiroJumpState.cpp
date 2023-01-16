#include "stdafx.h"
#include "TanjiroJumpstate.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;

CJumpstate::CJumpstate(STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CTanjiroState * CJumpstate::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CJumpstate::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_START);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_START);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_END);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);


	if(m_eStateType == TYPE_START)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 3.f);
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.05f;

	if(m_eStateType != TYPE_DEFAULT)
		Jump(pTanjiro, fTimeDelta + m_fJumpTime);
	
	return nullptr;
}

CTanjiroState * CJumpstate::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // �ִϸ��̼� �� �ð�
	_float fCurrentTime = 0.f; // �ִϸ��̼� ���� �ð�

	fDurationTime = pTanjiro->Get_Model()->Get_Duration();
	fCurrentTime = pTanjiro->Get_Model()->Get_CurrentTime();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			printf_s("Start Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CJumpstate(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			printf_s("Loop Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CJumpstate(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_END:
			printf_s("End jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			printf_s("Default Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}


	return nullptr;
}

void CJumpstate::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = CTanjiroState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_START, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_START);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_START, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_END, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
		break;
	}


}

void CJumpstate::Exit(CTanjiro * pTanjiro)
{
}


CTanjiroState* CJumpstate::Jump(CTanjiro* pTanjiro, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 15.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_eStateType = CTanjiroState::TYPE_DEFAULT;
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
	}

	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}


