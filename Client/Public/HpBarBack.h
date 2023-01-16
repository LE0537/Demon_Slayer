#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CHpBarBack final : public CUI
{
private:
	CHpBarBack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHpBarBack(const CHpBarBack& rhs);
	virtual ~CHpBarBack() = default;

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
	static CHpBarBack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END