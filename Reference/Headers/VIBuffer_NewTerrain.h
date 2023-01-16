#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_NewTerrain final : public CVIBuffer
{
public:
	typedef struct tagTerrainDesc
	{
		_uint	iSizeX = 0;
		_uint	iSizeZ = 0;
		char strFileName[MAX_PATH] = "";
	}TERRAINDESC;

protected:
	CVIBuffer_NewTerrain(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_NewTerrain(const CVIBuffer_NewTerrain& rhs);
	virtual ~CVIBuffer_NewTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT		Load_Heightmap(TERRAINDESC* tTerrainDesc);
	
private:
	HRESULT		Check_Sphere_Pos(_float3 vDescPos, _float3 vSpherePos, _float fRadius, _float* fOut);

private:
	_uint				m_iNumVerticesX = 0, m_iNumVerticesZ = 0;
	VTXNORTEX*			m_pVertices = nullptr;
	FACEINDICES32*		m_pIndices = nullptr;
	
public:
	static CVIBuffer_NewTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
