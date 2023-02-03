#pragma once

#include "AkazaState.h"
#include "BaseAtk.h"
#include "Camera_Dynamic.h"
BEGIN(Client)
BEGIN(Akaza)
class CAdvSkill_CommmonState : public CAkazaState
{
public:
	CAdvSkill_CommmonState(STATE_TYPE eType);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

	CAkazaState* Increase_Height(CAkaza* pAkaza, _float fTimeDelta);
	CAkazaState* Fall_Height(CAkaza* pAkaza, _float fTimeDelta);

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
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fGravity;
	_float m_fHeight;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_float m_fDistance;
	_bool m_bRange = false;
	CCamera_Dynamic* m_pCamera = nullptr;
};
END
END

