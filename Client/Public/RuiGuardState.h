#pragma once

#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)
class CGuardState : public CRuiState
{
public:
	CGuardState(STATE_TYPE eType = STATE_TYPE::TYPE_START);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;

	CRuiState* AIGuard(CRui* pRui, _float fTimeDelta);
private:
	_float m_fTime = 0.f;
};
END
END

