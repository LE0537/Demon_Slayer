#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CWindowRight final : public CUI
{
private:
	CWindowRight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWindowRight(const CWindowRight& rhs);
	virtual ~CWindowRight() = default;

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
	CTexture*						m_pTextureMaskCom = nullptr;

private:
	_float							m_fUvMove = 0.f;
	_float							m_fDelayTime = 0.f;

public:
	static CWindowRight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END