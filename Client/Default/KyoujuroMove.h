#pragma once
#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)

class CMoveState : public CKyoujuroState
{
public:
	CMoveState();

	virtual CKyoujuroState* HandleInput(CKyoujuro* pTanjiro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pTanjiro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pTanjiro) override;
	virtual void Exit(CKyoujuro* pTanjiro) override;
};
END
END