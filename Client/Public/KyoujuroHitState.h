#pragma once

#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CHitState : public CKyoujuroState
{
public:
	CHitState(_float _fPow, _bool _bJump = 0);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;
private:
	CKyoujuroState* Jump(CKyoujuro* pKyoujuro, _float fTimeDelta);
	void Set_HitState(CKyoujuro* pKyoujuro);
	void Set_JumpHitState(CKyoujuro* pKyoujuro);
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