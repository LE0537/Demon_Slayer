#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Nezuko)
class CJumpSkill_MoveState : public CNezukoState
{
public:
	CJumpSkill_MoveState(STATE_TYPE eType);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;

	void Jump(CNezuko* pNezuko, _float fTimeDelta);
	void Move(CNezuko* pNezuko, _float fTimeDelta);
	void Initialize_value(CNezuko* pNezuko);
	void Initialize_MoveValue(CNezuko* pNezuko);
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

