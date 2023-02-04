#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CCharSelMsg final : public CUI
{
private:
	CCharSelMsg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharSelMsg(const CCharSelMsg& rhs);
	virtual ~CCharSelMsg() = default;

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
	static CCharSelMsg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END