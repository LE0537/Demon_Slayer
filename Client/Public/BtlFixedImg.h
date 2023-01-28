#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CBtlFixedImg final : public CUI
{
private:
	CBtlFixedImg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBtlFixedImg(const CBtlFixedImg& rhs);
	virtual ~CBtlFixedImg() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Sel_Texture();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */
	_float fRadian = 0.f;

public:
	static CBtlFixedImg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END