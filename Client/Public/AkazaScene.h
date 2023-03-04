#pragma once

#include "Client_Defines.h"
#include "Characters.h"
#include "Base.h"

BEGIN(Client)

class CAkazaScene : public CBase
{
	DECLARE_SINGLETON(CAkazaScene)
public:
	enum ANIMID
	{
		ANIM_IDLE,
	};

	enum SCENE
	{
		SCENE_TANJIRO,
		SCENE_KYOUJURO,
		SCENE_AKAZA,
		SCENE_END
	};

public:
	CAkazaScene();
	virtual ~CAkazaScene() = default;

public:
	HRESULT Initialize();
	HRESULT Add_Character(CCharacters* pCharacter);

public:
	void CAkazaScene_Tick(_float fTimeDelta);
	void CAkazaScene_LateTick(_float fTimeDelta);

	_bool Get_Start() const { return m_bStart; }
	void Set_Start(_bool _bStart) { m_bStart = _bStart; }



private:
	void HandleInput();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);

public:
	vector<CCharacters*> Get_AkazaSceneList() { return m_AkazaSceneList; }
	_float Get_Time() { return m_fTime; }
private:	
	vector<CCharacters*> m_AkazaSceneList;
	class CAkazaSceneState* m_pAkazaSceneState = nullptr;

	_bool m_bStart = false;
	_float m_fTime = 0.f;
public:
	virtual void Free() override;
};


END