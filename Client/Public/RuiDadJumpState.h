#pragma once
#include "RuiDadState.h"

BEGIN(Client)
BEGIN(RuiDad)

class CJumpState : public CRuiDadState
{
public:
	CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime, _bool bAiMoveAttack = false);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;

private:
	CRuiDadState* Jump(CRuiDad* pRuiDad, _float fTimeDelta);


private:
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;

	_bool m_bMoveAtkJump = false;
	_bool m_bEffect = false;
};
END
END