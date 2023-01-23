#pragma once

#include "TanjiroState.h"
#include "BaseAtk.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CSkill_WaterMillState : public CTanjiroState
{
public:
	CSkill_WaterMillState(STATE_TYPE eType);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

private:


};
END
END

