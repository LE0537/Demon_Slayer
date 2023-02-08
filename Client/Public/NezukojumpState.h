#pragma once
#include "NezukoState.h"

BEGIN(Client)
BEGIN(Nezuko)

class CJumpState : public CNezukoState
{
public:
	CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;

private:
	CNezukoState* Jump(CNezuko* pNezuko, _float fTimeDelta);


private:
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;
	_bool m_bEffect = false;
};
END
END