#pragma once

#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)
class CUpperHitState : public CShinobuState
{
public:
	CUpperHitState(CShinobu::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime = 0.035f);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


private:
	void Increase_Height(CShinobu * pShinobu, _float fTimeDelta);
	void Fall_Height(CShinobu * pShinobu, _float fTimeDelta);

	void KnockBack_Player(CShinobu* pShinobu, _float fTimeDelta);
	void Bound_Player(CShinobu* pShinobu, _float fTimeDelta);
	void Upper_Player(CShinobu* pShinobu, _float fTimeDelta);

	CShinobuState* KnockBackState(CShinobu* pShinobu, _float fTimeDelta);
	CShinobuState* BoundState(CShinobu* pShinobu, _float fTimeDelta);
	CShinobuState* UpperState(CShinobu* pShinobu, _float fTimeDelta);
	CShinobuState* Upper2State(CShinobu* pShinobu, _float fTimeDelta);
private:
	CShinobu::HIT_TYPE m_eHitType = CShinobu::HIT_TYPE::HIT_END;
	_float m_fBoundPower = 0.f;
	_float m_fJumpPower = 0.f;
	_float m_fKnockBackPower = 0.f;

private:
	_bool m_bHitPlayerJump = false;
	_bool m_bNextAnim = false;
	_bool m_bReset = false;
	_bool m_bMaxHeight = false;

private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fCurrentPosY = 0.f;
	_float m_fJumpTime = 0.f;
	_float m_fDelay = 0.f;

};
END
END