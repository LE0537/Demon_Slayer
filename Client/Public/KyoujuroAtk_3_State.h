#pragma once

#include "KyoujuroState.h"
#include "BaseAtk.h"
BEGIN(Client)
BEGIN(Kyoujuro)
class CAtk_3_State : public CKyoujuroState
{
public:
	CAtk_3_State();

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;

	virtual CKyoujuroState* CommandCheck(CKyoujuro* pKyoujuro) override;
private:
	CBaseAtk*	m_pCollBox = nullptr;
	_bool m_bAtkCombo = false;
	_float m_fTime = 0.f;
	_float m_fComboDelay = 0.f;

	_float m_fMove = 0.f;
	_bool  m_bHit = false;
	_int   m_iHit = 0;
	_bool  m_bEffect = false;
	_bool m_bIsCreate = false;
};
END
END

