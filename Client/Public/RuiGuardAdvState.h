#pragma once

#include "RuiState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Rui)
class CGuardAdvState : public CRuiState
{
public:
	CGuardAdvState();

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;

	_float m_fMove = 0.f;
	_bool  m_bHit = false;
};
END
END

