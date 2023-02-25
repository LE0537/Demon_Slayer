#pragma once
#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)

class CSkill3_EnmuState : public CEnmuState
{
public:
	CSkill3_EnmuState(STATE_TYPE eType);
	
	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;


	void Increase_Height(CEnmu* pEnmu, _float fTimeDelta);
	void Decrease_Height(CEnmu* pEnmu, _float fTimeDelta);

private:
	_bool m_bNextAnim = false;
	_bool m_bIncreaseHeight = false;
	_float3 m_vVelocity;
	_float3 m_vPosition;
	_float m_fOriginPosY = 0.f;
};

END
END