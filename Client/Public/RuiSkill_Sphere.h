#pragma once

#include "RuiState.h"


BEGIN(Client)
BEGIN(Rui)
class CSkill_SphereState : public CRuiState
{
public:
	CSkill_SphereState(STATE_TYPE eType);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;
private:

	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;
};
END
END

