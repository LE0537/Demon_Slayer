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

	enum PARTS
	{
		PARTS_HEAD,
		PARTS_SHIELD,
		PARTS_RIGHT_HAND,
		PARTS_LEFT_HAND,
		PARTS_CHOCK1,
		PARTS_CHOCK2,
		PARTS_CHOCK3,
		PARTS_CHOCK4,
		PARTS_END
	};

public:
	CEnmuBoss();
	virtual ~CEnmuBoss() = default;

public:
	HRESULT Initialize();
	HRESULT Add_EnmuParts(CCharacters* pCharacter);

public:
	void Set_ToolState(_uint iPartsIndex, _uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue);
	void BossEnmu_Tick(_float fTimeDelta);
	void BossEnmu_LateTick(_float fTimeDelta);

	_bool Get_Start() const { return m_bStart; }
	void Set_Start(_bool _bStart) { m_bStart = _bStart; }

	_uint Get_BossState();
	void Set_Position(_fvector vPosition) { m_vTargetPosition = vPosition; }
	_vector Get_Position() const { return m_vTargetPosition; }
private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);

public:
	vector<CCharacters*> Get_EnmuPartsList() { return m_EnmuParts; }
	_float Get_Time() { return m_fTime; }
private:	
	vector<CCharacters*> m_EnmuParts;
	class CEnmuBossState* m_pEnmuBossState = nullptr;

	_bool m_bStart = false;
	_float m_fTime = 0.f;
	_vector m_vTargetPosition;
public:
	virtual void Free() override;
};


END