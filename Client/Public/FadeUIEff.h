#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CFadeUIEff final : public CUI
{
private:
	CFadeUIEff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFadeUIEff(const CFadeUIEff& rhs);
	virtual ~CFadeUIEff() = default;

public:
	_bool Get_UIOn() { return m_bUIOn; }
	
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
	_float					m_fFadeTime = 0.f;
	_bool					m_bUIOn = false;
	_float					m_fStopTime = 0.f;

public:
	static CFadeUIEff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
