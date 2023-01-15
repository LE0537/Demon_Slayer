#include "stdafx.h"
#include "TanjiroMoveState.h"
#include "TanjiroIdlestate.h"
#include "GameInstance.h"

using namespace Tanjiro;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CTanjiroState * CMoveState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_W)) // 菊
	{
		if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_LOOP);
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快
			return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_LOOP);
		else
			return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_LOOP);
	}

	else if (pGameInstance->Key_Pressing(DIK_D)) // 第
	{
		if (pGameInstance->Key_Pressing(DIK_A)) // 谅
			return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_LOOP);
		else if (pGameInstance->Key_Pressing(DIK_D)) // 快 
			return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_LOOP);
		else
			return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_LOOP);
	}


	else if (pGameInstance->Key_Pressing(DIK_A)) // 谅
		return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_LOOP);
	else if (pGameInstance->Key_Pressing(DIK_S)) // 快
		return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_LOOP);
	else

		return new CMoveState(OBJDIR::DIR_STOP, STATE_TYPE::TYPE_END);



	return nullptr;
}

CTanjiroState * CMoveState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	Move(pTanjiro, fTimeDelta);
	//pTanjiro->Get_Model()->Play_Animation(fTimeDelta);


	return nullptr;
}

CTanjiroState * CMoveState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			m_eStateType = CTanjiroState::TYPE_LOOP;
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			m_eStateType = CTanjiroState::TYPE_LOOP;
			break;
		case Client::CTanjiroState::TYPE_END:
			return new CIdleState();
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}


	return nullptr;
}

void CMoveState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_START);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_START);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_LOOP);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_LOOP);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_END);
		break;
	case Client::CTanjiroState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CTanjiro * pTanjiro)
{
}

void CMoveState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pTanjiro->Tanjiro_GoStraight(fTimeDelta);
		break;
	case Client::DIR_LEFT:
		pTanjiro->Tanjiro_GoLeft(fTimeDelta);
		break;
	case Client::DIR_RIGHT:
		pTanjiro->Tanjiro_GoRight(fTimeDelta);
		break;
	case Client::DIR_BACK:
		pTanjiro->Tanjiro_GoBackWard(fTimeDelta);
		break;
	case Client::DIR_LF:
		pTanjiro->Tanjiro_GoLF(fTimeDelta);
		break;
	case Client::DIR_RF:
		pTanjiro->Tanjiro_GoRF(fTimeDelta);
		break;
	case Client::DIR_LB:
		pTanjiro->Tanjiro_GoLB(fTimeDelta);
		break;
	case Client::DIR_RB:
		pTanjiro->Tanjiro_GoRB(fTimeDelta);
		break;
	case Client::DIR_STOP:
		break;
	}


}


