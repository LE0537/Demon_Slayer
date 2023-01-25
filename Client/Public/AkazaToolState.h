#pragma once

#include "AkazaState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Akaza)
class CToolState : public CAkazaState
{
public:
	CToolState(_uint iAnimIndex, _uint iAnimIndex_2 = 0, _uint iAnimIndex_3 = 0, STATE_TYPE eType = TYPE_START, _bool bIsContinue = false);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;


private:
	_uint m_iAnimIndex = 0;
	_uint m_iAnimIndex_Second = 0;
	_uint m_iAnimIndex_Third = 0;

	STATE_TYPE m_eStateType = STATE_TYPE::TYPE_DEFAULT;
	_bool m_bIsContinue = false;
};
END
END

