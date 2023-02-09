#include "stdafx.h"
#include "RuiDadJumpState.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
using namespace RuiDad;

CJumpState::CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime, _bool bAiMoveAttack)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
	m_bMoveAtkJump = bAiMoveAttack;
}

CRuiDadState * CJumpState::HandleInput(CRuiDad* pRuiDad)
{
	

	return nullptr;
}

CRuiDadState * CJumpState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	


	return nullptr;
}

CRuiDadState * CJumpState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	

	return nullptr;
}

void CJumpState::Enter(CRuiDad* pRuiDad)
{

}

void CJumpState::Exit(CRuiDad* pRuiDad)
{
}

CRuiDadState * CJumpState::Jump(CRuiDad* pRuiDad, _float fTimeDelta)
{



	return nullptr;
}



