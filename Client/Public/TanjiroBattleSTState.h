#pragma once

#include "TanjiroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CBattleStartState : public CTanjiroState
{
public:
	CBattleStartState();

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;
private:
	_float  m_fTime = 0.f;
};
END
END

