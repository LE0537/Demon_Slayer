#pragma once

#include "ShinobuState.h"
BEGIN(Client)
BEGIN(Shinobu)
class CChangeState : public CShinobuState
{
public:
	CChangeState(STATE_TYPE eType);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


	CShinobuState* Increase_Height(CShinobu* pShinobu, _float fTimeDelta);
	CShinobuState* Fall_Height(CShinobu* pShinobu, _float fTimeDelta);


private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_float m_fGravity;
	_bool m_bNextAnim = false;
	_bool m_bEffect = false;
};
END
END

