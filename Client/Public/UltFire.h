#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUltFire final : public CUI
{
private:
	CUltFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUltFire(const CUltFire& rhs);
	virtual ~CUltFire() = default;

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
	_uint				m_iFrame = 0;
	_uint				m_iNumTextureU = 0;
	_uint				m_iNumTextureV = 0;
	_float				m_fSpriteTime = 0.f;
	_uint				m_iPowerIndex = 0;

public:
	static CUltFire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
