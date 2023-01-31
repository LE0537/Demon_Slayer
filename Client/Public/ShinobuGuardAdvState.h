#pragma once

#include "ShinobuState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Shinobu)
class CGuardAdvState : public CShinobuState
{
public:
	CGuardAdvState();

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

private:
	CBaseAtk*	m_pCollBox = nullptr;

	_float m_fMove = 0.f;
	_bool  m_bHit = false;
};
END
END

