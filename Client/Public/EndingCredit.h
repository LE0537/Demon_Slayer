#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CEndingCredit final : public CUI
{
private:
	CEndingCredit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEndingCredit(const CEndingCredit& rhs);
	virtual ~CEndingCredit() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Ending();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

private:
	_float					m_fFontFade = 1.f;
	_float					m_fDelayTime = 0.f;

public:
	static CEndingCredit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
