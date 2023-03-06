#pragma once

#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)

class CAkaza_CinemaState : public CAkazaState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_7, SCENE_8, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_START = 109,
		ANIM_SCENE_0 = 110,
		ANIM_SCENE_1 = 111,
		ANIM_SCENE_2 = 112,
		ANIM_SCENE_3 = 113,
		ANIM_SCENE_4 = 114,
		ANIM_SCENE_5 = 115,
		ANIM_SCENE_6 = 116,
		ANIM_SCENE_7 = 117,

		ANIM_SCENE_END = 100,
	};

public:
	CAkaza_CinemaState(CINEMASCENE eScene);
	
	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

private:
	CINEMASCENE m_eScene = SCENE_END;
	_float		m_fTime = 0.f;
	_bool		m_bRenderObjects = false;

};

END
END