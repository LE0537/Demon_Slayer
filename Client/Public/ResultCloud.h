#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CResultCloud final : public CUI
{
private:
	CResultCloud(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CResultCloud(const CResultCloud& rhs);
	virtual ~CResultCloud() = default;

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
	_float					m_fMoveTime = 0.f;
	_bool					m_bMoveCheck = false;
	_int					m_iMoveCount = 0;


public:
	static CResultCloud* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END