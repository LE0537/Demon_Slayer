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
		TYPE_DEFAULT
	};


	enum STATE_ID
	{
		STATE_IDLE,
		STATE_MOVE,
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

protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;
};

END