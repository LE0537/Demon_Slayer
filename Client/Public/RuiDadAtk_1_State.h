#pragma once

#include "RuiDadState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(RuiDad)
class CAtk_1_State : public CRuiDadState
{
public:
	CAtk_1_State(STATE_TYPE eType);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;


private:
	CBaseAtk*	m_pCollBox = nullptr;
	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;
	_bool  m_bEffect = false;
	_float m_fMove = 0.f;
	_float m_fDelay = 0.f;
	_int   m_iHit = 0;
	_bool m_bNextAnim = false;
};
END
END

