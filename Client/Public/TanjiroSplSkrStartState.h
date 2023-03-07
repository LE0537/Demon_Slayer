#pragma once

#include "TanjiroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Tanjiro)


class CSplSkrStartState : public CTanjiroState
{
public:
	CSplSkrStartState(STATE_TYPE eType);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

private:
	void Move(CTanjiro* pTanjiro, _float fTimeDelta);

private:
	_bool m_bCollision = false;
	_bool m_bPlayScene = false;
	_bool m_bCreate = false;
	CBaseAtk*	m_pCollBox = nullptr;

	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;
	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_bool  m_bEffect = false;


};

END
END