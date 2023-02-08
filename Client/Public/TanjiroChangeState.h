#pragma once

#include "TanjiroState.h"
BEGIN(Client)
BEGIN(Tanjiro)
class CChangeState : public CTanjiroState
{
public:
	CChangeState(STATE_TYPE eType);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;


	CTanjiroState* Increase_Height(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Fall_Height(CTanjiro* pTanjiro, _float fTimeDelta);


private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_float m_fGravity;
	_bool m_bNextAnim = false;
	_bool m_bEffect = false;
};
END
END

