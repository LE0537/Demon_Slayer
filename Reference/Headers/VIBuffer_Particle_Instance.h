#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL  CVIBuffer_Particle_Instance final : public CVIBuffer_Instance
{
private:
	CVIBuffer_Particle_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle_Instance(const CVIBuffer_Particle_Instance& rhs);
	virtual ~CVIBuffer_Particle_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance) override;
	virtual HRESULT Initialize(void* pArg) override;
public:
	void Update(_float fTimeDelta, _float2 fScale, _float4x4 ParentMtr, _float * fLifeTime, _float iSpeedType, _float fGravity);

	void Update(_float fTimeDelta, _float2 fScale, _float2* vTexScale, _float4x4 ParentMtr, _float* fLifeTime, _float iSpeedType, _float fGravity,
		_float* fSpeed, _uint iParticleType, _uint iSizeX, _uint iSizeY, _float fDirectionX, _float fDirectionY, _float fRemaintingTime);

	void Reset(_float* fLifeTime, _float* fSpeed, _float2* vTexScale, _float4x4 mtrParent, _uint iParticleType,
		_uint iPartSizeX, _uint iPartSizeY, _float fDirectionX, _float fDirectionY, _float StartTurm);

	void Reset_One(_uint iNum, _float2* vTexScale, _float4x4 ParentMtr, _float* fLifeTime, _float* fSpeed, _uint iParticleType, _uint iSizeX, _uint iSizeY,
		_float fDirectionX, _float fDirectionY);

private:
	_float*				m_fTime = nullptr;
	VTXPARTICLE*		m_pParticleData = nullptr;

public:
	static CVIBuffer_Particle_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END