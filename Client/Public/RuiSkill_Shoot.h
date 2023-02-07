#pragma once

#include "RuiState.h"


BEGIN(Client)
BEGIN(Rui)
class CSkill_ShootState : public CRuiState
{
public:
	CSkill_ShootState(STATE_TYPE eType);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;
	virtual CRuiState* CommandCheck(CRui* pRui) override;
private:

	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bEffect = false;
};
END
END

