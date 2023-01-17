#pragma once
#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)

class CMoveState : public CKyoujuroState
{
public:
	CMoveState(OBJDIR eDir, STATE_TYPE eType);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;



public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CKyoujuro* pKyoujuro, _float fTimeDelta);


private:
	OBJDIR	m_eDirection = OBJDIR::DIR_END;
};
END
END