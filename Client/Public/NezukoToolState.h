#pragma once

#include "NezukoState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Nezuko)
class CToolState : public CNezukoState
{
public:
	CToolState(_uint iAnimIndex, _uint iAnimIndex_2 = 0, _uint iAnimIndex_3 = 0, STATE_TYPE eType = TYPE_START, _bool bIsContinue = false);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;


private:
	_uint m_iAnimIndex = 0;
	_uint m_iAnimIndex_Second = 0;
	_uint m_iAnimIndex_Third = 0;

	STATE_TYPE m_eStateType = STATE_TYPE::TYPE_DEFAULT;
	_bool m_bIsContinue = false;
};
END
END

