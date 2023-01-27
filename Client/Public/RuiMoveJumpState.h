#include "RuiState.h"

BEGIN(Client)
BEGIN(Rui)
class CMoveJumpState : public CRuiState
{
public:
	CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CRuiState* HandleInput(CRui* pRui) override;
	virtual CRuiState* Tick(CRui* pRui, _float fTimeDelta) override;
	virtual CRuiState* Late_Tick(CRui* pRui, _float fTimeDelta) override;


	virtual void Enter(CRui* pRui) override;
	virtual void Exit(CRui* pRui) override;


public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CRui* pRui, _float fTimeDelta);
	CRuiState* Jump(CRui* pRui, _float fTimeDelta);

private:
	OBJDIR	m_eDirection = OBJDIR::DIR_END;
	OBJDIR  m_eNextDir = OBJDIR::DIR_END;
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;
	_bool m_bMove = false;
};
END
END