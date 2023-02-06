#pragma once

#include "KyoujuroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CSkill_DashSlashState : public CKyoujuroState
{
public:
	CSkill_DashSlashState();

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* CommandCheck(CKyoujuro* pKyoujuro) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_float4 m_vLook;
	_bool   m_bLook = false;
	_bool	m_bEffect = false;
};
END
END

