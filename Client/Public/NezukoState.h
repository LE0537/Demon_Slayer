#pragma once

#include "Engine_Defines.h"
#include "Nezuko.h"


BEGIN(Client)
class CNezukoState
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
		STATE_SKILL_COMMON,
		STATE_CHANGE,
		STATE_END
	};


public:
	virtual ~CNezukoState() {};
	virtual CNezukoState* HandleInput(CNezuko* pNezuko) { return nullptr; };
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) { return nullptr; };
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CNezuko* pNezuko) {};
	virtual void Exit(CNezuko* pNezuko) {};

	CNezukoState* ChangeState(CNezuko* pNezuko, CNezukoState* pCurrentState, CNezukoState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pNezuko);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pNezuko);

		return pCurrentState;
	}
	
public:
	STATE_ID Get_NezukoState() const { return m_eStateId; }


protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	STATE_ID m_ePreState = STATE_END;
};

END