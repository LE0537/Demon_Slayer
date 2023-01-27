#pragma once

#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)
class CGuardHitState : public CAkazaState
{
public:
	CGuardHitState(STATE_TYPE eState);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

private:
	_float m_fTime = 0.f;
};
END
END

