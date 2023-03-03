#pragma once
#include "GameObj.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)
class CEffect;

class CEffect_AnimMesh :
	public CGameObj
{
public:
	typedef struct AnimMeshInfo {
		_tchar							m_szMeshName[MAX_PATH];

		_tchar							szMeshType[MAX_PATH];
		_tchar							szMeshDiffuse[MAX_PATH];
		_tchar							szMeshDissolve[MAX_PATH];
		_tchar							szMeshMask[MAX_PATH];

		_float							fLifeTime;
		_float							fStartTime;

		_float3							vPosition;
		_float3							vRotation;

		CGameObj*						pParents;

		/*_bool							bGlow;
		_bool							bUseRGB;
		_bool							bUseColor;
		_bool							bUseFlowMap;
		_bool							bUseGlowColor;
		_bool							bUseMask;
		_bool							bUseMeshDiffuse;
		_bool							bDisappearAlpha;

		_uint							iSprite_U;
		_uint							iShader;
		_uint							iSprite_V;
		_int							iNumUV_U = 1;
		_int							iNumUV_V = 1;

		_float							fPostProcessingValue;
		_float							fMoveSpeed;
		_float							fMoveFalloffSpeed;
		_float							fTurn;
		_float							fTurnFalloff;
		_float							fGravity;
		_float							fDistortionScale;
		_float							fDistortionBias;
		_float							fDistortion_U;
		_float							fDistortion_V;
		_float							fMove_Value_U;
		_float							fMove_Value_V;
		_float							fDisappearTimeRatio;

		_float3							vSize[6];
		_float3							vGlowColor;

		_float4							vColor[4];


		_float3							vTurnDirection;
		_float3							vMoveDirection;*/

	}ANIM_MESH_INFO;

private:
	CEffect_AnimMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_AnimMesh(const CEffect_AnimMesh& rhs);
	virtual ~CEffect_AnimMesh() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CModel*							m_pModelCom = nullptr;
	CTexture*						m_pDissolveTextureCom = nullptr;
	CTexture*						m_pDiffuseTextureCom = nullptr;
	CTexture*						m_pMaskTextureCom = nullptr;

private:
	ANIM_MESH_INFO					m_pInfo;

	_bool							m_bStart = true;

	_float							m_fTime = 0.f;
	_float							m_fMoveUV_U = 0.f;
	_float							m_fMoveUV_V = 0.f;
	_float							m_fTurnSpeed = 0.f;

	_float4x4						m_CombinedWorldMatrix;
	_float4x4						m_ParentsMtr;

	CGameObj*						m_pParents = nullptr;

	vector<char *>					m_MeshName;

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();

public:
	static CEffect_AnimMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END