#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CMenuFixedImg final : public CUI
{
private:
	CMenuFixedImg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMenuFixedImg(const CMenuFixedImg& rhs);
	virtual ~CMenuFixedImg() = default;

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
	static CMenuFixedImg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END