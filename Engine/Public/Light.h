#pragma once
#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight() = default;

public:
	const LIGHTDESC* Get_LightDesc() const {
		return &m_LightDesc;
	}
	void	Set_ShadowLightDesc(_float4 vPos, _float4 vAt) {
		m_LightDesc.vDirection = vPos;
		m_LightDesc.vDiffuse = vAt;
	}
	HRESULT Set_LightDesc(LIGHTDESC tLightDesc)
	{
		memcpy(&m_LightDesc, &tLightDesc, sizeof(LIGHTDESC));
		return S_OK;
	}
	_bool	Get_Dead() { return m_bDead;
	}
public:
	HRESULT Initialize(const LIGHTDESC& LightDesc);

public:
	void	Tick(_float fTimeDelta);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	LIGHTDESC				m_LightDesc;

	_float				m_fAccLifeTime = 0.f;
	_float				m_fOriginRange = 0.f;
	_bool			m_bDead = false;

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	virtual void Free();
};

END