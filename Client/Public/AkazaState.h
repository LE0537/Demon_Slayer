#pragma once

#include "Engine_Defines.h"
#include "Akaza.h"

BEGIN(Client)

class CAkazaState
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
		STATE_SKILL_SHOOT,
		STATE_SKILL_PUNCH,
		STATE_SKILL_DESTROY,
		STATE_CHANGE,
		STATE_ADVSKILL_COMMON,
		STATE_ADVSKILL_MOVE,
		STATE_JUMP_ATTACK,
		STATE_BATTLE_START,
		STATE_END
	};


public:
	virtual ~CAkazaState() {};
	virtual CAkazaState* HandleInput(CAkaza* pAkaza) { return nullptr; };
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) { return nullptr; };
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CAkaza* pAkaza) {};
	virtual void Exit(CAkaza* pAkaza) {};
	virtual CAkazaState* CommandCheck(CAkaza* pAkaza) { return nullptr; };
	CAkazaState* ChangeState(CAkaza* pAkaza, CAkazaState* pCurrentState, CAkazaState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pAkaza);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pAkaza);

		return pCurrentState;
	}

public:
	STATE_ID Get_AkazaState() const { return m_eStateId; }


protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	STATE_ID m_ePreState = STATE_END;
	_float m_fDuration = 0.f;
	_float m_fCurrentDuration = 0.f;
};

END
