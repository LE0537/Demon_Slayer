#pragma once

#include "ShinobuState.h"


BEGIN(Client)
BEGIN(Shinobu)


class CSplSkrStartState : public CShinobuState
{
public:
	CSplSkrStartState(STATE_TYPE eType);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

private:
	void Move(CShinobu* pShinobu, _float fTimeDelta);

private:
	_bool m_bCollision = false;

};

END
END