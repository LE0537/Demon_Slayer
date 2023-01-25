#pragma once

#include "KyoujuroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CSkill_DashSlashState : public CKyoujuroState
{
public:
	CSkill_DashSlashState();

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;

};
END
END

