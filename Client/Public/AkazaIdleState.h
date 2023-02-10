#pragma once
#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)

class CIdleState : public CAkazaState
{
public:
	CIdleState(STATE_ID eState = STATE_END);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

private:
	STATE_ID m_ePreState = STATE_END;
	_float   m_fSkillBarTime = 0.f;
};
END
END