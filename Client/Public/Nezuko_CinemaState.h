#pragma once

#include "NezukoState.h"


BEGIN(Client)
BEGIN(Nezuko)


class CNezuko_CinemaState : public CNezukoState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_7, SCENE_8, SCENE_9, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_START = 114,
		ANIM_SCENE_0 = 115,
		ANIM_SCENE_1 = 116,
		ANIM_SCENE_2 = 117,
		ANIM_SCENE_3 = 118,
		ANIM_SCENE_4 = 119, 
		ANIM_SCENE_5 = 120,
		ANIM_SCENE_6 = 121,
		ANIM_SCENE_7 = 122,
		ANIM_SCENE_8 = 123,
		ANIM_SCENE_9 = 124,
	};



public:
	CNezuko_CinemaState(CINEMASCENE eScene);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


private:
	CINEMASCENE m_eScene = SCENE_END;


};

END
END