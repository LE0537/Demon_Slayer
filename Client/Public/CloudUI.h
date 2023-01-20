#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CCloudUI final : public CUI
{
private:
	CCloudUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCloudUI(const CCloudUI& rhs);
	virtual ~CCloudUI() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_ImgNum(_uint iNum);

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CCloudUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END