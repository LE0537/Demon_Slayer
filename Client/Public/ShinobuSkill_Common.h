#pragma once

#include "ShinobuState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Shinobu)
class CSkill_CommonState : public CShinobuState
{
public:
	CSkill_CommonState();

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;

	virtual CShinobuState* CommandCheck(CShinobu* pShinobu) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;

	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_float4 m_vLook;
	_bool   m_bLook = false;

	_bool	m_bEffect = false;
};
END
END

