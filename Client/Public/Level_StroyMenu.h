#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_Dynamic.h"

BEGIN(Client)

class CLevel_StoryMenu final : public CLevel
{
private:
	CLevel_StoryMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_StoryMenu() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

private:
	_bool			 m_bCreateUI = false;

private:
	CCamera_Dynamic::CAMERADESC_DERIVED			CameraDesc;

public:
	static CLevel_StoryMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

