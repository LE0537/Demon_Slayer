#pragma once
#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)

class CJumpState : public CAkazaState
{
public:
	CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

private:
	CAkazaState* Jump(CAkaza* pAkaza, _float fTimeDelta);


private:
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;
	_bool m_bEffect = false;
};
END
END