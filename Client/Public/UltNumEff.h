#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUltNumEff final : public CUI
{
private:
	CUltNumEff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUltNumEff(const CUltNumEff& rhs);
	virtual ~CUltNumEff() = default;

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
	_float				m_fFadeTime = 0.1f;
	_bool				m_bFadeCheck = false;
	_bool				m_bZoomCheck = false;
	_bool				m_bRenderCheck = false;
	_uint				m_iUnicCount = 0;

public:
	static CUltNumEff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
