#include "KyoujuroState.h"

BEGIN(Client)
BEGIN(Kyoujuro)
class CMoveJumpState : public CKyoujuroState
{
public:
	CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime);

	virtual CKyoujuroState* HandleInput(CKyoujuro* pKyoujuro) override;
	virtual CKyoujuroState* Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;
	virtual CKyoujuroState* Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta) override;


	virtual void Enter(CKyoujuro* pKyoujuro) override;
	virtual void Exit(CKyoujuro* pKyoujuro) override;


public:
	OBJDIR Get_DIR() const { return m_eDirection; }

private:
	void Move(CKyoujuro* pKyoujuro, _float fTimeDelta);
	CKyoujuroState* Jump(CKyoujuro* pKyoujuro, _float fTimeDelta);

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