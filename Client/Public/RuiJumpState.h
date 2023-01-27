#pragma once
#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)

class CJumpState : public CRuiState
{
public:
	CJumpState(STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;

private:
	CRuiState* Jump(CRui* pRui, _float fTimeDelta);


private:
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;

};
END
END