#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_Dynamic.h"

BEGIN(Client)

class CLevel_Menu final : public CLevel
{
private:
	CLevel_Menu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Menu() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Layer_InkEff();
	
	

private:
	_bool			 m_bCreateUI = false;

private:
	CCamera_Dynamic::CAMERADESC_DERIVED			CameraDesc;

public:
	static CLevel_Menu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

