#pragma once

#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CDashState : public CKyoujuroState
{
public:
	CDashState(OBJDIR eDir, _bool bSecondDash = false, _bool bJump = false);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;


private:
	void Move(CKyoujuro* pKyoujuro, _float fTimeDelta);
	void Check_Coll(CKyoujuro* pKyoujuro, _float fTimeDelta);
private:
	OBJDIR m_eDir = OBJDIR::DIR_END;
	_bool m_bJump = false;
	_bool m_bSecondDash = false;
	_float m_fDashTime = 1.f;
	_float m_fSpeed = 0.f;
	_float m_fTime = 0.f;
	_int   iIndex = 0;
	_bool  m_bTrue = false;
};
END
END