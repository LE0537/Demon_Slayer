#pragma once

#include "TanjiroState.h"
#include "WindMill.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CKaguraSkill_SphereState : public CTanjiroState
{
public:
	CKaguraSkill_SphereState();

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;
private:
	CWindMill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
};
END
END

