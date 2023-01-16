#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CTexture;
class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CModel;
class CHierarchyNode;
class CVIBuffer_Trail;
END


BEGIN(Client)

class CTanjiroWeapon final : public CGameObject
{
public:
	typedef struct tagWeaponDesc
	{
		CHierarchyNode*		pSocket = nullptr;
		_float4x4			SocketPivotMatrix;
		const _float4x4*	pParentWorldMatrix;

	}WEAPONDESC;
private:
	CTanjiroWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTanjiroWeapon(const CTanjiroWeapon& rhs);
	virtual ~CTanjiroWeapon() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;

private:
	WEAPONDESC				m_WeaponDesc;
	_float4x4				m_CombinedWorldMatrix;


	_bool					m_bRender = false;


public:
	void Set_Render(_bool _bRender) {
		m_bRender = _bRender;
	}

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CTanjiroWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
