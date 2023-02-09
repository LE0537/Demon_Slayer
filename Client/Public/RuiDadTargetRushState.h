#pragma once
#include "RuiDadState.h"


BEGIN(Client)
BEGIN(RuiDad)
class CTargetRushState : public CRuiDadState
{
public:
	CTargetRushState(STATE_TYPE eType);
	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;

	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;

	void Move(CRuiDad* pRuiDad, _float fTimeDelta);
	void Initialize_value(CRuiDad* pRuiDad);

private:
	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_bool m_bRange = false;


};

END
END