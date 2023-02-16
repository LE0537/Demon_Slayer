#pragma once

#include "RuiState.h"
#include <queue>

BEGIN(Client)
BEGIN(Rui)
class CAiState : public CRuiState
{
public:
	enum TARGET_STATE { STATE_IDLE, STATE_MOVE, STATE_JUMP, STATE_ATTACK, STATE_SKILL, STATE_GUARD, STATE_HIT, STATE_RUSH, STATE_END};
	enum AI_STATE {AI_IDLE, AI_BACKMOVE ,AI_FRONTMOVE, AI_ATTACK, AI_GUARD, AI_SKILL, NEAR_AI_SKILL_1, NEAR_AI_SKILL_2, NEAR_AI_SKILL_3
		,AI_DASH ,AI_DASH_F, AI_DASH_B, AI_DASH_L, AI_DASH_R , AI_JUMPATK ,AI_JUMP , AI_RUSH,AI_HIT , AI_ADV_SKILL_0, AI_ADV_SKILL_1, AI_END};
	enum AI_DASHPOINT { DASH_F, DASH_L, DASH_R};
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
	void Update_AI_Out(CRui* pRui); 

	CRuiState* Return_AIState(CRui* pRui);

private:
	// RANGE IN
	// 루이 기준 near일때 플레이어 상태
	void Update_NearAttack();
	void Update_NearMove();
	void Update_NearGuard();
	void Update_NearRush();
	void Update_NearSkill();

	// 루이 기준 Far일때 플레이어 상태
	_bool Far_CompareOriginPoint(CRui* pRui); // 생성 지점과 거리 비교
	// In -> 지점과 가까운 경우
	void Update_Far_InAttack();
	void Update_Far_InMove();
	void Update_Far_InGuard();
	void Update_Far_InRush(); 
	void Update_Far_InSkill();
	// Out -> 지점과 먼 경우
	void Update_Far_OutAttack();
	void Update_Far_OutMove();
	void Update_Far_OutGuard();
	void Update_Far_OutRush();
	void Update_Far_OutSkill();

private:
	void Near_Skill_Setting(CRui* pRui);


private:
	void Dash_Setting(CRui* pRui);
	void DashDir_Calcul(CRui* pRui);
	void Compare_OriginPoint(CRui* pRui);


	
	
	
private:
	TARGET_STATE m_eTargetState = STATE_END;
	

	AI_RANGE	 m_eRange = RANGE_END;
	AI_DIR		 m_eDir;
	AI_STATE	 m_eState = AI_END;
	AI_STATE	 m_ePreState = AI_END;
	AI_DASHPOINT m_eDashDir;

	_float		m_fDelay = 0.f;
	_float		m_fAIPatternDelay = 0.f;

	_bool		m_bNextRound = false;

	_uint m_iTargetState = 0;


	queue<_uint> m_queueDash;
	_vector m_vOriginPosition = _vector({ 64.f, 0.f, 38.5f,1.f });
};

END
END