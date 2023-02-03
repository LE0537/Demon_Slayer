#pragma once

#include "Engine_Defines.h"
#include "Tanjiro.h"


BEGIN(Client)
class CTanjiroState
{
public:
	enum STATE_TYPE
	{
		TYPE_START,
		TYPE_LOOP,
		TYPE_END,
		TYPE_DEFAULT,
		TYPE_CHANGE
	};


	enum STATE_ID
	{
		STATE_IDLE,
		STATE_MOVE,
		STATE_JUMP,
		STATE_ATK_1,
		STATE_ATK_2,
		STATE_ATK_3,
		STATE_ATK_4,
		STATE_HIT,
		STATE_GUARD,
		STATE_GUARD_ADV,
		STATE_GUARD_HIT,
		STATE_DASH,
		STATE_SKILL_WATERMILL,
		STATE_SKILL_WINDMILL,
		STATE_SKILL_COMMON,
		STATE_SKILL_KAGURA_COMMON,
		STATE_SKILL_KAGURA_MOVE,
		STATE_SKILL_KAGURA_SPHERE,
		STATE_CHANGE,
		STATE_JUMP_ATTACK,
		STATE_BATTLE_START,
		STATE_END
	};


public:
	virtual ~CTanjiroState() {};
	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) { return nullptr; };
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) { return nullptr; };
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CTanjiro* pTanjiro) {};
	virtual void Exit(CTanjiro* pTanjiro) {};

	CTanjiroState* ChangeState(CTanjiro* pTanjiro, CTanjiroState* pCurrentState, CTanjiroState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pTanjiro);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pTanjiro);

		return pCurrentState;
	}
	
public:
	STATE_ID Get_TanjiroState() const { return m_eStateId; }


protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	STATE_ID m_ePreState = STATE_END;
};

END