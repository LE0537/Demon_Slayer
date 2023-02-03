#pragma once
#include "TanjiroState.h"


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


};

END
END