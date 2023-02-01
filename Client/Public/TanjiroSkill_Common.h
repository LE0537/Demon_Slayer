#pragma once

#include "TanjiroState.h"
#include "WindMill.h"

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
private:
	CWindMill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_float4 m_vLook;
	_bool   m_bLook = false;
};
END
END

