#pragma once

#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)
class CGuardHitState : public CEnmuState
{
public:
	CGuardHitState(STATE_TYPE eState);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;

private:
	_float m_fTime = 0.f;
	_bool m_bEffect = false;
};
END
END

