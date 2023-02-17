#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CQuiestSubBase final : public CUI
{
private:
	CQuiestSubBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQuiestSubBase(const CQuiestSubBase& rhs);
	virtual ~CQuiestSubBase() = default;

public:
	_bool Get_BaseOn() { return m_bBaseOn; }

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
	_bool						m_bBaseOn = false;
	_float						m_fFadeTime = 0.f;
	wstring						m_szQuest = TEXT("");

public:
	static CQuiestSubBase* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
