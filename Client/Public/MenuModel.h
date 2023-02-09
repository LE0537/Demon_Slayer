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

END


BEGIN(Client)

class CMenuModel : public CCharacters
{
private:
	CMenuModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMenuModel(const CMenuModel& rhs);
	virtual ~CMenuModel() = default;

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
	void	Set_Pos();
	HRESULT Ready_TanjiroParts();
	HRESULT Ready_TanjiroParts2();
	HRESULT Ready_KyoujuroParts();
	HRESULT Ready_KyoujuroParts2();
	HRESULT Ready_ShinobuParts();
	HRESULT Ready_ShinobuParts2();
public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
public:
	CGameObject* Get_Weapon(_int _iIndex) { return m_pWeapon[_iIndex]; }
	CGameObject* Get_Sheath(_int _iIndex) { return m_pSheath[_iIndex]; }
	void		 Set_Black(_bool _bBlack) { m_bBlack = _bBlack; }
	CModel*      Get_ModelCom(_int _Index) { return m_pModelCom[_Index]; }
private:
	CModel*					m_pModelCom[6] = { nullptr };
	CGameObject*			m_pWeapon[3] = { nullptr };
	CGameObject*			m_pSheath[3] = { nullptr };

	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	_int					m_iCurrentModelIndex = 0;
	_bool					m_bPos = false;
	_bool					m_bBlack = false;

public:
	static CMenuModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
