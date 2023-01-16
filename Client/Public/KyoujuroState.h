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
		TYPE_DEFAULT
	};


	enum STATE_ID
	{
		STATE_IDLE,
		STATE_MOVE,
		STATE_JUMP,
		STATE_END
	};


public:
	virtual ~CKyoujuroState() {};
	virtual CKyoujuroState* HandleInput(CKyoujuro* pTanjiro) { return nullptr; };
	virtual CKyoujuroState* Tick(CKyoujuro* pTanjiro, _float fTimeDelta) { return nullptr; };
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pTanjiro, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CKyoujuro* pTanjiro) {};
	virtual void Exit(CKyoujuro* pTanjiro) {};

	CKyoujuroState* ChangeState(CKyoujuro* pTanjiro, CKyoujuroState* pCurrentState, CKyoujuroState* pNewState)
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