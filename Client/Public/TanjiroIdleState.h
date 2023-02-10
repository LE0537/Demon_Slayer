#pragma once

#include "TanjiroState.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CIdleState : public CTanjiroState
{
public:
	CIdleState(STATE_ID eState = CTanjiroState::STATE_END);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	

	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;


private:
	STATE_ID ePreState = CTanjiroState::STATE_END;
	_float   m_fSkillBarTime = 0.f;
};
END
END

