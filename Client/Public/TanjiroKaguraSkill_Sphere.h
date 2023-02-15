#pragma once

#include "TanjiroState.h"
#include "KaguraSkill.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CKaguraSkill_SphereState : public CTanjiroState
{
public:
	CKaguraSkill_SphereState(STATE_TYPE eType = TYPE_START);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* CommandCheck(CTanjiro* pTanjiro) override;

private:
	void Fall_Height(CTanjiro* pTanjiro, _float fTimeDelta);
private:
	CKaguraSkill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_int   m_iHit = 0;
	_bool   m_bLook = false;
	_bool m_bNextAnim = false;
	_float4 m_vLook;
private:
	_vector m_vTargetPosition;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fGravity;
	_float m_fHeight;
	_float m_fOriginPosY;

	_bool  m_bEffect = false;
	_bool  m_bEffect2 = false;
	_bool  m_bTrue = false;
};
END
END

