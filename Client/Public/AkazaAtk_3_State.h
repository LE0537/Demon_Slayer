#pragma once

#include "AkazaState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Akaza)
class CAtk_3_State : public CAkazaState
{
public:
	CAtk_3_State();

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

	virtual CAkazaState* CommandCheck(CAkaza* pAkaza) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;
	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;

	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
};
END
END

