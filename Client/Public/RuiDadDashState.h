#pragma once

#include "RuiDadState.h"

BEGIN(Client)
BEGIN(RuiDad)
class CDashState : public CRuiDadState
{
public:
	CDashState(OBJDIR eDir, _bool bSecondDash = false, _bool bJump = false);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;


private:
	void Move(CRuiDad* pRuiDad, _float fTimeDelta);
	void Check_Coll(CRuiDad* pRuiDad, _float fTimeDelta);
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