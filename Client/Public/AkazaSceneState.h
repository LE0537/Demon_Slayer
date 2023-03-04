#pragma once

#include "Engine_Defines.h"
#include "AkazaScene.h"


BEGIN(Client)

class CAkazaSceneState
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
	virtual ~CAkazaSceneState() {};
	virtual CAkazaSceneState* HandleInput(CAkazaScene* pAkazaScene) { return nullptr; };
	virtual CAkazaSceneState* Tick(CAkazaScene* pAkazaScene, _float fTimeDelta) { return nullptr; };
	virtual CAkazaSceneState* Late_Tick(CAkazaScene* pAkazaScene, _float fTimeDelta) { return nullptr; };

	virtual void Enter(CAkazaScene* pAkazaScene) {};
	virtual void Exit(CAkazaScene* pAkazaScene) {};
	
	CAkazaSceneState* ChangeState(CAkazaScene* pAkazaScene, CAkazaSceneState* pCurrentState, CAkazaSceneState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit(pAkazaScene);
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter(pAkazaScene);

		return pCurrentState;
	}

public:
	STATE_ID Get_AkazaScene() const { return m_eStateId; }


protected:
	STATE_TYPE m_eStateType = TYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	STATE_ID m_ePreState = STATE_END;

	_float m_fDuration = 0.f;
	_float m_fCurrentDuration = 0.f;
};

END