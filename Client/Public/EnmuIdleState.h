#pragma once

#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)
class CIdleState : public CEnmuState
{
public:
	enum TARGET_STATE { STATE_IDLE, STATE_MOVE, STATE_JUMP, STATE_ATTACK, STATE_SKILL, STATE_GUARD, STATE_HIT, STATE_RUSH, STATE_END };
	enum AI_STATE { AI_IDLE, AI_BACKMOVE, AI_FRONTMOVE, AI_ATTACK, AI_GUARD, AI_SKILL, AI_DASH, AI_DASH_F, AI_DASH_B, AI_DASH_L, AI_DASH_R, AI_JUMPATK, AI_JUMP, AI_RUSH, AI_HIT, AI_SKILL_0, AI_SKILL_1, AI_SKILL_2, AI_SKILL_3, AI_END };
	enum AI_DASHPOINT { DASH_F, DASH_L, DASH_R };
	enum AI_RANGE { RANGE_IN, RANGE_NEAR, RANGE_FAR, RANGE_OUT, RANGE_END };
	enum AI_DIR { DIR_STRAIGHT, DIR_LEFT, DIR_RIGHT, DIR_BACK, DIR_LF, DIR_RF, DIR_LB, DIR_RB, DIR_LEFT_DASH, DIR_RIGHT_DASH, DIR_STOP, DIR_END };

public:
	CIdleState(STATE_ID eState = CEnmuState::STATE_END);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	

	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;



private:
	void Update_TargetState(CEnmu* pEnmu);


	void Update_AI_Near(CEnmu* pEnmu);
	void Update_AI_Out(CEnmu* pEnmu);


	void Update_Near_Attack();
	void Update_Near_Move();
	void Update_Near_Guard();
	void Update_Near_Rush();
	void Update_Near_Skill();

	void Update_Far_Attack();
	void Update_Far_Move();
	void Update_Far_Guard();
	void Update_Far_Rush();
	void Update_Far_Skill();


	_bool Dash_CalCul(CEnmu* pEnmu);

	CEnmuState* Near_Skill_Setting(CEnmu* pEnmu);
	CEnmuState* Return_AIState(CEnmu* pEnmu);


private:
	STATE_ID ePreState = CEnmuState::STATE_END;
	_bool	 m_bNextAnim = false;

	TARGET_STATE m_eTargetState = STATE_END;


	AI_RANGE	 m_eRange = RANGE_END;
	AI_DIR		 m_eDir;
	AI_STATE	 m_eState = AI_END;
	AI_STATE	 m_ePreState = AI_END;
	AI_DASHPOINT m_eDashDir;

	_float		m_fDelay = 0.f;
	_float		m_fAIPatternDelay = 0.f;
	_float		m_fTime = 0.f;
	_uint		m_iTargetState = 0;
	_float		m_fDashDelay = 0.f;
};
END
END

