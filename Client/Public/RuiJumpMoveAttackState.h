#pragma once
#include "RuiState.h"


BEGIN(Client)
BEGIN(Rui)
class CJumpMoveAttackState : public CRuiState
{
public:
	CJumpMoveAttackState(STATE_TYPE eType);
	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;

	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;

	void Jump(CRui* pRui, _float fTimeDelta);
	void Initialize_value(CRui* pRui);

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