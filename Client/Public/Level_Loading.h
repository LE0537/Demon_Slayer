#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameObj.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eNextLevel);
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

private:
	HRESULT Create_MiniGame();
	HRESULT Create_Item(_float fTimeDelta);
	void Save_Score();
	void Load_Score();
private:
	LEVEL				m_eNextLevel = LEVEL_END;
	class CLoader*		m_pLoader = nullptr;

	_bool				m_bNextLevel = false;

	_bool				m_bMiniGameIni = false;
	CGameObj*			m_pMiniGame_Player = nullptr;
	class CMini_Result*		m_pMini_Result = nullptr;

	_float				m_fCreateItemTime = 0.f;
	_float				m_fCreateMonsterTime = 0.f;
	_bool				m_bPlayerDead = false;
	_bool				m_bMiniGameStart = false;
	_int				m_iPrevRankScore = 0;
public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel);
	virtual void Free() override;
};

END

