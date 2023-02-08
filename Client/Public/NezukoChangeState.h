#pragma once

#include "NezukoState.h"
BEGIN(Client)
BEGIN(Nezuko)
class CChangeState : public CNezukoState
{
public:
	CChangeState(STATE_TYPE eType);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


	CNezukoState* Increase_Height(CNezuko* pNezuko, _float fTimeDelta);
	CNezukoState* Fall_Height(CNezuko* pNezuko, _float fTimeDelta);


private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_float m_fGravity;
	_bool m_bNextAnim = false;
	_bool m_bEffect = false;
};
END
END

