#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_Dynamic.h"

BEGIN(Client)

class CLevel_AdvRui final : public CLevel
{
private:
	CLevel_AdvRui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_AdvRui() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Load_StaticObjects(char* pFileName);
	HRESULT Load_Map(const _tchar* pLayerTag, char* pFileName);
private:
	_bool			 m_bCreateUI = false;

private:
	CCamera_Dynamic::CAMERADESC_DERIVED			CameraDesc;

public:
	static CLevel_AdvRui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

