#pragma once

#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)
class CTakeDownState : public CRuiState
{
public:
	CTakeDownState(_float _fPow,_bool _bJump = 0, STATE_TYPE eType = TYPE_START);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;
private:
	CRuiState* Jump(CRui* pRui, _float fTimeDelta);
	void Set_HitState(CRui* pRui);
	void Set_JumpHitState(CRui* pRui);
private:
	_float fHitTime = 0.f;
	_float m_fPow = 0.f;
	_bool  m_bReset = false;
	_bool  m_bJumpHit = false;
	_bool  m_bJump = false;
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fCurrentPosY = 0.f;
	_bool  m_bTrun = false;


private:
	_bool m_bHitPlayerJump = false;

};
END
END