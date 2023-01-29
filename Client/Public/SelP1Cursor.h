#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CSelP1Cursor final : public CUI
{
private:
	CSelP1Cursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSelP1Cursor(const CSelP1Cursor& rhs);
	virtual ~CSelP1Cursor() = default;

public:
	_uint Get_FrameLayerNum() { return m_iFrameLayerNum; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Cursor_To_SelFrame();
	void Move_Cursor();
	void Select_Order();
	void Cursor_ImgSel();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_uint				m_iFrameLayerNum = 0;

public:
	static CSelP1Cursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END