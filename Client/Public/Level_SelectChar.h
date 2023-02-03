#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_Dynamic.h"
BEGIN(Client)

class CLevel_SelectChar final : public CLevel
{
private:
	CLevel_SelectChar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_SelectChar() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);


private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);

	_bool			 m_bCreateUI = false;
	_float	           m_fDelayTime = 0.f;


private:
	_bool			 m_bCreateUI = false;
	CCamera_Dynamic::CAMERADESC_DERIVED			CameraDesc;
public:
	static CLevel_SelectChar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END


