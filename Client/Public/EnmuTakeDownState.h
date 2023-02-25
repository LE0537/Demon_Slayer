#pragma once

#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)
class CTakeDownState : public CEnmuState
{
public:
	CTakeDownState(_float _fPow,_bool _bJump = 0, STATE_TYPE eType = TYPE_START);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;
private:
	CEnmuState* Jump(CEnmu* pEnmu, _float fTimeDelta);
	void Set_HitState(CEnmu* pEnmu);
	void Set_JumpHitState(CEnmu* pEnmu);
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