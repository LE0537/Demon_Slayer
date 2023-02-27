#pragma once
#include "EnmuBossState.h"

BEGIN(Client)
BEGIN(EnmuBoss)

class CEnmuBoss_Pattern2State : public CEnmuBossState
{
public:
	CEnmuBoss_Pattern2State(STATE_TYPE eType, CEnmuBoss::PARTS eParts);

	virtual CEnmuBossState* HandleInput(CEnmuBoss* pEnmuBoss) override;
	virtual CEnmuBossState* Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) override;
	virtual CEnmuBossState* Late_Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) override;


	virtual void Enter(CEnmuBoss* pEnmuBoss) override;
	virtual void Exit(CEnmuBoss* pEnmuBoss) override;

private:
	void Create_Chok();

private:
	CEnmuBoss::PARTS m_eParts;


};
END
END