#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Nezuko)
class CAtk_4_State : public CNezukoState
{
public:
	CAtk_4_State();

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


private:
	CBaseAtk*	m_pCollBox = nullptr;
	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;

	_float m_fMove = 0.f;
	_int   m_iHit = 0;
};
END
END

