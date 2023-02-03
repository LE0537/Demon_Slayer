#pragma once
#include "KyoujuroState.h"


BEGIN(Client)
BEGIN(Kyoujuro)
class CTargetRushState : public CKyoujuroState
{
public:
	CTargetRushState(STATE_TYPE eType);
	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;

	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;

	void Move(CKyoujuro* pKyoujuro, _float fTimeDelta);
	void Initialize_value(CKyoujuro* pKyoujuro);

private:
	_vector m_vTargetPosition;
	_float m_fDistance;
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_bool m_bNextAnim = false;
	_bool m_bRange = false;


};

END
END