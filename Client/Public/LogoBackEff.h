#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CLogoBackEff final : public CUI
{
private:
	CLogoBackEff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogoBackEff(const CLogoBackEff& rhs);
	virtual ~CLogoBackEff() = default;

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
	_float					m_fUvMoveTime = 0.f;
	_float					m_fAlphaTime = 0.f;
	_bool					m_bAlphaValueCheck = false;


public:
	static CLogoBackEff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END