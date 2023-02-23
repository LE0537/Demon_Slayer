#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLoadingBar final : public CUI
{
private:
	CLoadingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingBar(const CLoadingBar& rhs);
	virtual ~CLoadingBar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

private:
	_float					m_fMaxTime = 3.f;
	_float					m_fCurTime = 0.f;


public:
	static CLoadingBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END