#pragma once
#include "GameObj.h"

BEGIN(Engine)
class CModel;

END

BEGIN(Client)

class CMeshObj_Static final : public CGameObj
{
public:
	typedef struct tagMeshObj_Static_Desc
	{
		_float4x4	matWorld;
		_float		fGlowPower;
		_uint		iCurrentLevel;

		_uint		iModelIndex;

		_bool		bAlphaBlend;

	}MESHOBJ_STATIC_DESC;

private:
	CMeshObj_Static(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshObj_Static(const CMeshObj_Static& rhs);
	virtual ~CMeshObj_Static() = default;

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

private:
	MESHOBJ_STATIC_DESC			m_tMyDesc;
	_bool		m_bRenderShadow = true;

	_float		m_fFrustumRadiusRatio = 1.f;

private:
	HRESULT Delete_Component(const _tchar* pComponentTag);

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

	HRESULT Ready_ModelComponent();

public:
	static CMeshObj_Static* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END