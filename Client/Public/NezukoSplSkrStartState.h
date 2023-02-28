#pragma once

#include "NezukoState.h"


BEGIN(Client)
BEGIN(Nezuko)


class CSplSkrStartState : public CNezukoState
{
public:
	CSplSkrStartState(STATE_TYPE eType);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;

private:
	void Move(CNezuko* pNezuko, _float fTimeDelta);

private:
	_bool m_bCollision = false;

};

END
END