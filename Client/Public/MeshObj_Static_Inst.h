#pragma once
#include "GameObj.h"

BEGIN(Engine)
class CModel;

END

BEGIN(Client)

class CMeshObj_Static_Inst final : public CGameObj
{
public:
	typedef struct tagMeshObj_Static_Instancing_Desc
	{
		_uint			iNumInstancing = 1;
		_float4x4*		pWorld = nullptr;
		_float*			pGlowPower;

		_uint			iCurrentLevel;

		_uint		iModelIndex;

	}MESHOBJ_STATIC_INSTANCING_DESC;

private:
	CMeshObj_Static_Inst(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshObj_Static_Inst(const CMeshObj_Static_Inst& rhs);
	virtual ~CMeshObj_Static_Inst() = default;

public:
	void	Change_Model(_uint iObjNumber, const _tchar* pModel_PrototypeTag);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

private:
	CModel*		m_pModelCom = nullptr;
	vector<VTXMATRIX>		m_vecMatrix;
	vector<_float>			m_vecGlowPower;

	_bool		m_bFrustum = true;
	_float		m_fFrustumRadiusRatio = 1.f;

private:
	MESHOBJ_STATIC_INSTANCING_DESC			m_tMyDesc;
	_bool		m_bRenderShadow = true;

	_bool		m_bInit = false;
	_float4x4	m_matProjOrigin;

private:
	HRESULT Delete_Component(const _tchar* pComponentTag);

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

	HRESULT Ready_ModelComponent();

public:
	static CMeshObj_Static_Inst* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END