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

		_uint							iMaxParticleNumber;				// �ִ� ��ƼŬ ��
		_uint							iOneParticleNumber;				// �ѹ��� ����Ǵ� ��ƼŬ ��
		_uint							iShape;							// ��ƼŬ�� ����Ǵ� ���
		_uint							iShader;
		_uint							iTextureType;
		_int							iNumUV_U;
		_int							iNumUV_V;

		_float							fDuration;						// ��ƼŬ �ý����� ���� �ð�
		_float							fStartTime;						// ��ƼŬ �ý����� ���� �ð�
		_float							fLifeTime[2];					// ��ƼŬ ������ ���� �ð�
		_float							fSpeed[2];						// ��ƼŬ ������ �ӵ� �ִ� �ּҰ�
		_float							fGravity;						// ��ƼŬ�� �߷� ��
		_float							fAngle;							// ��ƼŬ�� �߻�Ǵ� �� ���� ����
		_float							fRadius;						// ��ƼŬ�� �߻�Ǵ� ��/���� ������
		_float							fSpeedReduction;				// ��ƼŬ�� �ӵ� ��ȭ��
		_float							fShotTime;						// ����Ǵ� �ð� ��

		_float2							vParticleSize[2];				// ��ƼŬ ������ ������ �ִ� �ּҰ�
		_float2							vSizeReduction;					// ��ƼŬ ������ ������ ��ȭ��

		_float3							vParticleRotation[2];			// ��ƼŬ ������ ȸ���� �ִ� �ּҰ�
		_float3							vRotationOverLifeTime;			// �ð��� ���� ���� ��ƼŬ�� ȸ�� ��
		_float3							vSize;							// ��ƼŬ�� �߻�Ǵ� ������ ������
		_float3							vPosition;
		_float3							vRotation;
		_float3							vGlowColor;

		_float4							vColor;							// ��ƼŬ�� ���� ����
		_float4							vColorGradient[5];				// ��ƼŬ ���� ��ȭ

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
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

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