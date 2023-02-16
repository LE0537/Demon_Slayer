#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CQuiestMainIcon final : public CUI
{
private:
	CQuiestMainIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQuiestMainIcon(const CQuiestMainIcon& rhs);
	virtual ~CQuiestMainIcon() = default;

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
	static CQuiestMainIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
