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
	void Update(_float fTimeDelta, _float3 fScale, _float4x4 ParentMtr, _float iSpeedType, _float fGravity, _float3 vScale,
		_bool bRoof, _float fSpeed, _uint iParticleType, _uint iSizeX, _uint iSizeY);
	void Reset(_float3 vScale, _float fLifeTime, _float fSpeed, _uint iParticleType, _float4x4 mtrParent, _uint iSizeX,
		_uint iSizeY, _bool bRoof);
	void Reset(_float* fLifeTime, _float* fSpeed, _float2* vTexScale, _float4x4 mtrParent, _uint iParticleType,
		_uint iPartSizeX, _uint iPartSizeY, _float fDirectionX, _float fDirectionY);

private:
	_float*				m_fTime = nullptr;
	VTXPARTICLE*		m_pParticleData = nullptr;

public:
	static CVIBuffer_Particle_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END