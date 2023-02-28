#pragma once

#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)
class CUpperHitState : public CRuiState
{
public:
	CUpperHitState(CRui::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime = 0.035f);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


private:
	void Increase_Height(CRui* pRui, _float fTimeDelta);
	void Fall_Height(CRui* pRui, _float fTimeDelta);

	void KnockBack_Player(CRui* pRui, _float fTimeDelta);
	void Bound_Player(CRui* pRui, _float fTimeDelta);
	void Upper_Player(CRui* pRui, _float fTimeDelta);

	CRuiState* KnockBackState(CRui* pRui, _float fTimeDelta);
	CRuiState* BoundState(CRui* pRui, _float fTimeDelta);
	CRuiState* UpperState(CRui* pRui, _float fTimeDelta);
	CRuiState* Upper2State(CRui* pRui, _float fTimeDelta);
private:
	CRui::HIT_TYPE m_eHitType = CRui::HIT_TYPE::HIT_END;
	_float m_fBoundPower = 0.f;
	_float m_fJumpPower = 0.f;
	_float m_fKnockBackPower = 0.f;

private:
	_bool m_bHitPlayerJump = false;
	_bool m_bNextAnim = false;
	_bool m_bReset = false;
	_bool m_bMaxHeight = false;

	_bool m_bTrue = false;
private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fCurrentPosY = 0.f;
	_float m_fJumpTime = 0.f;
	_float m_fDelay = 0.f;

};
END
END