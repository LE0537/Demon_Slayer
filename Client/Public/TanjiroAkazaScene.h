#pragma once

#include "TanjiroState.h"
#include "Client_Defines.h"


BEGIN(Client)
BEGIN(Tanjiro)

class CTanjiroAkazaScene : public CTanjiroState
{
public:
	CTanjiroAkazaScene();

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;

private:
	_bool m_bNextAnim;
	_bool m_bControl;
	_float m_fDelay = 0.f;
};

END
END