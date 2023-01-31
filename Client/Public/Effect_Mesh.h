#pragma once
#include "GameObj.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CEffect_Mesh :
	public CGameObj
{
public:
	typedef struct MeshInfo {
		char							m_szMeshName[MAX_PATH];

		char							szMeshType[MAX_PATH];
		char							szMeshDissolve[MAX_PATH];
		char							szMeshMask[MAX_PATH];

		_bool							bGlow;
		_bool							bUseRGB;
		_bool							bUseColor;
		_bool							bUseFlowMap;
		_bool							bUseGlowColor;
		_bool							bMaskTest;
		_bool							bUseMeshDiffuse;
		_bool							bDisappearAlpha;

		_uint							iMeshType;
		_uint							iShader;
		_uint							iMoveType;

		_float							fLifeTime;
		_float							fStartTime;
		_float							fPostProcessingValue;
		_float							fMoveSpeed;
		_float							fTurn;
		_float							fTurnFalloff;
		_float							fDistortionScale;
		_float							fDistortionBias;
		_float							fDistortion_U;
		_float							fDistortion_V;
		_float							fMove_Value_U;
		_float							fMove_Value_V;
		_float							fDisappearTimeRatio;

		_float3							vSize[6];
		_float3							vPosition;
		_float3							vRotation;
		_float3							vGlowColor;

		_float4							vColor[4];

	}MESH_INFO;

private:
	CEffect_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Mesh(const CEffect_Mesh& rhs);
	virtual ~CEffect_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_Parents(CGameObj* pParents) { m_pParents = pParents; }
	void Set_MeshInfo(MESH_INFO MeshInfo);

private:
	CModel*							m_pModelCom = nullptr;
	CTexture*						m_pNoiseTextureCom = nullptr;
	CTexture*						m_pDissolveTextureCom = nullptr;
	CTexture*						m_pDiffuseTextureCom = nullptr;

private:
	MESH_INFO						m_MeshInfo;

	_float							m_fTime = 0.f;
	_float							m_fMoveUV_U = 0.f;
	_float							m_fMoveUV_V = 0.f;

	_float4x4						m_CombinedWorldMatrix;

	CGameObj*						m_pParents = nullptr;

	_bool							m_bDead = false;

	vector<char *>					m_MeshName;

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();

public:
	static CEffect_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END