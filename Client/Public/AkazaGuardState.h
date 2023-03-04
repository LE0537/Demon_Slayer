#pragma once

#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)
class CGuardState : public CAkazaState
{
public:
	CGuardState(STATE_TYPE eType = STATE_TYPE::TYPE_START);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;


private:
	_float m_fTime = 0.f;
	_float m_fGuardTime = 0.f;
};
END
END

