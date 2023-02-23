#pragma once
#include "ShinobuState.h"


BEGIN(Client)
BEGIN(Shinobu)


class CShinobu_CinemaState : public CShinobuState
{
public:
	enum CINEMASCENE { SCENE_START, SCENE_0, SCENE_1, SCENE_2, SCENE_3, SCENE_4, SCENE_5, SCENE_6, SCENE_END };
	enum SCENE_ANIM
	{
		ANIM_SCENE_START = 139,
		ANIM_SCENE_0 = 109,
		ANIM_SCENE_1 = 110,
		ANIM_SCENE_2 = 111,
		ANIM_SCENE_3 = 112,

		ANIM_SCENE_END = 100
	};
public:
	CShinobu_CinemaState(CINEMASCENE eScene);
	
	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

	

private:
	CINEMASCENE m_eScene = SCENE_END;

};

END
END