#pragma once

#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)
class CIdleState : public CShinobuState
{
public:
	CIdleState(STATE_ID eState = CShinobuState::STATE_END);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	

	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


private:
	STATE_ID ePreState = CShinobuState::STATE_END;
	_float   m_fSkillBarTime = 0.f;

};
END
END

