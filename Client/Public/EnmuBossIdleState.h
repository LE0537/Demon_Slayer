#pragma once
#include "EnmuBossState.h"

BEGIN(Client)
BEGIN(EnmuBoss)

class CIdleState : public CEnmuBossState
{
public:
	enum AI_RANGE { RANGE_IN,  RANGE_OUT, RANGE_END };
	enum AI_PATTERN {PATTERN_1, PATTERN_2, PATTERN_3, PATTERN_4, PATTERN_5, PATTERN_6, PATTERN_END};


public:
	CIdleState();

	virtual CEnmuBossState* HandleInput(CEnmuBoss* pEnmuBoss) override;
	virtual CEnmuBossState* Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) override;
	virtual CEnmuBossState* Late_Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) override;


	virtual void Enter(CEnmuBoss* pEnmuBoss) override;
	virtual void Exit(CEnmuBoss* pEnmuBoss) override;



private:
	void Update_AI_Near(CEnmuBoss* pEnmuBoss);
	void Update_AI_Out(CEnmuBoss* pEnmuBoss);

	CEnmuBossState* Return_AIState(CEnmuBoss* pEnmuBoss);


private:
	_float m_fDelay = 0.f;
	AI_RANGE m_eRange = RANGE_END;
	AI_PATTERN m_ePattern = PATTERN_END;
	_bool m_bStop = false;
	_bool m_bPlay = false;
};
END
END