#pragma once
#include "NezukoState.h"


BEGIN(Client)
BEGIN(Nezuko)
class CTargetRushState : public CNezukoState
{
public:
	CTargetRushState(STATE_TYPE eType);
	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;

	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;

	void Move(CNezuko* pNezuko, _float fTimeDelta);
	void Initialize_value(CNezuko* pNezuko);
	virtual CNezukoState* CommandCheck(CNezuko* pNezuko) override;
private:
	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_bool m_bRange = false;
	_bool  m_bEffect = false;
	_float m_fComboDelay = 0.f;
};

END
END