#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL  CVIBuffer_NewParticle_Instance final : public CVIBuffer_Instance
{
private:
	CVIBuffer_NewParticle_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_NewParticle_Instance(const CVIBuffer_NewParticle_Instance& rhs);
	virtual ~CVIBuffer_NewParticle_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance) override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Update(_float fTimeDelta, _float2 fScaleReduction, _float4x4 ParentMtr, _float iSpeedReduction, _float fGravity, _bool bSpeedKill);

	void Reset(_uint iMaxParticleNumber, _float* fLifeTime, _float* fSpeed, _float2* vTexScale, _uint iParticleType,
		_float fAngle, _float fRadius, _float3 vSize, _float3* vRotation, _float4 vColor, _float fDuration, _float fShotTime,
		_uint iOneParticleNumber, _float fCircleY, _float fCircleAngle);

	void Reset_One(_uint iNum);

private:
	_uint				m_iMaxParticleNum;
	_uint				m_iParticleNum;
	_uint				m_iShape;

	_float				m_fDuration;
	_float				m_fShot;
	_float				m_fDurationTime = 0.f;
	_float				m_fShotTime = 0.f;
	_float				m_fLifeTime[2];
	_float				m_fSpeed[2];
	_float2				m_vParticleSize[2];

	_float				m_fAngle;
	_float				m_fRadius;
	_float				m_fCircleAngle;
	_float				m_fCircleY;

	_float3				m_vSize;
	_float3				m_vParticleRotation[2];

	_float4				m_vColor;

	VTXNEWPARTICLE*		m_pParticleData = nullptr;

public:
	static CVIBuffer_NewParticle_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END