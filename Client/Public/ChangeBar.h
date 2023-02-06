#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CChangeBar final : public CUI
{
private:
	CChangeBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChangeBar(const CChangeBar& rhs);
	virtual ~CChangeBar() = default;

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
	_float		m_fTime = 0.f;
public:
	static CChangeBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
