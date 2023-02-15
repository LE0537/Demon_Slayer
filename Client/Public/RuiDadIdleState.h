#pragma once
#include "RuiDadState.h"

BEGIN(Client)
BEGIN(RuiDad)

class CIdleState : public CRuiDadState
{
public:
	enum TARGET_STATE { TARGET_IDLE, TARGET_MOVE, TARGET_JUMP, TARGET_ATTACK, TARGET_SKILL, TARGET_GUARD, TARGET_HIT, TARGET_RUSH, TARGET_END };
	enum AI_STATE {AI_IDLE, AI_BACKMOVE, AI_FRONTMOVE, AI_ATTACK, AI_GUARD, AI_SKILL, AI_DASH, AI_DASH_F, AI_DASH_B, AI_DASH_L, AI_DASH_R, AI_JUMPATK, AI_JUMP, AI_RUSH, AI_HIT, AI_ADV_SKILL_0, AI_ADV_SKILL_1, AI_END};
	enum AI_DASHPOINT { DASH_F, DASH_L, DASH_R };
	enum AI_RANGE { RANGE_IN, RANGE_NEAR, RANGE_FAR, RANGE_OUT, RANGE_END };
	enum AI_DIR { DIR_STRAIGHT, DIR_LEFT, DIR_RIGHT, DIR_BACK, DIR_LF, DIR_RF, DIR_LB, DIR_RB, DIR_LEFT_DASH, DIR_RIGHT_DASH, DIR_STOP, DIR_END };

public:
	CIdleState(STATE_ID eState = STATE_END);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;

private:
	STATE_ID m_ePreState = STATE_END;
};
END
END