#pragma once

#include "Client_Defines.h"
#include "Characters.h"
#include "Base.h"

BEGIN(Client)

class CEnmuBoss : public CBase
{
	DECLARE_SINGLETON(CEnmuBoss)
public:
	enum ANIMID
	{
		ANIM_IDLE,
	};


public:
	CEnmuBoss();
	virtual ~CEnmuBoss() = default;

public:
	HRESULT Add_EnmuParts(CCharacters* pCharacter);


public:
	void Set_ToolState(_uint iPartsIndex, _uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);

public:
	vector<CCharacters*> Get_EnmuPartsList() { return m_EnmuParts; }

private:	
	vector<CCharacters*> m_EnmuParts;
	class CEnmuBossState* m_pEnmuBossState = nullptr;

public:
	virtual void Free() override;
};


END