#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class C2PIcon final : public CUI
{
private:
	C2PIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	C2PIcon(const C2PIcon& rhs);
	virtual ~C2PIcon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

private:
	_bool				m_bRenderOn = false;

public:
	static C2PIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END