#include "..\Public\VIBuffer_Particle_Instance.h"

CVIBuffer_Particle_Instance::CVIBuffer_Particle_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instance(pDevice, pContext)
{
}

CVIBuffer_Particle_Instance::CVIBuffer_Particle_Instance(const CVIBuffer_Particle_Instance & rhs)
	: CVIBuffer_Instance(rhs)
{
}

HRESULT CVIBuffer_Particle_Instance::Initialize_Prototype(_uint iNumInstance)
{
	if (FAILED(__super::Initialize_Prototype(iNumInstance)))
		return E_FAIL;

#pragma region VERTICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;
	m_iNumVertexBuffers = 2;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXPOINT*			pVertices = new VTXPOINT;
	ZeroMemory(pVertices, sizeof(VTXPOINT));

	pVertices->vPosition = _float3(0.0f, 0.f, 0.f);
	pVertices->vPSize = _float2(1.f, 1.f);
	
	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion


#pragma region Indices
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(_ushort);
	m_iNumPrimitive = m_iNumInstance;
	m_iNumIndicesPerPrimitive = 1;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	_ushort*			pIndices = new _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion


#pragma region INSTANCE_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iInstanceStride = sizeof(VTXMATRIX);
	m_iNumVertices = m_iNumInstance;
	m_iNumIndicesPerInstance = 1;

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	VTXMATRIX*			pInstanceVtx = new VTXMATRIX[m_iNumVertices];
	

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVtx[i].vRight = _float4(0.1f, 0.f, 0.f, 0.f);
		pInstanceVtx[i].vUp = _float4(0.f, 0.1f, 0.f, 0.f);
		pInstanceVtx[i].vLook = _float4(0.f, 0.f, 0.1f, 0.f);
		pInstanceVtx[i].vPosition = _float4(0.f, 0.f, 0.1f, 0.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstanceVtx;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pInstanceBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVtx);
#pragma endregion

	return S_OK;

}

HRESULT CVIBuffer_Particle_Instance::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float4 vSize = *(_float4*)pArg;

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	m_pSizePerSec = new _float3[m_iNumInstance];
	ZeroMemory(m_pSizePerSec, sizeof(_float) * m_iNumInstance);

	m_pSpeedPerSec = new _float[m_iNumInstance];
	ZeroMemory(m_pSpeedPerSec, sizeof(_float) * m_iNumInstance);

	m_pLifeTime = new _float[m_iNumInstance];
	ZeroMemory(m_pLifeTime, sizeof(_float) * m_iNumInstance);

	m_fTime = new _float[m_iNumInstance];
	ZeroMemory(m_fTime, sizeof(_float) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(rand() % 360)) *
			XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(rand() % 360)) *
			XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(rand() % 360));

		m_pLifeTime[i] = rand() % (int)vSize.w * 100 % 100;
		m_fTime[i] = 0.f;

		m_pSpeedPerSec[i] = rand() % 10 + 1;

		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook, 
			XMVector3TransformNormal(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), RotationMatrix) 
			);
		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight,
			XMVector3TransformNormal(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), RotationMatrix) 
			);
		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp,
			XMVector3TransformNormal(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), RotationMatrix) 
			);
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);

	return S_OK;
}

void CVIBuffer_Particle_Instance::Update(_float fTimeDelta, _float3 fScale, _float4x4 ParentMtr, _float iSpeedType, _float fGravity, 
	_float3 vScale, _bool bRoof, _float fSpeed, _uint iParticleType, _uint iSizeX, _uint iSizeY)
{

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_fTime[i] += fTimeDelta;

		_vector		vPosition = XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vPosition);
		_vector		vUp = XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp);

		m_pSpeedPerSec[i] += 0.01f * iSpeedType;

		if (m_pSpeedPerSec[i] < 0) {
			if (bRoof) {
				((VTXMATRIX*)MappedSubResource.pData)[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
				((VTXMATRIX*)MappedSubResource.pData)[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
				((VTXMATRIX*)MappedSubResource.pData)[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
				((VTXMATRIX*)MappedSubResource.pData)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);

				_matrix		RotationMatrix, PositionMatrix, ScaleMatrix, WorldMatrix;

				RotationMatrix = XMMatrixIdentity();
				PositionMatrix = XMMatrixIdentity();
				WorldMatrix = XMMatrixIdentity();
				ScaleMatrix = XMMatrixIdentity();

				int iRand = rand();

				m_pLifeTime[i] = rand() % 100 * 0.01f;

				m_fTime[i] = 0.f;

				m_pSpeedPerSec[i] = rand() % 50 * 0.1f + fSpeed - 2.5f;

				m_pSizePerSec[i] = _float3((rand() % 50 * 0.1f * vScale.x + vScale.x),
					(rand() % 50 * 0.1f * vScale.x + vScale.y),
					(rand() % 50 * 0.1f * vScale.x + vScale.z));

				ScaleMatrix = XMMatrixScaling(m_pSizePerSec[i].x, m_pSizePerSec[i].y, m_pSizePerSec[i].z);

				if (iParticleType == 0) {
					_float x = rand() % 360;
					_float y = rand() % 360;
					_float z = rand() % 360;

					RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
				}
				else if (iParticleType == 1) {
					_float z = rand() % 360;
					RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&(_float4)ParentMtr.m[2]), XMConvertToRadians(z));
				}
				else if (iParticleType == 2) {
					_float fSizeX = (rand() % 100) * 0.001f;
					_float fSizeZ = (rand() % 200) * 0.01f;

					PositionMatrix = XMMatrixTranslation(fSizeX, 0.f, fSizeZ);

					_float x = rand() % 10;
					_float y = rand() % 360;
					_float z = rand() % 20;

					RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
				}
				else if (iParticleType == 3) {
					_float x = rand() % iSizeX - iSizeX / 2;
					_float y = rand() % 360;
					_float z = rand() % iSizeY - iSizeY / 2;

					RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
				}

				WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix /** XMLoadFloat4x4(&mtrParent)*/;

				XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight, WorldMatrix.r[0]);
				XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp, WorldMatrix.r[1]);
				XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook, WorldMatrix.r[2]);
				vPosition = WorldMatrix.r[3];
			}
			else {
				m_pSpeedPerSec[i] = 0.f;

				m_pSizePerSec[i].x *= 0;
				m_pSizePerSec[i].y *= 0;
				m_pSizePerSec[i].z *= 0;
			}
		}

		m_pSizePerSec[i].x -= fScale.x;
		m_pSizePerSec[i].y -= fScale.y;
		m_pSizePerSec[i].z -= fScale.z;

		if (m_pLifeTime[i] < m_fTime[i]) {
			if (bRoof) {
				((VTXMATRIX*)MappedSubResource.pData)[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
				((VTXMATRIX*)MappedSubResource.pData)[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
				((VTXMATRIX*)MappedSubResource.pData)[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
				((VTXMATRIX*)MappedSubResource.pData)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);

				_matrix		RotationMatrix, PositionMatrix, ScaleMatrix, WorldMatrix;

				RotationMatrix = XMMatrixIdentity();
				PositionMatrix = XMMatrixIdentity();
				WorldMatrix = XMMatrixIdentity();
				ScaleMatrix = XMMatrixIdentity();

				int iRand = rand();

				m_pLifeTime[i] = rand() % 100 * 0.01f;
				
				m_fTime[i] = 0.f;

				m_pSpeedPerSec[i] = rand() % 50 * 0.1f + fSpeed - 2.5f;

				m_pSizePerSec[i] = _float3((rand() % 50 * 0.1f * vScale.x + vScale.x),
					(rand() % 50 * 0.1f * vScale.x + vScale.y),
					(rand() % 50 * 0.1f * vScale.x + vScale.z));

				ScaleMatrix = XMMatrixScaling(m_pSizePerSec[i].x, m_pSizePerSec[i].y, m_pSizePerSec[i].z);

				if (iParticleType == 0) {
					_float x = rand() % 360;
					_float y = rand() % 360;
					_float z = rand() % 360;

					RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
				}
				else if (iParticleType == 1) {
					_float z = rand() % 360;
					RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&(_float4)ParentMtr.m[2]), XMConvertToRadians(z));
				}
				else if (iParticleType == 2) {

					_float fSizeX = (rand() % iSizeX) * 0.01f;
					_float fSizeZ = (rand() % iSizeY) * 0.01f;

					//_float fSizeX = (rand() % 100) * 0.001f;
					//_float fSizeZ = (rand() % 200) * 0.01f;

					PositionMatrix = XMMatrixTranslation(fSizeX, 0.f, fSizeZ);

					_float x = rand() % 10;
					_float y = rand() % 360;
					_float z = rand() % 20;

					RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
				}
				else if (iParticleType == 3) {
					_float x = rand() % iSizeX - iSizeX / 2;
					_float y = rand() % 360;
					_float z = rand() % iSizeY - iSizeY / 2;

					RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
						XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
				}

				WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix /** XMLoadFloat4x4(&mtrParent)*/;

				XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight, WorldMatrix.r[0]);
				XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp, WorldMatrix.r[1]);
				XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook, WorldMatrix.r[2]);
				vPosition = WorldMatrix.r[3];
			}
			else {
				m_pSizePerSec[i].x *= 0;
				m_pSizePerSec[i].y *= 0;
				m_pSizePerSec[i].z *= 0;
			}
		}

		vPosition += XMVector3Normalize(vUp) * m_pSpeedPerSec[i] * fTimeDelta;
		vPosition += XMVectorSet(0.f, 1.f, 0.f, 0.f) * fGravity * fTimeDelta;

		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vPosition, vPosition);

		if (m_pSizePerSec[i].x < 0) {
			m_pSizePerSec[i].x = 0.f;
		}
		if (m_pSizePerSec[i].y < 0) {
			m_pSizePerSec[i].y = 0.f;
		}
		if (m_pSizePerSec[i].z < 0) {
			m_pSizePerSec[i].z = 0.f;
		}

		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook,
			XMVector3Normalize(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook)) * m_pSizePerSec[i].z);
		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight,
			XMVector3Normalize(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight)) * m_pSizePerSec[i].x);
		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp,
			XMVector3Normalize(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp)) * m_pSizePerSec[i].y);
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

void CVIBuffer_Particle_Instance::Reset(_float3 vScale, _float fLifeTime, _float fSpeed, _uint iParticleType,
	_float4x4 mtrParent, _uint iSizeX, _uint iSizeY, _bool bRoof)
{
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXMATRIX*)MappedSubResource.pData)[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		((VTXMATRIX*)MappedSubResource.pData)[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		((VTXMATRIX*)MappedSubResource.pData)[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		((VTXMATRIX*)MappedSubResource.pData)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);

		_matrix		RotationMatrix, PositionMatrix, ScaleMatrix, WorldMatrix;

		RotationMatrix = XMMatrixIdentity();
		PositionMatrix = XMMatrixIdentity();
		WorldMatrix = XMMatrixIdentity();
		ScaleMatrix = XMMatrixIdentity();

		int iRand = rand();
		
		if (bRoof) {
			m_pLifeTime[i] = rand() % 100 * 0.01f;
		}
		else {
			m_pLifeTime[i] = iRand % 20 * 0.1f + fLifeTime - 1.f;
		}
		m_fTime[i] = 0.f;

		m_pSpeedPerSec[i] = rand() % 50 * 0.1f + fSpeed - 2.5f;

		m_pSizePerSec[i] = _float3((rand() % 50 * 0.1f * vScale.x + vScale.x),
			(rand() % 50 * 0.1f * vScale.x + vScale.y),
			(rand() % 50 * 0.1f * vScale.x + vScale.z));

		ScaleMatrix = XMMatrixScaling(m_pSizePerSec[i].x, m_pSizePerSec[i].y, m_pSizePerSec[i].z);

		if (iParticleType == 0) {
			_float x = rand() % 360;
			_float y = rand() % 360;
			_float z = rand() % 360;

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
		}
		else if (iParticleType == 1) {
			_float z = rand() % 360;
			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&(_float4)mtrParent.m[2]), XMConvertToRadians(z));
		}
		else if (iParticleType == 2) {
			_float fSizeX = (rand() % iSizeX) * 0.01f;
			_float fSizeZ = (rand() % iSizeY) * 0.01f;

			PositionMatrix = XMMatrixTranslation(fSizeX, 0.f, fSizeZ);

			_float x = rand() % 10;
			_float y = rand() % 360;
			_float z = rand() % 20;

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
		}
		else if (iParticleType == 3) {
			_float x = rand() % iSizeX - iSizeX / 2;
			_float y = rand() % 360;
			_float z = rand() % iSizeY - iSizeY / 2;

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook), XMConvertToRadians(z));
		}

		WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix /** XMLoadFloat4x4(&mtrParent)*/;

		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vRight, WorldMatrix.r[0]);
		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vUp, WorldMatrix.r[1]);
		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vLook, WorldMatrix.r[2]);
		XMStoreFloat4(&((VTXMATRIX*)MappedSubResource.pData)[i].vPosition, WorldMatrix.r[3]);
	}
	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

CVIBuffer_Particle_Instance * CVIBuffer_Particle_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance)
{
	CVIBuffer_Particle_Instance*	pInstance = new CVIBuffer_Particle_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_Particle_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_Particle_Instance::Clone(void * pArg)
{
	CVIBuffer_Particle_Instance*	pInstance = new CVIBuffer_Particle_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CVIBuffer_Particle_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Particle_Instance::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pSpeedPerSec);
	Safe_Delete_Array(m_pLifeTime);
	Safe_Delete_Array(m_fTime);
	Safe_Delete_Array(m_pSizePerSec);
}
