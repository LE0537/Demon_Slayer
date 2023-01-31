#pragma once

#include "ShinobuState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Shinobu)
class CToolState : public CShinobuState
{
public:
	CToolState(_uint iAnimIndex, _uint iAnimIndex_2 = 0, _uint iAnimIndex_3 = 0, STATE_TYPE eType = TYPE_START, _bool bIsContinue = false);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


private:
	_uint m_iAnimIndex = 0;
	_uint m_iAnimIndex_Second = 0;
	_uint m_iAnimIndex_Third = 0;

	STATE_TYPE m_eStateType = STATE_TYPE::TYPE_DEFAULT;
	_bool m_bIsContinue = false;
};
END
END

