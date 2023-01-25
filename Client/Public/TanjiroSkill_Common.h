#pragma once

#include "TanjiroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CSkill_CommonState : public CTanjiroState
{
public:
	CSkill_CommonState();

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

};
END
END

