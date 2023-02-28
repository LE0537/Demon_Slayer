#pragma once

#include "TanjiroState.h"


BEGIN(Client)
BEGIN(Tanjiro)


class CSplSkrStartState : public CTanjiroState
{
public:
	CSplSkrStartState(STATE_TYPE eType);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

private:
	void Move(CTanjiro* pTanjiro, _float fTimeDelta);

private:
	_bool m_bCollision = false;

};

END
END