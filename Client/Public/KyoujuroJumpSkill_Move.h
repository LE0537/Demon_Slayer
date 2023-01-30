#pragma once

#include "KyoujuroState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Kyoujuro)
class CJumpSkill_MoveState : public CKyoujuroState
{
public:
	CJumpSkill_MoveState(_float fPositionY);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;

	CKyoujuroState* Move(CKyoujuro*pKyoujuro, _float fTimeDelta);
	CKyoujuroState* Jump(CKyoujuro*pKyoujuro, _float fTimeDelta);

private:
	CBaseAtk*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_float m_fPositionY = 0.f;
	_float m_fJumpTime = 0.f;

	//test
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_bool m_bOnGround = false;
	_bool m_bFinishAnim = false;
	_bool m_bNextAnim = false;
	_float m_fGravity = -19.8f;
};
END
END

