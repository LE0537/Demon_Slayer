#pragma once

#include "TanjiroState.h"

BEGIN(Client)
BEGIN(Tanjiro)
class JumpState : public CTanjiroState
{
public:
	JumpState(STATE_TYPE eType);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;



private:
	void Jump(CTanjiro* pTanjiro, _float fTimeDelta);

private:
	_float m_fJumpTime = 0.f;
	_bool  m_bJump = true;

};
END
END

