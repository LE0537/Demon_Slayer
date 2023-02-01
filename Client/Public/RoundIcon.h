#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CRoundIcon final : public CUI
{
private:
	CRoundIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRoundIcon(const CRoundIcon& rhs);
	virtual ~CRoundIcon() = default;

public:
	_bool Get_1PRoundCheck() { return m_b1PRoundCheck; }
	void Set_1PRoundCheck(_bool bCheck) { m_b1PRoundCheck = bCheck; }

	_bool Get_2PRoundCheck() { return m_b2PRoundCheck; }
	void Set_2PRoundCheck(_bool bCheck) { m_b2PRoundCheck = bCheck; }

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
	_bool					m_b1PRoundCheck = false;
	_bool					m_b2PRoundCheck = false;

public:
	static CRoundIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END