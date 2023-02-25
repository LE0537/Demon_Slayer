#pragma once

#include "Engine_Defines.h"
#include "RuiDad.h"


BEGIN(Client)

class CRuiDadState
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
		STATE_SKILL_MOVE,
		STATE_SKILL_FALLCUT,
		STATE_CHANGE,
		STATE_JUMP_ATTACK,
		STATE_BATTLE_START,
		STATE_SKILL_THROW,
		STATE_END
	};


public:
	virtual ~CRuiDadState() {};
	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) { return nullptr; };
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) { return nullptr; };
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CRuiDad* pRuiDad) {};
	virtual void Exit(CRuiDad* pRuiDad) {};
	virtual CRuiDadState* CommandCheck(CRuiDad* pRuiDad) { return nullptr; };
	CRuiDadState* ChangeState(CRuiDad* pRuiDad, CRuiDadState* pCurrentState, CRuiDadState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pRuiDad);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pRuiDad);

		return pCurrentState;
	}

public:
	STATE_ID Get_RuiDadState() const { return m_eStateId; }


protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	STATE_ID m_ePreState = STATE_END;

	_float m_fDuration = 0.f;
	_float m_fCurrentDuration = 0.f;
};

END