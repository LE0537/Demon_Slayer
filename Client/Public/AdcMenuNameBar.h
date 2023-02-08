#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CAdcMenuNameBar final : public CUI
{
private:
	CAdcMenuNameBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAdcMenuNameBar(const CAdcMenuNameBar& rhs);
	virtual ~CAdcMenuNameBar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_tchar			m_szName;

public:
	static CAdcMenuNameBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
