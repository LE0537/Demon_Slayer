#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CCharFrame final : public CUI
{
private:
	CCharFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharFrame(const CCharFrame& rhs);
	virtual ~CCharFrame() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_Info();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

public:
	static CCharFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END