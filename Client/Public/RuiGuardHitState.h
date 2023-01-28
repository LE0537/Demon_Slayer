#pragma once

#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)
class CGuardHitState : public CRuiState
{
public:
	CGuardHitState(STATE_TYPE eState);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;

private:
	_float m_fTime = 0.f;
};
END
END

