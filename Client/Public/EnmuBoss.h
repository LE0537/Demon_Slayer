#pragma once

#include "Client_Defines.h"
#include "Characters.h"
#include "Base.h"

BEGIN(Client)

class CEnmuBoss : public CBase
{
	DECLARE_SINGLETON(CEnmuBoss)
public:
	CEnmuBoss();
	virtual ~CEnmuBoss() = default;

public:
	HRESULT Add_EnmuParts(CCharacters* pCharacter);

public:
	vector<CCharacters*> Get_EnmuPartsList() { return m_EnmuParts; }

private:	
	vector<CCharacters*> m_EnmuParts;
	class CEnmuBossState* m_pEnmuBossState = nullptr;

public:
	virtual void Free() override;
};


END