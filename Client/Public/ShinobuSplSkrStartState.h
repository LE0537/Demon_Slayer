#pragma once

#include "ShinobuState.h"
#include "BaseAtk.h"

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