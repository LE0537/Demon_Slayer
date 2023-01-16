#pragma once
#include "Client_Defines.h"
#include "GameObj.h"


BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CModel;
class CCollider;

END


BEGIN(Client)

class CKyoujuro : public CGameObj
{
private:
	CKyoujuro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKyoujuro(const CKyoujuro& rhs);
	virtual ~CKyoujuro() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void	Set_ShadowLightPos();
	HRESULT Ready_Parts();
	HRESULT Ready_Parts2();

private:
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;

	CGameObject*			m_pWeapon = nullptr;
	CGameObject*			m_pSheath = nullptr;

public:
	static CKyoujuro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
