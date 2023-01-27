#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUltBar final : public CUI
{
private:
	CUltBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUltBar(const CUltBar& rhs);
	virtual ~CUltBar() = default;

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
	_float				m_fUltMaxBar = 0;
	_float				m_fUltCurBar = 0;

public:
	static CUltBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END