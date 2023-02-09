#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_Dynamic.h"

BEGIN(Client)

class CLevel_GameResult final : public CLevel
{
private:
	CLevel_GameResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GameResult() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Load_Map(const _tchar* pLayerTag, char* pFileName);
	HRESULT Load_StaticObjects(char* pFileName);
private:
	_bool			 m_bCreateUI = false;
	class CCharacters*  m_pModel = nullptr;
	class CCharacters*  m_pModelSub = nullptr;
	_int            m_iMain = 0;
	_int			m_iSub = 0;
	_bool			m_bStart = false;
	_float			m_fStartTime = 0.f;
private:
	CCamera_Dynamic::CAMERADESC_DERIVED			CameraDesc;

public:
	static CLevel_GameResult* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

