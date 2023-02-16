#pragma once

#include "RuiDadState.h"


BEGIN(Client)
BEGIN(RuiDad)
class CSkill_JumpDropState : public CRuiDadState
{
public:
	CSkill_JumpDropState(STATE_TYPE eType);

	virtual CRuiDadState* HandleInput(CRuiDad* pRuiDad) override;
	virtual CRuiDadState* Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;
	virtual CRuiDadState* Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta) override;


	virtual void Enter(CRuiDad* pRuiDad) override;
	virtual void Exit(CRuiDad* pRuiDad) override;

	CRuiDadState* Increase_Height(CRuiDad* pRuiDad, _float fTimeDelta);
	CRuiDadState* Fall_Height(CRuiDad* pRuiDad, _float fTimeDelta);
	void Create_TargetCircle(CRuiDad* pRuiDad, _float fTimeDelta);


private:

	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bEffect = false;

	_bool m_bNextAnim = false;
	_bool m_bIncreaseHeight = false;
	_float3 m_vVelocity;
	_float3 m_vPosition;
	_float m_fOriginPosY = 0.f;
	_bool m_bCreateTargetCircle = false;
	_float m_fFallTime = 0.f;
	_float3 m_vTempPosition;

};
END
END

