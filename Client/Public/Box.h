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

class CBox final : public CGameObject
{
public:
	typedef struct tagWeaponDesc
	{
		CHierarchyNode*		pSocket = nullptr;
		_float4x4			SocketPivotMatrix;
		const _float4x4*	pParentWorldMatrix;
		_bool				bStory;
	}WEAPONDESC;
private:
	CBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBox(const CBox& rhs);
	virtual ~CBox() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

	void Set_Rot() { m_bRot = true; }
private:
	CShader*				m_pShaderCom = nullptr;
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
	_bool					m_bRot = false;
public:
	void Set_Render(_bool _bRender) {
		m_bRender = _bRender;
	}
	void Set_Menu(_bool _bMenu) { m_bMenu = _bMenu; }
	CTransform* Get_Transform() { return m_pTransformCom; }
private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */


public:
	static CBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
