#pragma once
#include "RuiDadState.h"

BEGIN(Client)
BEGIN(RuiDad)

class CMoveState : public CRuiDadState
{
public:
	CMoveState(OBJDIR eDir, STATE_TYPE eType);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;



public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CRuiDad* pRuiDad, _float fTimeDelta);
	CRuiDadState* AIMove(CRuiDad* pRuiDad, OBJDIR eDir,_float fTimeDelta);

private:
	OBJDIR	m_eDirection = OBJDIR::DIR_END;

	_float m_fShakeTime = 0.f;
};
END
END