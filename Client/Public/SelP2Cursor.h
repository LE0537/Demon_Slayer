#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CSelP2Cursor final : public CUI
{
private:
	CSelP2Cursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSelP2Cursor(const CSelP2Cursor& rhs);
	virtual ~CSelP2Cursor() = default;

public:
	_uint Get_FrameLayerNum() { return m_iFrameLayerNum; }
	_bool Get_FirstSelCheck() { return m_bFirstSelCheck; }
	_bool Get_SecondSelCheck() { return m_bSecondSelCheck; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_uint Cursor_To_SelFrame();
	void Move_Cursor();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

private:
	_uint				m_iFrameLayerNum = 3;
	_bool				m_bFirstSelCheck = false;
	_bool				m_bSecondSelCheck = false;

public:
	static CSelP2Cursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END