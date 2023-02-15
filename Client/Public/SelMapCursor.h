#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CSelMapCursor final : public CUI
{
private:
	CSelMapCursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSelMapCursor(const CSelMapCursor& rhs);
	virtual ~CSelMapCursor() = default;

public:
	_uint Get_FrameNum() { return m_iFrameNum; }
	_bool Get_MapSelectCheck() { return m_bMapSelectCheck; }
	_bool Get_MapSelectEnd() { return m_bMapSelectEnd; }
	void Set_MapSelectEnd(_bool _bCheck) { m_bMapSelectEnd = _bCheck; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_uint					m_iFrameNum = 0;
	_bool					m_bMapSelectCheck = false;
	_bool					m_bMapSelectEnd = false;

public:
	static CSelMapCursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
