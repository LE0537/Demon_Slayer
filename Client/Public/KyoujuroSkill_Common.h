#pragma once

#include "KyoujuroState.h"
#include "KyoujuroSkill.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Kyoujuro)
class CSkill_CommonState : public CKyoujuroState
{
public:
	CSkill_CommonState();

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;
private:
	CKyoujuroSkill*	m_pCollBox2 = nullptr;
	CBaseAtk*	m_pCollBox = nullptr;
	CGameObj*   m_pEffect = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_float4 m_vLook;
	_bool   m_bLook = false;

	_bool	m_bEffect = false;

	_bool   m_bTrue = false;
};
END
END

