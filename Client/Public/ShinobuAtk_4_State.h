#pragma once

#include "ShinobuState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Shinobu)
class CAtk_4_State : public CShinobuState
{
public:
	CAtk_4_State();

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

private:
	CBaseAtk*	m_pCollBox = nullptr;

	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_float m_fComboDelay = 0.f;
	_bool  m_bEffect = false;
	_float m_fDelay = 0.f;
	_int   m_iHit = 0;
};
END
END

