#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLogoButton final : public CUI
{
private:
	CLogoButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogoButton(const CLogoButton& rhs);
	virtual ~CLogoButton() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Font_Color();
	HRESULT Add_InkEff();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_float4				m_vFontColor;
	_float				m_fAlpha = 0.f;

public:
	static CLogoButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END