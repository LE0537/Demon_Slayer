#pragma once

#include "TanjiroState.h"
#include "WindMill.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CSkill_WindMillState : public CTanjiroState
{
public:
	CSkill_WindMillState();

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;
private:
	CTanjiroState* Jump(CTanjiro* pTanjiro, _float fTimeDelta);
private:
	CWindMill*	m_pCollBox = nullptr;
	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_bool  m_bHit = false;
	//Jump
private:
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fCurrentPosY = 0.f;
	_bool  m_bJump = false;
	_float4 m_vLook;
	_bool   m_bLook = false;
	_bool  m_bEffect = false;
};
END
END

