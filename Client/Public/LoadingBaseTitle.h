#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLoadingBaseTitle final : public CUI
{
private:
	CLoadingBaseTitle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingBaseTitle(const CLoadingBaseTitle& rhs);
	virtual ~CLoadingBaseTitle() = default;

public:
	_uint Get_Rand() { return m_iRand; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Tip();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	wstring								m_szTitle = TEXT("");
	_uint								m_iRand = 0;


public:
	static CLoadingBaseTitle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END