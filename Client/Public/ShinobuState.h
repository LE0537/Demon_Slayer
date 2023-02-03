#pragma once

#include "Engine_Defines.h"
#include "Shinobu.h"


BEGIN(Client)
class CShinobuState
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
		STATE_CHANGE,
		STATE_SKILL_COMMON,
		STATE_SKILL_MOVE,
		STATE_SKILL_MOVE_ALPHA,
		STATE_SKILL_UPPER,
		STATE_END
	};


public:
	virtual ~CShinobuState() {};
	virtual CShinobuState* HandleInput(CShinobu* pShinobu) { return nullptr; };
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) { return nullptr; };
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CShinobu* pShinobu) {};
	virtual void Exit(CShinobu* pShinobu) {};

	CShinobuState* ChangeState(CShinobu* pShinobu, CShinobuState* pCurrentState, CShinobuState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pShinobu);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pShinobu);

		return pCurrentState;
	}
	
public:
	STATE_ID Get_ShinobuState() const { return m_eStateId; }


protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	STATE_ID m_ePreState = STATE_END;
};

END