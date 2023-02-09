#pragma once
#include "RuiDadState.h"

BEGIN(Client)
BEGIN(RuiDad)

class CIdleState : public CRuiDadState
{
public:
	CIdleState(STATE_ID eState = STATE_END);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;

private:
	STATE_ID m_ePreState = STATE_END;
};
END
END