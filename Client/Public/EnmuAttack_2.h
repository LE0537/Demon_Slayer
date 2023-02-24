#pragma once

#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)
class CEnmuAttack2 : public CEnmuState
{
public:
	CEnmuAttack2(STATE_TYPE eType);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	

	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;


private:
	STATE_ID ePreState = CEnmuState::STATE_END;
	_bool m_bNextAnim = false;

};
END
END

