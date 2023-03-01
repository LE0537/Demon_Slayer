#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CTexture;
class CShader;
class CRenderer;
class CTransform;
class CModel;
class CHierarchyNode;
END


BEGIN(Client)

class CKyoujuroWeapon final : public CGameObject
{
public:
	typedef struct tagWeaponDesc
	{
		CHierarchyNode*		pSocket = nullptr;
		_float4x4			SocketPivotMatrix;
		const _float4x4*	pParentWorldMatrix;

	}WEAPONDESC;
private:
	CKyoujuroWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKyoujuroWeapon(const CKyoujuroWeapon& rhs);
	virtual ~CKyoujuroWeapon() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
private:
	CShader*				m_pShaderCom = nullptr;
	CShader*				m_pShaderCom2 = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CModel*					m_pModelCom = nullptr;

private:
	WEAPONDESC				m_WeaponDesc;
	_float4x4				m_CombinedWorldMatrix;
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

	_bool					m_bRender = false;
	_bool					m_bMenu = false;
	_bool					m_bTrue = false;

	_float m_fAngleX = 0.f;
	_float m_fAngleY = 0.f;
	_float m_fAngleZ = 0.f;

public:
	void Set_Render(_bool _bRender) {
		m_bRender = _bRender;
	}
	void Set_Menu(_bool _bMenu) { m_bMenu = _bMenu; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	_matrix Get_CombinedWorldMatrix() { return XMLoadFloat4x4(&m_CombinedWorldMatrix); }
	_float4x4* Get_CombinedWorld4x4() { return &m_CombinedWorldMatrix; }
private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

	

public:
	static CKyoujuroWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
