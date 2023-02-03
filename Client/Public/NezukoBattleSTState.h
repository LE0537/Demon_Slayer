#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Nezuko)
class CBattleStartState : public CNezukoState
{
public:
	CBattleStartState();

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;
};
END
END

