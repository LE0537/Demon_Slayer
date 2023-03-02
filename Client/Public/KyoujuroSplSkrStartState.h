#pragma once

#include "KyoujuroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Kyoujuro)


class CSplSkrStartState : public CKyoujuroState
{
public:
	CSplSkrStartState(STATE_TYPE eType);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;

private:
	void Move(CKyoujuro* pKyoujuro, _float fTimeDelta);

private:
	_bool m_bCollision = false;
	CBaseAtk*	m_pCollBox = nullptr;

	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;
	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_bool  m_bEffect = false;
};

END
END