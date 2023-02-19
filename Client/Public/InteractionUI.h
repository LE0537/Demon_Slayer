#pragma once

#include "Client_Defines.h"
#include "UI.h"


BEGIN(Client)

class CInteractionUI final : public CUI
{
private:
	CInteractionUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInteractionUI(const CInteractionUI& rhs);
	virtual ~CInteractionUI() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void   OnBillboard();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

private:
	_float					m_fFadeTime = 0.f;

public:
	static CInteractionUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
