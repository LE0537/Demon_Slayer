#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CRoundUI final : public CUI
{
private:
	CRoundUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRoundUI(const CRoundUI& rhs);
	virtual ~CRoundUI() = default;

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
	CTexture*				m_pTextureCom2 = nullptr;


public:
	static CRoundUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END