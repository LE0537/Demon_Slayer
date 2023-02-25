#pragma once

#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)
class CMoveState : public CEnmuState
{
public:
	CMoveState(STATE_TYPE eType);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	

	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;

	void Move(CEnmu* pEnmu, _float fTimeDelta);
private:
	STATE_ID ePreState = CEnmuState::STATE_END;
	_float   m_fMoveTime = 0.f;
	_bool	 m_bNextAnim = false;


};
END
END

