#pragma once

#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)
class CGuardHitState : public CShinobuState
{
public:
	CGuardHitState(STATE_TYPE eState);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

private:
	_float m_fTime = 0.f;
	_bool m_bEffect = false;
};
END
END

