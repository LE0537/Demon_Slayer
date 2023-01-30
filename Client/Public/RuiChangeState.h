#pragma once

#include "RuiState.h"
BEGIN(Client)
BEGIN(Rui)
class CChangeState : public CRuiState
{
public:
	CChangeState(STATE_TYPE eType);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


	CRuiState* Increase_Height(CRui* pRui, _float fTimeDelta);
	CRuiState* Fall_Height(CRui* pRui, _float fTimeDelta);


private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_float m_fGravity;
	_bool m_bNextAnim = false;
};
END
END

