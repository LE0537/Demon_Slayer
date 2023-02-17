#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CQuiestStampIcon final : public CUI
{
private:
	CQuiestStampIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQuiestStampIcon(const CQuiestStampIcon& rhs);
	virtual ~CQuiestStampIcon() = default;

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
	_float					m_fFadeTime = 0;
	_bool					m_bIconOn = false;

public:
	static CQuiestStampIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
