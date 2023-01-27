#pragma once

#include "TanjiroState.h"
#include "WaterMill.h"

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
	CWaterMill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
};
END
END

