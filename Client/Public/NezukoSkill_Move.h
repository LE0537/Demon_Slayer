#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Nezuko)
class CSkill_MoveState : public CNezukoState
{
public:
	CSkill_MoveState(STATE_TYPE eType);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


private:
	void Fall_Height(CNezuko* pNezuko, _float fTimeDelta);
	void Increase_Height(CNezuko* pNezuko, _float fTimeDelta);

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
	_bool	m_bDist = false;
private:
	_vector m_vTargetPosition;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fGravity;
	_float m_fHeight;
	_float m_fOriginPosY;
	_bool m_bNextAnim;
};
END
END

