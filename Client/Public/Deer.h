#pragma once
#include "Client_Defines.h"
#include "Characters.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CModel;
class CCollider;
class CNavigation;

END


BEGIN(Client)

class CDeer : public CCharacters
{
public:
	CDeer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDeer(const CDeer& rhs);
	virtual ~CDeer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();


public:
	//	CTransform* Get_Transfrom() const { return m_pTransformCom; }
	CModel* Get_Model() const { return m_pModelCom; }



private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();


public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);


private:
	CModel*					m_pModelCom = nullptr;

	_float					m_fEffectTime = 0.f;
private:


public:
	static CDeer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;


};

END
