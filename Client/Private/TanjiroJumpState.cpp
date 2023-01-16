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
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Down(DIK_1))
		return new CIdleState();

	return nullptr;
}

CTanjiroState * JumpState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_START);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_START);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_END);
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);


	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += fTimeDelta;

	//Jump(pTanjiro, m_fJumpTime);

	return nullptr;
}

CTanjiroState * JumpState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간

	fDurationTime = pTanjiro->Get_Model()->Get_Duration();
	fCurrentTime = pTanjiro->Get_Model()->Get_CurrentTime();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if(pGameInstance->Mouse_Down(DIMK_LBUTTON))
		printf_s("state : %d \n", pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			printf_s("Start Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new JumpState(STATE_TYPE::TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			printf_s("Loop Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new JumpState(STATE_TYPE::TYPE_END);
			break;
		case Client::CTanjiroState::TYPE_END:
			printf_s("End Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new JumpState(STATE_TYPE::TYPE_DEFAULT);
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			printf_s("Default Jump \n");
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}


	//if (m_eStateType == TYPE_LOOP && pGameInstance->Mouse_Pressing(DIMK_LBUTTON))
	//{
	//	printf_s("fDurationTime : %d \n", (int)fDurationTime);
	//	printf_s("fCurrentTime : %d \n", (int)fCurrentTime);
	//}

	//switch (m_eStateType)
	//{
	//case Client::CTanjiroState::TYPE_START:
	//	return new JumpState(STATE_TYPE::TYPE_LOOP);
	//	break;
	//case Client::CTanjiroState::TYPE_LOOP:
	//	return new JumpState(STATE_TYPE::TYPE_END);
	//	break;
	//case Client::CTanjiroState::TYPE_END:
	//	return new JumpState(STATE_TYPE::TYPE_DEFAULT);
	//	break;
	//case Client::CTanjiroState::TYPE_DEFAULT:
	//	return new CIdleState();
	//	break;
	//}


	return nullptr;
}

void JumpState::Enter(CTanjiro * pTanjiro)
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

void JumpState::Exit(CTanjiro * pTanjiro)
{
}



void JumpState::Jump(CTanjiro* pTanjiro, _float fTimeDelta)
{
	static _float fStartHeight = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fEndHeight = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fVelocity = 5.f;
	static _float fGravity = 6.f;


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

	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}


