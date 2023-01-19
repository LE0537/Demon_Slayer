#pragma once

#include "KyoujuroState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Kyoujuro)
class CGuardAdvState : public CKyoujuroState
{
public:
	CGuardAdvState();

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;

	_float m_fMove = 0.f;
	_bool  m_bHit = false;
};
END
END

