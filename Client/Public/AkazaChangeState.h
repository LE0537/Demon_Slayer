#pragma once

#include "AkazaState.h"
BEGIN(Client)
BEGIN(Akaza)
class CChangeState : public CAkazaState
{
public:
	CChangeState(STATE_TYPE eType);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;


	CAkazaState* Increase_Height(CAkaza* pAkaza, _float fTimeDelta);
	CAkazaState* Fall_Height(CAkaza* pAkaza, _float fTimeDelta);


private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_float m_fGravity;
	_bool m_bNextAnim = false;
};
END
END

