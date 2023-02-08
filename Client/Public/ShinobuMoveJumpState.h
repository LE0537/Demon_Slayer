#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)
class CMoveJumpState : public CShinobuState
{
public:
	CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CShinobu* pShinobu, _float fTimeDelta);
	CShinobuState* Jump(CShinobu* pShinobu, _float fTimeDelta);

private:
	OBJDIR	m_eDirection = OBJDIR::DIR_END;
	OBJDIR  m_eNextDir = OBJDIR::DIR_END;
	_float m_fJumpPower = 15.f;
	_float m_fJumpTime = 0.f;
	_float m_fGravity = 9.8f;
	_float m_fCurrentPosY = 0.f;
	_bool m_bMove = false;
	_bool m_bEffect = false;
};
END
END