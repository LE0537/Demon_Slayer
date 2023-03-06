#pragma once
#include "EnmuBossState.h"

BEGIN(Client)
BEGIN(EnmuBoss)

class CEnmuBoss_Pattern6State : public CEnmuBossState
{
public:
	CEnmuBoss_Pattern6State(STATE_TYPE eType, CEnmuBoss::PARTS eParts);

	virtual CEnmuBossState* HandleInput(CEnmuBoss* pEnmuBoss) override;
	virtual CEnmuBossState* Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) override;
	virtual CEnmuBossState* Late_Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) override;


	virtual void Enter(CEnmuBoss* pEnmuBoss) override;
	virtual void Exit(CEnmuBoss* pEnmuBoss) override;

private:
	void Create_Chok(_vector vPosition);
	void Create_TargetChok();
private:
	CEnmuBoss::PARTS m_eParts;

	_float m_fTime = 0.f;
	_float m_fCreateTime = 0.f;
	_bool m_bCreateBool = false;
};
END
END