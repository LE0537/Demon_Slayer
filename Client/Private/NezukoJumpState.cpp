#include "stdafx.h"
#include "NezukoJumpState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "NezukoJumpAttackState.h"

using namespace Nezuko;

CJumpState::CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CNezukoState * CJumpState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pNezuko->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J))
		{
			return new CJumpAttackState(TYPE_START);
		}
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z))
		{
			return new CJumpAttackState(TYPE_START);
		}
		break;
	}


	return nullptr;
}

CNezukoState * CJumpState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간

	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_JUMP_START);
	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_JUMP_LOOP);
	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_JUMP_END);



	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			return new CJumpState(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			//return new CJumpState(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}

	  
	return nullptr;
}

CNezukoState * CJumpState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.05f;

	if (m_eStateType != TYPE_END)
		Jump(pNezuko, fTimeDelta + m_fJumpTime);

	return nullptr;
}

void CJumpState::Enter(CNezuko* pNezuko)
{
	m_eStateId = CNezukoState::STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_JUMP_START);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_JUMP_START, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_START);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_JUMP_LOOP);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_JUMP_LOOP, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_LOOP);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_JUMP_END);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_JUMP_END, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_END);
		break;
	}
}

void CJumpState::Exit(CNezuko* pNezuko)
{
}

CNezukoState * CJumpState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_eStateType = CNezukoState::TYPE_END;
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_JUMP_END);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_JUMP_END, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_END);
	}




	pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}



