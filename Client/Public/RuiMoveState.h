#pragma once
#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)

class CMoveState : public CRuiState
{
public:
	CMoveState(OBJDIR eDir, STATE_TYPE eType);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pKyoujuro) override;
	virtual void Exit(CRui* pRui) override;



public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CRui* pRui, _float fTimeDelta);
	CRuiState* AIMove(CRui* pRui, OBJDIR eDir,_float fTimeDelta);

private:
	OBJDIR	m_eDirection = OBJDIR::DIR_END;
};
END
END