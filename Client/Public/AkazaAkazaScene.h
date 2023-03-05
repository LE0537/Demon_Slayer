#pragma once

#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)


class CAkazaAkazaScene : public CAkazaState
{
public:
	CAkazaAkazaScene(STATE_TYPE eType);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

	void Fall_Height(CAkaza * pAkaza, _float fTimeDelta);


private:
	_float m_fSceneTimer = 0.f;
	_float m_fOriginPosY = 0.f;
	_bool m_bNextAnim = false;
	_bool m_bControl = false;


	
};

END
END