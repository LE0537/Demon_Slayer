#pragma once

#include "AkazaState.h"


BEGIN(Client)
BEGIN(Akaza)

class CAkazaAIState : public CAkazaState
{
public:
	enum TARGET_STATE { STATE_IDLE, STATE_MOVE, STATE_JUMP, STATE_ATTACK, STATE_SKILL, STATE_GUARD, STATE_HIT, STATE_RUSH, STATE_END };
	enum AI_STATE {AI_IDLE, AI_BACKMOVE, AI_FRONTMOVE, AI_ATTACK, AI_GUARD, AI_SKILL, NEAR_AI_SKILL_1, NEAR_AI_SKILL_2, NEAR_AI_SKILL_3, 
		AI_DASH, AI_DASH_F, AI_DASH_B, AI_DASH_L, AI_DASH_R, AI_JUMPATK, AI_JUMP, AI_RUSH, AI_HIT, AI_ADV_SKILL_0, AI_ADV_SKILL_1, AI_END};
	enum AI_RANGE { RANGE_IN, RANGE_NEAR, RANGE_FAR, RANGE_OUT, RANGE_END };
	


public:
	CAkazaAIState();

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;


private:
	void Update_TargetState(CAkaza* pAkaza);
	void Update_AI_Near(CAkaza* pAkaza);
	void Update_AI_Far(CAkaza* pAkaza);
	void Update_AI_Out(CAkaza* pAkaza);

	void Update_NearAttack();
	void Update_NearMove();
	void Update_NearGuard();
	void Update_NearRush();
	void Update_NearSkill();

	void Update_Far_InAttack();
	void Update_Far_InMove();
	void Update_Far_InGuard();
	void Update_Far_InRush();
	void Update_Far_InSkill();


	CAkazaState* Near_Skill_Setting(CAkaza* pAkaza);
	CAkazaState* Return_AIState(CAkaza* pAkaza);

	void Dash_CalCul(CAkaza* pAkaza);

private:
	TARGET_STATE m_eTargetState = STATE_END;
	_uint m_iTargetState = 0;
	AI_STATE	 m_eState = AI_END;
	AI_RANGE	 m_eRange = RANGE_END;
	_float		m_fDelay = 0.f;
	_float		m_fDashDelay = 0.f;

	_vector m_vOriginPosition = _vector({ 64.f, 0.f, 38.5f,1.f });
};
END
END

