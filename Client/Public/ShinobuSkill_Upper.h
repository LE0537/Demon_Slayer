#pragma once

#include "ShinobuState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Shinobu)
class CSkill_UpperState : public CShinobuState
{
public:
	CSkill_UpperState(STATE_TYPE eType);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


	void Move(CShinobu* pShinobu, _float fTimeDelta);

	CShinobuState* Increase_Height(CShinobu* pShinobu, _float fTimeDelta);
	CShinobuState* Fall_Height(CShinobu* pShinobu, _float fTimeDelta);
private:
	CBaseAtk*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;

	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_float4 m_vLook;
	_bool   m_bLook = false;



private:
	_vector m_vTargetPosition;
	_bool m_bNextAnim = false;
	_float m_fCurrentPosY;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_float m_fGravity;
	_float3 m_vMove;
};
END
END

