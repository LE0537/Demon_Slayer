#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Nezuko)
class CSkill_CommonState : public CNezukoState
{
public:
	CSkill_CommonState(STATE_TYPE eType);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* CommandCheck(CNezuko* pNezuko) override;

	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;
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

	_float m_fComboDelay = 0.f;
};
END
END

