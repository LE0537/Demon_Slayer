#pragma once

#include "NezukoState.h"

BEGIN(Client)
BEGIN(Nezuko)
class CGuardHitState : public CNezukoState
{
public:
	CGuardHitState(STATE_TYPE eState);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;

private:
	_float m_fTime = 0.f;
};
END
END

