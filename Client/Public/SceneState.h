#pragma once

#include "Client_Defines.h"
#include "AkazaSceneState.h"


BEGIN(Client)
BEGIN(AkazaScene)

class CSceneState : public CAkazaSceneState
{
public:
	enum SCENE { SCENE_0, SCENE_1, SCENE_END };

public:
	CSceneState(STATE_TYPE eType);

	virtual CAkazaSceneState* HandleInput(CAkazaScene* pAkazaScene);
	virtual CAkazaSceneState* Tick(CAkazaScene* pAkazaScene, _float fTimeDelta) ;
	virtual CAkazaSceneState* Late_Tick(CAkazaScene* pAkazaScene, _float fTimeDelta);

	virtual void Enter(CAkazaScene* pAkazaScene);
	virtual void Exit(CAkazaScene* pAkazaScene);
};

END
END