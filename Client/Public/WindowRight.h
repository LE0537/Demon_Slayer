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
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */


public:
	static CWindowRight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END