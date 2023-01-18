#pragma once

#include "Engine_Defines.h"
#include "Kyoujuro.h"

BEGIN(Client)
class CKyoujuroState
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
		STATE_END
	};


public:
	virtual ~CKyoujuroState() {};
	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) { return nullptr; };
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) { return nullptr; };
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CKyoujuro* pKyoujuro) {};
	virtual void Exit(CKyoujuro* pKyoujuro) {};

	CKyoujuroState* ChangeState(CKyoujuro* pKyoujuro, CKyoujuroState* pCurrentState, CKyoujuroState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pKyoujuro);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pKyoujuro);

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