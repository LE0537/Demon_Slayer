#pragma once

#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)
class CGuardState : public CEnmuState
{
public:
	CGuardState(STATE_TYPE eType = STATE_TYPE::TYPE_START);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;

private:
	_float m_fTime = 0.f;
};
END
END

