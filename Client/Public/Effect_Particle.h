#pragma once

#include "Client_Defines.h"
#include "GameObj.h"

BEGIN(Engine)

class CVIBuffer_Particle_Instance;

END

BEGIN(Client)

class CEffect_Particle :
	public CGameObj
{
public:
	typedef struct ParticleInfo {
		char							m_szParticleName[MAX_PATH];

		char							szParticleDissolve[MAX_PATH];
		char							szParticleMask[MAX_PATH];

		_bool							bGlow;
		_bool							bUseRGB;
		_bool							bUseColor;
		_bool							bUseFlowMap;
		_bool							bUseGlowColor;
		_bool							bUseMask;
		_bool							bBillboard;
		_bool							bYBillboard;
		_bool							bDisappearStart;
		_bool							bGravity;
		_bool							bPositionPix;
		_bool							bSizePix;
		_bool							bRoof;

		_uint							iDisappear;
		_uint							iShader;
		_uint							iTextureType;
		_uint							iParticleNum;
		_uint							iParticleType;
		_uint							iConeSizeX;
		_uint							iConeSizeY;
		_uint							iNumUV_U;
		_uint							iNumUV_V;

		_float							fGravitySpeed;
		_float							fLifeTime[2];
		_float							fStartTime;
		_float							fSpeed[2];
		_float							fStartTurm;
		_float							fSpeedType;
		_float							fMoveSpeed;
		_float							fDistortionScale;
		_float							fDistortionBias;
		_float							fDirectionX;
		_float							fDirectionY;
		_float							fMove_Value_U;
		_float							fMove_Value_V;
		_float							fDisappearTimeRatio;
		_float							fRoofTime[2];
		_float							fRoofTerm;

		_float2							vSize[2];

		_float3							vPosition;
		_float3							vGlowColor;
		_float3							vRotation;

		_float4							vColor;

	}PARTICLE_INFO;

private:
	CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Particle(const CEffect_Particle& rhs);
	virtual ~CEffect_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CVIBuffer_Particle_Instance*	m_pVIBufferCom = nullptr;

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

public:
	void Set_Parents(CGameObj* pParents) { m_pParents = pParents; }
	void Set_ParticleInfo(PARTICLE_INFO ParticleInfo);

private:
	_float							m_fTime = 0.f;

	_float4x4						m_CombinedWorldMatrix;

	CGameObj*						m_pParents = nullptr;

	PARTICLE_INFO					m_ParticleInfo;

public:
	static CEffect_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END

