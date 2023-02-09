#pragma once

#include "RuiDadState.h"

BEGIN(Client)
BEGIN(RuiDad)
class CGuardHitState : public CRuiDadState
{
public:
	CGuardHitState(STATE_TYPE eState);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;

private:
	_float m_fTime = 0.f;
	_bool m_bEffect = false;
};
END
END

