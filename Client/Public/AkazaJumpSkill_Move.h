#pragma once

#include "AkazaState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Akaza)
class CJumpSkill_MoveState : public CAkazaState
{
public:
	CJumpSkill_MoveState(STATE_TYPE eType);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

	void Jump(CAkaza* pAkaza, _float fTimeDelta);
	void Move(CAkaza* pAkaza, _float fTimeDelta);
	void Initialize_value(CAkaza * pAkaza);
	void Initialize_MoveValue(CAkaza* pAkaza);
private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_bool m_bJump = false;


	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vMovePosition;
	_float3 m_vMoveVelocity;
	_float m_fMoveOriginPosY;
};
END
END

