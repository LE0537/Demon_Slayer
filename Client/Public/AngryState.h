#pragma once

#include "RuiDadState.h"
BEGIN(Client)
BEGIN(RuiDad)

class CAngryState : public CRuiDadState
{
public:
	CAngryState();

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;


};

END
END