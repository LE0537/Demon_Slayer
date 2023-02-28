#pragma once

#include "AkazaState.h"


BEGIN(Client)
BEGIN(Akaza)


class CSplSkrStartState : public CAkazaState
{
public:
	CSplSkrStartState(STATE_TYPE eType);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

private:
	void Move(CAkaza* pAkaza, _float fTimeDelta);

private:
	_bool m_bCollision = false;

};

END
END