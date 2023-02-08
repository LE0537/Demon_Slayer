#pragma once

#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)
class CJumpstate : public CShinobuState
{
public:
	CJumpstate(STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;

	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


private:
	CShinobuState* Jump(CShinobu* pShinobu, _float fTimeDelta);


private:
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;
	
	_bool m_bEffect = false;
};
END
END

