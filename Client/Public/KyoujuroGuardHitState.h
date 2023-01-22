#pragma once

#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CGuardHitState : public CKyoujuroState
{
public:
	CGuardHitState(STATE_TYPE eState);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;

private:
	_float m_fTime = 0.f;
};
END
END

