#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Nezuko)
class CAtk_2_State : public CNezukoState
{
public:
	CAtk_2_State();

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;

	virtual CNezukoState* CommandCheck(CNezuko* pNezuko) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;
	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;

	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_bool  m_bEffect = false;
};
END
END

