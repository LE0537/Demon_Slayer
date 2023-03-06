#pragma once

#include "Engine_Defines.h"
#include "EnmuBoss.h"
#include "SoundMgr.h"

BEGIN(Client)

class CEnmuBossState
{
public:
	enum STATE_TYPE
	{
		TYPE_START,
		TYPE_LOOP,
		TYPE_END,
		TYPE_DEFAULT,
		TYPE_CHANGE,
		TYPE_ADD,
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
		STATE_RUSH,
		STATE_SKILL1,
		STATE_SKILL2,
		STATE_SKILL3,
		STATE_END
	};


public:
	virtual ~CEnmuBossState() {};
	virtual CEnmuBossState* HandleInput(CEnmuBoss* pEnmuBoss) { return nullptr; };
	virtual CEnmuBossState* Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) { return nullptr; };
	virtual CEnmuBossState* Late_Tick(CEnmuBoss* pEnmuBoss, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CEnmuBoss* pEnmuBoss) {};
	virtual void Exit(CEnmuBoss* pEnmuBoss) {};
	
	CEnmuBossState* ChangeState(CEnmuBoss* pEnmuBoss, CEnmuBossState* pCurrentState, CEnmuBossState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pEnmuBoss);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pEnmuBoss);

		return pCurrentState;
	}

public:
	STATE_ID Get_EnmuBossState() const { return m_eStateId; }


protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	STATE_ID m_ePreState = STATE_END;

	_float m_fDuration = 0.f;
	_float m_fCurrentDuration = 0.f;
};

END