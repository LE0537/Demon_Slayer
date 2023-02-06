#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CComboNum final : public CUI
{
private:
	CComboNum(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComboNum(const CComboNum& rhs);
	virtual ~CComboNum() = default;

public:
	_uint Get_ComboNum() { return m_iComboNum; }

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
	_uint				m_iFirstNum = 0;
	_uint				m_iSecondNum = 0;
	_uint				m_iComboNum = 0;
	_bool				m_bRenderOnOff = false;

public:
	static CComboNum* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
