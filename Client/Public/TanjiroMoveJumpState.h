#include "TanjiroState.h"

BEGIN(Client)
BEGIN(Tanjiro)
class CMoveJumpState : public CTanjiroState
{
public:
	CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CTanjiroState* HandleInput(CTanjiro* pTanjiro) override;
	virtual CTanjiroState* Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;
	virtual CTanjiroState* Late_Tick(CTanjiro* pTanjiro, _float fTimeDelta) override;


	virtual void Enter(CTanjiro* pTanjiro) override;
	virtual void Exit(CTanjiro* pTanjiro) override;


public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CTanjiro* pTanjiro, _float fTimeDelta);
	CTanjiroState* Jump(CTanjiro* pTanjiro, _float fTimeDelta);

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