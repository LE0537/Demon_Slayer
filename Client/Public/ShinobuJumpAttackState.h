#pragma once
#include "ShinobuState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Shinobu)
class CJumpAttackState : public CShinobuState
{
public:
	CJumpAttackState(STATE_TYPE eType);
	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;

	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

	void Jump(CShinobu* pShinobu, _float fTimeDelta);
	void Initialize_value(CShinobu* pShinobu);
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
	_bool  m_bJumpEffect = false;
};

END
END