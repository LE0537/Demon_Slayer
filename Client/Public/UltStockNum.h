#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUltStockNum final : public CUI
{
private:
	CUltStockNum(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUltStockNum(const CUltStockNum& rhs);
	virtual ~CUltStockNum() = default;

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
	_uint				m_iFrame = 0;
	_uint				m_iNumTextureU = 0;
	_uint				m_iNumTextureV = 0;

public:
	static CUltStockNum* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END