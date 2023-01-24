#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLoadingShoji final : public CUI
{
private:
	CLoadingShoji(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingShoji(const CLoadingShoji& rhs);
	virtual ~CLoadingShoji() = default;

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
	static CLoadingShoji* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END