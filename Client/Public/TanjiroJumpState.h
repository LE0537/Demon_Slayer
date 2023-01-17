#pragma once

#include "TanjiroState.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CJumpstate : public CTanjiroState
{
public:
	CJumpstate(STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;

	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;


private:
	CTanjiroState* Jump(CTanjiro* pTanjiro, _float fTimeDelta);


private:
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;
	

};
END
END

