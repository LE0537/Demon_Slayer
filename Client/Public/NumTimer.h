#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CNumTimer final : public CUI
{
private:
	CNumTimer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNumTimer(const CNumTimer& rhs);
	virtual ~CNumTimer() = default;

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
	_float				m_fTimer = 100.f;
	_uint				m_iFirstNum = 0;
	_uint				m_iSecondNum = 0;


public:
	static CNumTimer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END