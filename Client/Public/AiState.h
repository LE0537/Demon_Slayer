#pragma once

#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)
class CAiState : public CRuiState
{
public:
	enum TARGET_STATE { STATE_IDLE, STATE_MOVE, STATE_JUMP, STATE_ATTACK, STATE_SKILL, STATE_GUARD, STATE_HIT, STATE_RUSH, STATE_END};
	enum AI_STATE {AI_IDLE, AI_BACKMOVE ,AI_FRONTMOVE, AI_ATTACK, AI_GUARD, AI_SKILL, AI_DASH , AI_JUMP , AI_HIT ,AI_END};
	enum AI_REACT { REACT_GUARD, REACT_ATTACK, REACT_MOVE, REACT_SKILL, REACT_DASH, REACT_JUMP};
	enum AI_RANGE {RANGE_IN, RANGE_NEAR, RANGE_FAR ,RANGE_OUT, RANGE_END};
	enum AI_DIR { DIR_STRAIGHT, DIR_LEFT, DIR_RIGHT, DIR_BACK, DIR_LF, DIR_RF, DIR_LB, DIR_RB, DIR_LEFT_DASH, DIR_RIGHT_DASH, DIR_STOP, DIR_END };

public:
	CAiState();

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;

	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;
		

private:
	void Update_TargetState(CRui* pRui);
	void Update_TargetToAngle(CRui* pRui);

	void Update_AI_Near(CRui* pRui);
	void Update_AI_Far(CRui* pRui);
	CRuiState* Return_AIState(CRui* pRui);

private:
	void Update_NearAttack();








private:
	TARGET_STATE m_eTargetState = STATE_END;
	

	AI_RANGE	 m_eRange = RANGE_END;
	AI_DIR		 m_eDir;
	AI_STATE	 m_eState = AI_END;


	_uint m_iTargetState = 0;

	vector<AI_REACT> m_vecReact;
};

END
END