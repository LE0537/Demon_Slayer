#pragma once
#include "AkazaState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Akaza)
class CJumpAttackState : public CAkazaState
{
public:
	CJumpAttackState(STATE_TYPE eType);
	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;

	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;

	void Jump(CAkaza* pAkaza, _float fTimeDelta);
	void Initialize_value(CAkaza* pAkaza);
private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;

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