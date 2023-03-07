#pragma once

#include "AkazaState.h"
#include "BaseAtk.h"

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
	CBaseAtk*	m_pCollBox = nullptr;

	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;
	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_bool  m_bEffect = false;

	_bool m_bCreate = false;
	_bool m_bPlayScene = false;
};

END
END