#pragma once
#include "NezukoState.h"

BEGIN(Client)
BEGIN(Nezuko)

class CMoveState : public CNezukoState
{
public:
	CMoveState(OBJDIR eDir, STATE_TYPE eType);

	virtual CNezukoState* HandleInput(CNezuko* pNezuko) override;
	virtual CNezukoState* Tick(CNezuko* pNezuko, _float fTimeDelta) override;
	virtual CNezukoState* Late_Tick(CNezuko* pNezuko, _float fTimeDelta) override;


	virtual void Enter(CNezuko* pNezuko) override;
	virtual void Exit(CNezuko* pNezuko) override;



public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CNezuko* pNezuko, _float fTimeDelta);


private:
	OBJDIR	m_eDirection = OBJDIR::DIR_END;
};
END
END