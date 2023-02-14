#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CRankIcon final : public CUI
{
private:
	CRankIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRankIcon(const CRankIcon& rhs);
	virtual ~CRankIcon() = default;

public:
	_bool Get_ZoomEnd() { return m_bZoomEnd; }
	void Set_ZoomStart(_bool bCheck) { m_bZoomStart = bCheck; }

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
	_bool					m_bZoomEnd = false;
	_bool					m_bZoomStart = false;
	_uint					m_iRankScore = 0;
	_bool					m_bMinusCheck = false;


public:
	static CRankIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END