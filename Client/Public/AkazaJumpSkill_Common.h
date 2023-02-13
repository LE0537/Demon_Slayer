#pragma once

#include "AkazaState.h"
#include "BaseAtk.h"


BEGIN(Client)
BEGIN(Akaza)
class CJumpSkill_CommmonState : public CAkazaState
{
public:
	CJumpSkill_CommmonState(STATE_TYPE eType, _uint iCount = 0);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;


	void Jump(CAkaza* pAkaza, _float fTimeDelta);
	void Initialize_value(CAkaza * pAkaza);

private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_uint m_iCount = 0;
	_float m_fNextAnim = 0.f;
private:

	_float m_fTime = 0.f;
	_float m_fHitTime = 0.f;
	_float m_fDelay = 0.f;
	_bool m_bCombo = false;

	_float m_fMove = 0.f;
	_int   m_iHit = 0;

	_bool	m_bEffect = false;
};
END
END

