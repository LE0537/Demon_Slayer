#pragma once
#include "EnmuState.h"


BEGIN(Client)
BEGIN(Enmu)
class CTargetRushState : public CEnmuState
{
public:
	CTargetRushState(STATE_TYPE eType);
	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;

	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;

	void Move(CEnmu* pEnmu, _float fTimeDelta);
	void Initialize_value(CEnmu* pEnmu);

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