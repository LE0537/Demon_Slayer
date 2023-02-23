#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CAdv_ResultBase final : public CUI
{
private:
	CAdv_ResultBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAdv_ResultBase(const CAdv_ResultBase& rhs);
	virtual ~CAdv_ResultBase() = default;

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
	static CAdv_ResultBase* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
