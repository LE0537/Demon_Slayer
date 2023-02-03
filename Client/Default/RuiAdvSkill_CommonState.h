#pragma once
#include "RuiState.h"


// u 스킬 구 생성해서 적 띄우기

BEGIN(Client)
BEGIN(Rui)
class CAdvSkill_CommonState : public CRuiState
{
public:
	CAdvSkill_CommonState();
	
	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


private:


};

END
END