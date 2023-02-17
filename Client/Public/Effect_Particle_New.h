#pragma once

#include "Client_Defines.h"
#include "GameObj.h"


BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_NewParticle_Instance;

END

BEGIN(Client)
class CEffect;

class CEffect_Particle_New final : public CGameObj
{
public:
	typedef struct ParticleInfo {
		char							m_szParticleName[MAX_PATH];

		char							szParticleDissolve[MAX_PATH];
		char							szParticleMask[MAX_PATH];
			
		_bool							bUseGlow;
		_bool							bUseRGB;
		_bool							bUseColor;
		_bool							bBillboard;
		_bool							bBillboardTurn;
		_bool							bYBillboard;
		_bool							bFollow;
		_bool							bSpeedkill;

		_uint							iMaxParticleNumber;				// 최대 파티클 수
		_uint							iOneParticleNumber;				// 한번에 방출되는 파티클 수
		_uint							iShape;							// 파티클이 방출되는 모양
		_uint							iShader;
		_uint							iTextureType;
		_int							iNumUV_U;
		_int							iNumUV_V;

		_float							fDuration;						// 파티클 시스템의 생존 시간
		_float							fStartTime;						// 파티클 시스템의 시작 시간
		_float							fLifeTime[2];					// 파티클 각각의 생존 시간
		_float							fSpeed[2];						// 파티클 각각의 속도 최대 최소값
		_float							fGravity;						// 파티클의 중력 값
		_float							fAngle;							// 파티클이 발사되는 콘 원의 각도
		_float							fRadius;						// 파티클이 발사되는 구/원의 반지름
		_float							fSpeedReduction;				// 파티클의 속도 변화값
		_float							fShotTime;						// 방출되는 시간 텀

		_float2							vParticleSize[2];				// 파티클 각각의 사이즈 최대 최소값
		_float2							vSizeReduction;					// 파티클 각각의 사이즈 변화값

		_float3							vParticleRotation[2];			// 파티클 각각의 회전값 최대 최소값
		_float3							vRotationOverLifeTime;			// 시간에 따른 각각 파티클의 회전 값
		_float3							vSize;							// 파티클이 발사되는 상자의 사이즈
		_float3							vPosition;
		_float3							vRotation;
		_float3							vGlowColor;

		_float4							vColor;							// 파티클의 시작 색상
		_float4							vColorGradient[5];				// 파티클 색상 변화

		_bool							b1;
		_bool							b2;
		_bool							b3;

		_int							i1;
		_int							i2;
		_int							i3;

		_float							f1;
		_float							f2;
		_float							f3;
		_float							f4;
		_float							f5;

		_float2							v2_1;
		_float2							v2_2;
		_float3							v3_1;
		_float3							v3_2;
		_float3							v3_3;
		_float4							v4_1;

	}PARTICLE_INFO;

	typedef struct ParticleInfoBefore {
		
	}PARTICLE_INFO_BEFORE;

private:
	CEffect_Particle_New(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Particle_New(const CEffect_Particle_New& rhs);
	virtual ~CEffect_Particle_New() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CVIBuffer_NewParticle_Instance*	m_pVIBufferCom = nullptr;

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

public:
	void Set_Parents(CEffect* pParents) { m_pParents = pParents; }
	void Set_ParticleInfo(PARTICLE_INFO ParticleInfo);

private:
	_float							m_fTime = 0.f;

	_float4x4						m_CombinedWorldMatrix;
	_float4x4						m_ParentsMtr;

	CEffect*						m_pParents = nullptr;

	PARTICLE_INFO					m_ParticleInfo;

public:
	static CEffect_Particle_New* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END