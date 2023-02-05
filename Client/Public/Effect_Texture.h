#pragma once
#include "GameObj.h"
#include "Client_Defines.h"

BEGIN(Client)

class CEffect_Texture : public CGameObj
{
public:
	typedef struct TextureInfo {
		char							m_szTextureName[MAX_PATH];

		char							szTextureType[MAX_PATH];
		char							szTextureDissolve[MAX_PATH];
		char							szTextureMask[MAX_PATH];

		_bool							bBillboard;
		_bool							bGlow;
		_bool							bUseRGB;
		_bool							bUseColor;
		_bool							bUseFlowMap;
		_bool							bUseGlowColor;
		_bool							bUseMask;
		_bool							bDisappearAlpha;

		_int							iSortingFudge;
		_int							iNumUV_U = 1;
		_int							iNumUV_V = 1;

		_uint							iShader;
		_uint							iMoveType;

		_float							fStartTime;
		_float							fLifeTime;
		_float							fMoveSpeed;
		_float							fMoveFalloffSpeed;
		_float							fTurn;
		_float							fTurnFalloff;
		_float							fPostProcessingValue;
		_float							fGravity;
		_float							fDistortionScale;
		_float							fDistortionBias;
		_float							fDistortion_U;
		_float							fDistortion_V;
		_float							fMove_Value_U;
		_float							fMove_Value_V;
		_float							fDisappearTimeRatio;

		_float2							vSize[6];

		_float3							vPosition;
		_float3							vGlowColor;
		_float3							fRotation;
		_float3							vMoveDirection;

		_float4							vColor;

	}TEXTURE_INFO;

private:
	CEffect_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Texture(const CEffect_Texture& rhs);
	virtual ~CEffect_Texture() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_Parents(CGameObj* pParents) { m_pParents = pParents; }
	void Set_TexInfo(TextureInfo TexInfo);

private:
	CTexture*						m_pNoiseTextureCom = nullptr;
	CTexture*						m_pDissolveTextureCom = nullptr;
	CTexture*						m_pMaskTextureCom = nullptr;

private:
	TEXTURE_INFO					m_TextureInfo;

	_float							m_fTime = 0.f;
	_float							m_fMoveUV_U = 0.f;
	_float							m_fMoveUV_V = 0.f;
	_float							m_fTurnSpeed = 0.f;

	_float4x4						m_CombinedWorldMatrix;

	CGameObj*						m_pParents = nullptr;

	_bool							m_bDead = false;

	vector<char *>					m_TextureName;

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();

public:
	static CEffect_Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END