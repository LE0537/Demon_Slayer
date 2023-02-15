#pragma once

#include "RuiDadState.h"


BEGIN(Client)
BEGIN(RuiDad)
class CSkill_PunchState : public CRuiDadState
{
public:
	CSkill_PunchState(STATE_TYPE eType);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;

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

