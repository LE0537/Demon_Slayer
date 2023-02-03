#pragma once
#include "NezukoState.h"


BEGIN(Client)
BEGIN(Nezuko)
class CJumpAttackState : public CNezukoState
{
public:
	CJumpAttackState(STATE_TYPE eType);
	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;

	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;

	void Jump(CNezuko* pNezuko, _float fTimeDelta);
	void Initialize_value(CNezuko* pNezuko);
private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;


};

END
END