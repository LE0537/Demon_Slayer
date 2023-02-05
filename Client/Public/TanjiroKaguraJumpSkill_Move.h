#pragma once

#include "TanjiroState.h"
#include "RuiAtk.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CKaguraJumpSkill_MoveState : public CTanjiroState
{
public:
	CKaguraJumpSkill_MoveState();

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

	void Move(CTanjiro* pTanjiro, _float fTimeDelta);
	void Jump(CTanjiro* pTanjiro, _float fTimeDelta);
	void Initialize_value(CTanjiro * pTanjiro);

private:
	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
};
END
END

