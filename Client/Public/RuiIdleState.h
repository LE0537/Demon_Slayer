#pragma once
#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)

class CIdleState : public CRuiState
{
public:
	CIdleState(STATE_ID eState = STATE_END);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;

private:
	STATE_ID m_ePreState = STATE_END;
	_float   m_fSkillBarTime = 0.f;
};
END
END