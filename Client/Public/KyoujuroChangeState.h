#pragma once

#include "KyoujuroState.h"
BEGIN(Client)
BEGIN(Kyoujuro)
class CChangeState : public CKyoujuroState
{
public:
	CChangeState(STATE_TYPE eType);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;


	CKyoujuroState* Increase_Height(CKyoujuro* pKyoujuro, _float fTimeDelta);
	CKyoujuroState* Fall_Height(CKyoujuro* pKyoujuro, _float fTimeDelta);


private:
	_float3 m_vPosition;
	_float3 m_vVelocity;
	_float m_fOriginPosY;
	_float m_fGravity;
	_bool m_bNextAnim = false;
};
END
END

