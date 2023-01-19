#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CSelOniLight final : public CUI
{
private:
	CSelOniLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSelOniLight(const CSelOniLight& rhs);
	virtual ~CSelOniLight() = default;

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
	static CSelOniLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END