#pragma once
#include "TanjiroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CJumpMoveAttackState : public CTanjiroState
{
public:
	CJumpMoveAttackState(STATE_TYPE eType);
	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;

	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

	void Jump(CTanjiro* pTanjiro, _float fTimeDelta);
	void Initialize_value(CTanjiro * pTanjiro);

private:
	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_bool m_bRange = false;

private:
	CBaseAtk*	m_pCollBox = nullptr;

	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;
	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_bool  m_bEffect = false;
};

END
END