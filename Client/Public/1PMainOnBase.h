#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class C1PMainOnBase final : public CUI
{
private:
	C1PMainOnBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	C1PMainOnBase(const C1PMainOnBase& rhs);
	virtual ~C1PMainOnBase() = default;

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
	static C1PMainOnBase* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END