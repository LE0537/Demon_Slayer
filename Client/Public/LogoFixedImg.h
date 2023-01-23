#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLogoFixedImg final : public CUI
{
private:
	CLogoFixedImg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogoFixedImg(const CLogoFixedImg& rhs);
	virtual ~CLogoFixedImg() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Texture_Sel();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CLogoFixedImg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END