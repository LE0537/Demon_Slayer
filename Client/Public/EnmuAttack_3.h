#pragma once

#include "EnmuState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Enmu)
class CEnmuAttack3 : public CEnmuState
{
public:
	CEnmuAttack3(STATE_TYPE eType);

	virtual CEnmuState* HandleInput(CEnmu* pEnmu) override;
	virtual CEnmuState* Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	virtual CEnmuState* Late_Tick(CEnmu* pEnmu, _float fTimeDelta) override;
	

	virtual void Enter(CEnmu* pEnmu) override;
	virtual void Exit(CEnmu* pEnmu) override;


private:
	STATE_ID ePreState = CEnmuState::STATE_END;
	_bool m_bNextAnim = false;
	CBaseAtk*	m_pCollBox = nullptr;

	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;
	_float m_fMove = 0.f;
	_int   m_iHit = 0;
	_bool  m_bHit = false;
	_bool  m_bEffect = false;
};
END
END

