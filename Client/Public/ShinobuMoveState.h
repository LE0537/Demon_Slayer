#include "ShinobuState.h"

BEGIN(Client)
BEGIN(Shinobu)
class CMoveState : public CShinobuState
{
public:
	CMoveState(OBJDIR eDir, STATE_TYPE eType);

	virtual CShinobuState* HandleInput(CShinobu* pShinobu) override;
	virtual CShinobuState* Tick(CShinobu* pShinobu, _float fTimeDelta) override;
	virtual CShinobuState* Late_Tick(CShinobu* pShinobu, _float fTimeDelta) override;


	virtual void Enter(CShinobu* pShinobu) override;
	virtual void Exit(CShinobu* pShinobu) override;


public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CShinobu* pShinobu, _float fTimeDelta);


private:
	OBJDIR	m_eDirection = OBJDIR::DIR_END;

};
END
END