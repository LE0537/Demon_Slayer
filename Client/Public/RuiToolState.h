#pragma once

#include "RuiState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Rui)
class CToolState : public CRuiState
{
public:
	CToolState(_uint iAnimIndex, _uint iAnimIndex_2 = 0, _uint iAnimIndex_3 = 0, STATE_TYPE eType = TYPE_START, _bool bIsContinue = false);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


private:
	_uint m_iAnimIndex = 0;
	_uint m_iAnimIndex_Second = 0;
	_uint m_iAnimIndex_Third = 0;

	STATE_TYPE m_eStateType = STATE_TYPE::TYPE_DEFAULT;
	_bool m_bIsContinue = false;
};
END
END

