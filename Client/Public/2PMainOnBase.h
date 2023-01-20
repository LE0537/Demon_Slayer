#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class C2PMainOnBase final : public CUI
{
private:
	C2PMainOnBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	C2PMainOnBase(const C2PMainOnBase& rhs);
	virtual ~C2PMainOnBase() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static C2PMainOnBase* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END