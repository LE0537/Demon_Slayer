#pragma once

#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CUpperHitState : public CKyoujuroState
{
public:
	CUpperHitState(CKyoujuro::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime = 0.035f);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;


private:
	void Increase_Height(CKyoujuro* pKyoujuro, _float fTimeDelta);
	void Fall_Height(CKyoujuro* pKyoujuro, _float fTimeDelta);

	void KnockBack_Player(CKyoujuro* pKyoujuro, _float fTimeDelta);
	void Bound_Player(CKyoujuro* pKyoujuro, _float fTimeDelta);
	void Upper_Player(CKyoujuro* pKyoujuro, _float fTimeDelta);

	CKyoujuroState* KnockBackState(CKyoujuro* pKyoujuro, _float fTimeDelta);
	CKyoujuroState* BoundState(CKyoujuro* pKyoujuro, _float fTimeDelta);
	CKyoujuroState* UpperState(CKyoujuro* pKyoujuro, _float fTimeDelta);
	CKyoujuroState* Upper2State(CKyoujuro* pKyoujuro, _float fTimeDelta);
private:
	CKyoujuro::HIT_TYPE m_eHitType = CKyoujuro::HIT_TYPE::HIT_END;
	_float m_fBoundPower = 0.f;
	_float m_fJumpPower = 0.f;
	_float m_fKnockBackPower = 0.f;

private:
	_bool m_bHitPlayerJump = false;
	_bool m_bNextAnim = false;
	_bool m_bReset = false;
	_bool m_bMaxHeight = false;

private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fCurrentPosY = 0.f;
	_float m_fJumpTime = 0.f;
	_float m_fDelay = 0.f;

};
END
END