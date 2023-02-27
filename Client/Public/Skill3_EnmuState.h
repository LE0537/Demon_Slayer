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
private:
	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fDelay = 0.f;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_bool  m_bEffect = false;
};

END
END