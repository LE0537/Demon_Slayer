#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Nezuko)
class CGuardAdvState : public CNezukoState
{
public:
	CGuardAdvState();

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;

	_float m_fMove = 0.f;
	_bool  m_bHit = false;
};
END
END

