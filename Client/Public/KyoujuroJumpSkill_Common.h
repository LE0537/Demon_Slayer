#pragma once

#include "KyoujuroState.h"
#include "KyoujuroJumpSkill.h"
BEGIN(Client)
BEGIN(Kyoujuro)
class CJumpSkill_CommonState : public CKyoujuroState
{
public:
	CJumpSkill_CommonState(_float fPositionY);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;


	CKyoujuroState* Jump(CKyoujuro*pKyoujuro, _float fTimeDelta);

private:
	CKyoujuroJumpSkill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_float m_fPositionY = 0.f;
	_float m_fJumpTime = 0.f;
	_float4 m_vLook;
	_bool   m_bLook = false;
	//test
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_bool m_bOnGround = false;

	_bool	m_bEffect = false;

	_float fTest = 0.f;

	_bool m_bCreate = false;
};
END
END

