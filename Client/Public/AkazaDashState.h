#pragma once

#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)
class CDashState : public CAkazaState
{
public:
	CDashState(OBJDIR eDir, _bool bSecondDash = false, _bool bJump = false);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;


private:
	void Move(CAkaza* pAkaza, _float fTimeDelta);
	void Check_Coll(CAkaza* pAkaza, _float fTimeDelta);
private:
	OBJDIR m_eDir = OBJDIR::DIR_END;
	_bool m_bJump = false;
	_bool m_bSecondDash = false;
	_float m_fDashTime = 1.f;
	_float m_fSpeed = 0.f;
	_float m_fTime = 0.f;
	_int   iIndex = 0;
	_bool  m_bTrue = false;
};
END
END