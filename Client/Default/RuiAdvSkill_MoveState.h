#include "RuiState.h"


// 움직이면서 u 백스텝 후 앞에 거미줄

BEGIN(Client)
BEGIN(Rui)
class CAdvSkill_MoveState : public CRuiState
{
public:
	CAdvSkill_MoveState();

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


private:
	_float m_fBackStepTime = 0.f;

};

END
END