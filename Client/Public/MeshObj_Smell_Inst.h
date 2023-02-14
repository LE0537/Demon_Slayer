#pragma once
#include "GameObj.h"

BEGIN(Engine)
class CModel;

END

BEGIN(Client)

class CMeshObj_Smell_Inst final : public CGameObj
{
public:
	typedef struct tagMeshObj_Smell_Instancing_Desc
	{
		_uint			iNumInstancing = 1;
		_float4x4*		pWorld = nullptr;
		_float*			pGlowPower;

		_uint			iCurrentLevel;

		_uint		iModelIndex;

		CMeshObj_Smell_Inst*		pMe = nullptr;

	}MESHOBJ_SMELL_INSTANCING_DESC;

private:
	CMeshObj_Smell_Inst(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshObj_Smell_Inst(const CMeshObj_Smell_Inst& rhs);
	virtual ~CMeshObj_Smell_Inst() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CModel*		m_pModelCom = nullptr;
	vector<VTXMATRIX>		m_vecMatrix;
	vector<_float>			m_vecGlowPower;

	_bool		m_bFrustum = true;
	_float		m_fFrustumRadiusRatio = 5.f;

private:
	MESHOBJ_SMELL_INSTANCING_DESC			m_tMyDesc;

private:
	_float		m_fTimeMul = 0.f;
	_float		m_fAliveTime = 0.f;

	CTexture*	m_pNoiseTextureCom = nullptr;

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */
	
public:
	static CMeshObj_Smell_Inst* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END