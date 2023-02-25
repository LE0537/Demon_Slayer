#pragma once
#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)

class CSkill2_EnmuState : public CEnmuState
{
public:
	CSkill2_EnmuState(STATE_TYPE eType);
	
	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;


};

END
END