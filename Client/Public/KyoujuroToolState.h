#pragma once

#include "KyoujuroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CToolState : public CKyoujuroState
{
public:
	CToolState(_uint iAnimIndex, _uint iAnimIndex_2 = 0, _uint iAnimIndex_3 = 0, STATE_TYPE eType = TYPE_START, _bool bIsContinue = false);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pkyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pkyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pkyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pTanjiro) override;
	virtual void Exit(CKyoujuro* pTanjiro) override;


private:
	_uint m_iAnimIndex = 0;
	_uint m_iAnimIndex_Second = 0;
	_uint m_iAnimIndex_Third = 0;

	STATE_TYPE m_eStateType = STATE_TYPE::TYPE_DEFAULT;
	_bool m_bIsContinue = false;
};
END
END

