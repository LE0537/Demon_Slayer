#pragma once
#include "ShinobuState.h"


BEGIN(Client)
BEGIN(Shinobu)
class CTargetRushState : public CShinobuState
{
public:
	CTargetRushState(STATE_TYPE eType);
	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;

	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

	void Move(CShinobu* pShinobu, _float fTimeDelta);
	void Initialize_value(CShinobu* pShinobu);

private:
	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_bool m_bRange = false;


};

END
END