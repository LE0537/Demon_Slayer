#pragma once

#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)
class CDashState : public CShinobuState
{
public:
	CDashState(OBJDIR eDir, _bool bSecondDash = false, _bool bJump = false);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


private:
	void Move(CShinobu* pShinobu, _float fTimeDelta);
	void Check_Coll(CShinobu* pShinobu, _float fTimeDelta);
private:
	OBJDIR m_eDir = OBJDIR::DIR_END;
	_bool m_bJump = false;
	_bool m_bSecondDash = false;
	_float m_fDashTime = 1.f;
	_float m_fSpeed = 0.f;
	_float m_fTime = 0.f;
	_int   iIndex = 0;
	_bool  m_bTrue = false;
	_bool  m_bEffect = false;
};
END
END