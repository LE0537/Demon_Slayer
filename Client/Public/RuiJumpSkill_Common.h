#pragma once

#include "RuiState.h"


BEGIN(Client)
BEGIN(Rui)
class CJumpSkill_CommonState : public CRuiState
{
public:
	CJumpSkill_CommonState(STATE_TYPE eType);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;

	void Jump(CRui* pRui, _float fTimeDelta);
	void Initialize_value(CRui* pRui);


private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_float m_fDelay = 0.f;
private:

	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_bool m_bCombo = false;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bEffect = false;
};
END
END

