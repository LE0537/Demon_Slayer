#pragma once

#include "Engine_Defines.h"
#include "Rui.h"

BEGIN(Client)

class CRuiState
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
		STATE_SKILL_SHOOTNET,
		STATE_SKILL_SHOOTCOMMON,
		STATE_SKILL_SPHERE,
		STATE_ADVSKILL_COMMON,
		STATE_ADVSKILL_MOVE,
		STATE_CHANGE,
		STATE_JUMP_ATTACK,
		STATE_BATTLE_START,
		STATE_END
	};


public:
	virtual ~CRuiState() {};
	virtual CRuiState* HandleInput(CRui* pRui) { return nullptr; };
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) { return nullptr; };
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CRui* pRui) {};
	virtual void Exit(CRui* pRui) {};
	virtual CRuiState* CommandCheck(CRui* pRui) { return nullptr; };
	CRuiState* ChangeState(CRui* pRui, CRuiState* pCurrentState, CRuiState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pRui);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pRui);

		return pCurrentState;
	}

public:
	STATE_ID Get_RuiState() const { return m_eStateId; }


protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	STATE_ID m_ePreState = STATE_END;
	_float m_fDuration = 0.f;
	_float m_fCurrentDuration = 0.f;
};

END
