#pragma once

#include "AkazaState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Akaza)
class CGuardAdvState : public CAkazaState
{
public:
	CGuardAdvState();

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;

	_float m_fMove = 0.f;
	_bool  m_bHit = false;
};
END
END

