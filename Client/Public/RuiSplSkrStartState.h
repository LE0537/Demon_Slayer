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
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;
	_bool  m_bEffect = false;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;

	_bool m_bPlayScene = false;
};

END
END