#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUltOpenBar final : public CUI
{
private:
	CUltOpenBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUltOpenBar(const CUltOpenBar& rhs);
	virtual ~CUltOpenBar() = default;

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
	_float						m_fMaxTime = 0.f;
	_float						m_fCurTime = 0.f;
	_int						m_iPowerCount = 0;
	_bool						m_bOniCheck = false;
public:
	static CUltOpenBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
