#pragma once

#include "RuiDadState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(RuiDad)
class CSkill_RushState : public CRuiDadState
{
public:
	CSkill_RushState(STATE_TYPE eType);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;
	void Move(CRuiDad* pRuiDad, _float fTimeDelta);
	void Initialize_value(CRuiDad* pRuiDad);
private:
	CBaseAtk*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bEffect = false;

private:
	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_bool m_bRange = false;
	_float EffectTime = 0.f;
};
END
END

