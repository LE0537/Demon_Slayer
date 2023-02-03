#pragma once
#include "AkazaState.h"


BEGIN(Client)
BEGIN(Akaza)
class CJumpMoveAttackState : public CAkazaState
{
public:
	CJumpMoveAttackState(STATE_TYPE eType);
	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;

	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

	void Jump(CAkaza* pAkaza, _float fTimeDelta);
	void Initialize_value(CAkaza* pAkaza);

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