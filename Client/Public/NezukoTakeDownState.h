#pragma once

#include "NezukoState.h"

BEGIN(Client)
BEGIN(Nezuko)
class CTakeDownState : public CNezukoState
{
public:
	CTakeDownState(_float _fPow,_bool _bJump = 0, STATE_TYPE eType = TYPE_START);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;
private:
	CNezukoState* Jump(CNezuko* pNezuko, _float fTimeDelta);
	void Set_HitState(CNezuko* pNezuko);
	void Set_JumpHitState(CNezuko* pNezuko);
private:
	_float fHitTime = 0.f;
	_float m_fPow = 0.f;
	_bool  m_bReset = false;
	_bool  m_bJumpHit = false;
	_bool  m_bJump = false;
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fCurrentPosY = 0.f;
	_bool  m_bTrun = false;


private:
	_bool m_bHitPlayerJump = false;

};
END
END