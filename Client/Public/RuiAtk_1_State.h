#pragma once

#include "RuiState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Rui)
class CAtk_1_State : public CRuiState
{
public:
	CAtk_1_State();

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


private:
	CBaseAtk*	m_pCollBox = nullptr;
	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;

	_float m_fMove = 0.f;
	_int   m_iHit = 0;
};
END
END

