#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_SelectMap final : public CLevel
{
private:
	CLevel_SelectMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_SelectMap() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

private:
	_bool			 m_bCreateUI = false;
	_float	           m_fDelayTime = 0.f;

public:
	static CLevel_SelectMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END


