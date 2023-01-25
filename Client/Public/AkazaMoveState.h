#pragma once
#include "AkazaState.h"

BEGIN(Client)
BEGIN(Akaza)

class CMoveState : public CAkazaState
{
public:
	CMoveState(OBJDIR eDir, STATE_TYPE eType);

	virtual CAkazaState* HandleInput(CAkaza* pAkaza) override;
	virtual CAkazaState* Tick(CAkaza* pAkaza, _float fTimeDelta) override;
	virtual CAkazaState* Late_Tick(CAkaza* pAkaza, _float fTimeDelta) override;


	virtual void Enter(CAkaza* pAkaza) override;
	virtual void Exit(CAkaza* pAkaza) override;



public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CAkaza* pAkaza, _float fTimeDelta);


private:
	OBJDIR	m_eDirection = OBJDIR::DIR_END;
};
END
END