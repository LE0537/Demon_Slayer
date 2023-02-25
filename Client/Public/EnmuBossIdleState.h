#pragma once
#include "EnmuBossState.h"

BEGIN(Client)
BEGIN(EnmuBoss)

class CIdleState : public CEnmuBossState
{
public:
	CIdleState();

	virtual CEnmuBossState* HandleInput(CEnmuBoss* pEnmuBoss) override;
	virtual CEnmuBossState* Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) override;
	virtual CEnmuBossState* Late_Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) override;


	virtual void Enter(CEnmuBoss* pEnmuBoss) override;
	virtual void Exit(CEnmuBoss* pEnmuBoss) override;


};
END
END