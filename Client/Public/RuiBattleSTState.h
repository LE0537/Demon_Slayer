#pragma once

#include "RuiState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Rui)
class CBattleStartState : public CRuiState
{
public:
	CBattleStartState();

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;
private:
	_float  m_fTime = 0.f;
};
END
END

