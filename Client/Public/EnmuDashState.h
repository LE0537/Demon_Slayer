#pragma once

#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)
class CDashState : public CEnmuState
{
public:
	CDashState(OBJDIR eDir, _bool bSecondDash = false, _bool bJump = false);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;


private:
	void Move(CEnmu* pEnmu, _float fTimeDelta);
	void Check_Coll(CEnmu* pEnmu, _float fTimeDelta);
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