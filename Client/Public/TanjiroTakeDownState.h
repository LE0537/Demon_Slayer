#pragma once

#include "TanjiroState.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CTakeDownState : public CTanjiroState
{
public:
	CTakeDownState(_float _fPow,_bool _bJump = 0, STATE_TYPE eType = TYPE_START);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;
private:
	CTanjiroState* Jump(CTanjiro* pTanjiro, _float fTimeDelta);
	void Set_HitState(CTanjiro* pTanjiro);
	void Set_JumpHitState(CTanjiro* pTanjiro);
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