#pragma once
#include "EnmuState.h"

BEGIN(Client)
BEGIN(Enmu)

class CSkill1_EnmuState : public CEnmuState
{
public:
	CSkill1_EnmuState(STATE_TYPE eType);
	
	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;


	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;

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