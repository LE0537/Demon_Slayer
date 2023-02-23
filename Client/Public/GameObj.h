#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

END


BEGIN(Client)

class CGameObj : public CGameObject
{
protected:
	CGameObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObj(const CGameObj& rhs);
	virtual ~CGameObj() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
public:
	void   Set_EffectTime(_float _fTime) { m_fEffectTime = _fTime; }
	_float Get_EffectTime() { return m_fEffectTime; }
protected:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

protected:
	_float		m_fCamAngle = 0.f;
	_float      m_fEffectTime = 0.f;
public:
	CTransform* Get_Transform() { return m_pTransformCom; }
	CRenderer* Get_Renderer() { return m_pRendererCom; }
	void	Set_CamAngle(_float _fCamAngle) { m_fCamAngle = _fCamAngle; }
	_float  Get_CamAngle() { return m_fCamAngle; }
public:
	static CGameObj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
