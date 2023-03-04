#pragma once
#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)

class CKyoujuroAkazaScene : public CKyoujuroState
{
public:
	CKyoujuroAkazaScene(STATE_TYPE eType);


	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;


	void Fall_Height(CKyoujuro* pKyoujuro, _float fTimeDelta);


private:
	_float m_fSceneTimer = 0.f;
	_float m_fOriginPosY = 0.f;
	_bool m_bNextAnim = false;

};

END
END