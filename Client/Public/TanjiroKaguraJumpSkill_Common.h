#pragma once

#include "TanjiroState.h"
#include "RuiAtk.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CKaguraJumpSkill_CommonState : public CTanjiroState
{
public:
	CKaguraJumpSkill_CommonState();

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

	void Jump(CTanjiro* pTanjiro, _float fTimeDelta);
	void Initialize_value(CTanjiro * pTanjiro);

private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
private:
	CRuiAtk*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_int   m_iHit = 0;
	_bool   m_bLook = false;
	_float4 m_vLook;


};
END
END

