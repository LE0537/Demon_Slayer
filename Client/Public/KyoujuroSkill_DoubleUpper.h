#pragma once

#include "KyoujuroState.h"
#include "WaterMill.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CSkill_DoubleUpperState : public CKyoujuroState
{
public:
	CSkill_DoubleUpperState();

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* CommandCheck(CKyoujuro* pKyoujuro) override;
private:
	CKyoujuroState* Jump(CKyoujuro* pKyoujuro, _float fTimeDelta);
private:
	CWaterMill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;

private:
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fCurrentPosY = 0.f;
	_bool  m_bJump = false;
	_bool	m_bEffect = false;
	_float4 m_vLook;
	_bool   m_bLook = false;



};
END
END

