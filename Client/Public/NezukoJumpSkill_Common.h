#pragma once

#include "NezukoState.h"
#include "WindMill.h"


BEGIN(Client)
BEGIN(Nezuko)
class CJumpSkill_CommmonState : public CNezukoState
{
public:
	CJumpSkill_CommmonState(STATE_TYPE eType, _uint iCount = 0);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


	void Jump(CNezuko* pNezuko, _float fTimeDelta);
	void Initialize_value(CNezuko* pNezuko);

private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_uint m_iCount = 0;
	_float m_fNextAnim = 0.f;
	_bool	m_bEffect = false;
private:
	CWindMill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_float4 m_vLook;
	_bool   m_bLook = false;
	_int   m_iHit = 0;
};
END
END

