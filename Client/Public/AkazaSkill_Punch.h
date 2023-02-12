#pragma once

#include "AkazaState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Akaza)
class CSkill_PunchState : public CAkazaState
{
public:
	CSkill_PunchState(STATE_TYPE eType);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;
	virtual CAkazaState* CommandCheck(CAkaza* pAkaza) override;
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

