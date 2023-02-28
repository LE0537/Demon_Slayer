#pragma once

#include "RuiState.h"


BEGIN(Client)
BEGIN(Rui)


class CSplSkrStartState : public CRuiState
{
public:
	CSplSkrStartState(STATE_TYPE eType);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;

private:
	void Move(CRui* pRui, _float fTimeDelta);

private:
	_bool m_bCollision = false;

};

END
END