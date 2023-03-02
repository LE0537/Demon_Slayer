#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"

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
	CBaseAtk*	m_pCollBox = nullptr;

	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;
	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_bool  m_bEffect = false;
};

END
END