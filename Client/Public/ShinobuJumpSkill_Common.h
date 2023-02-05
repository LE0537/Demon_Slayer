#pragma once
#include "ShinobuState.h"
#include "WindMill.h"

BEGIN(Client)
BEGIN(Shinobu)
class CJumpCommonSkillState : public CShinobuState
{
public:
	CJumpCommonSkillState(STATE_TYPE eType);
	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;

	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

	void Jump(CShinobu* pShinobu, _float fTimeDelta);
	void Initialize_value(CShinobu* pShinobu);

private:
	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_bool m_bRange = false;
private:
	CWindMill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_float4 m_vLook;
	_bool   m_bLook = false;
	_bool  m_bEffect = false;
	_int   m_iHit = 0;

};

END
END