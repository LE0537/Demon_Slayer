#pragma once

#include "RuiDadState.h"

BEGIN(Client)
BEGIN(RuiDad)
class CGuardState : public CRuiDadState
{
public:
	CGuardState(STATE_TYPE eType = STATE_TYPE::TYPE_START);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;

private:
	_float m_fTime = 0.f;
};
END
END

