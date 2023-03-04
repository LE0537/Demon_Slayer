#pragma once

#include "Client_Defines.h"
#include "AkazaSceneState.h"


BEGIN(Client)
BEGIN(AkazaScene)

class CSceneState : public CAkazaSceneState
{
public:
	enum SCENE { SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_END };

public:
	CSceneState(SCENE eScene);

	virtual CAkazaSceneState* HandleInput(CAkazaScene* pAkazaScene);
	virtual CAkazaSceneState* Tick(CAkazaScene* pAkazaScene, _float fTimeDelta) ;
	virtual CAkazaSceneState* Late_Tick(CAkazaScene* pAkazaScene, _float fTimeDelta);

	virtual void Enter(CAkazaScene* pAkazaScene);
	virtual void Exit(CAkazaScene* pAkazaScene);


	void Fall_Height(CAkazaScene* pAkazaScene, _float fTimeDelta);



private:
	SCENE m_eScene = SCENE_END;
	_float m_fSceneTimer = 0.f;
	_float m_fOriginPosY = 0.f;
	_bool m_bNextAnim = false;
};

END
END