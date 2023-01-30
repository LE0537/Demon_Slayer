#pragma once
#include "NezukoState.h"


BEGIN(Client)
BEGIN(Nezuko)

class CIdleState : public CNezukoState
{
public:
	CIdleState(STATE_ID eState = STATE_END);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;

private:
	STATE_ID m_ePreState = STATE_END;
};
END
END