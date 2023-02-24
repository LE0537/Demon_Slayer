#pragma once

#include "EnmuState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Enmu)
class CBattleStartState : public CEnmuState
{
public:
	CBattleStartState();

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;
private:
	_float  m_fTime = 0.f;
};
END
END

