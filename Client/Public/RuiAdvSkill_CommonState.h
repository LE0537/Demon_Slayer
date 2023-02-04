#pragma once
#include "RuiState.h"
#include "RuiSphere.h"

// u 스킬 구 생성해서 적 띄우기

BEGIN(Client)
BEGIN(Rui)
class CAdvSkill_CommonState : public CRuiState
{
public:
	CAdvSkill_CommonState();
	
	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


private:
	CRuiSphere*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;
	_bool  m_bEffect = false;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_float4 m_vTargetPos;
	_bool	m_bTargetPos = false;
};

END
END