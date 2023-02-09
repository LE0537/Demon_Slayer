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

	return S_OK;

}

HRESULT CVIBuffer_Particle_Instance::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

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

	m_iInstanceStride = sizeof(VTXPARTICLE);
	m_iNumVertices = m_iNumInstance;
	m_iNumIndicesPerInstance = 1;

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	VTXPARTICLE*			pInstanceVtx = new VTXPARTICLE[m_iNumVertices];


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

	_float4 vSize = *(_float4*)pArg;

	m_pParticleData = new VTXPARTICLE[m_iNumInstance];
	ZeroMemory(m_pParticleData, sizeof(VTXPARTICLE) * m_iNumInstance);

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	m_fTime = new _float[m_iNumInstance];
	ZeroMemory(m_fTime, sizeof(_float) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(rand() % 360)) *
			XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(rand() % 360)) *
			XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(rand() % 360));

		((VTXPARTICLE*)MappedSubResource.pData)[i].fLifeTime = rand() % (int)vSize.w * 100 % 100;
		m_fTime[i] = 0.f;

		((VTXPARTICLE*)MappedSubResource.pData)[i].fSpeed = rand() % 10 + 1;

		XMStoreFloat4(&((VTXPARTICLE*)MappedSubResource.pData)[i].vLook,
			XMVector3TransformNormal(XMLoadFloat4(&((VTXPARTICLE*)MappedSubResource.pData)[i].vLook), RotationMatrix)
		);
		XMStoreFloat4(&((VTXPARTICLE*)MappedSubResource.pData)[i].vRight,
			XMVector3TransformNormal(XMLoadFloat4(&((VTXPARTICLE*)MappedSubResource.pData)[i].vRight), RotationMatrix)
		);
		XMStoreFloat4(&((VTXPARTICLE*)MappedSubResource.pData)[i].vUp,
			XMVector3TransformNormal(XMLoadFloat4(&((VTXPARTICLE*)MappedSubResource.pData)[i].vUp), RotationMatrix)
		);
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);

	return S_OK;
}

void CVIBuffer_Particle_Instance::Update(_float fTimeDelta, _float2 fScale, _float4x4 ParentMtr, _float * fLifeTime, _float iSpeedType, _float fGravity)
{
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_fTime[i] += fTimeDelta;

		if (m_pParticleData[i].vSize.x < 0 || m_pParticleData[i].vSize.y < 0) {
			m_pParticleData[i].vSize.x = 0.f;
			m_pParticleData[i].vSize.y = 0.f;
		}

		if (m_pParticleData[i].fSpeed < 0) {
			m_pParticleData[i].fSpeed = 0.f;

			m_pParticleData[i].vSize.x = 0;
			m_pParticleData[i].vSize.y = 0;
		}

		_vector		vPosition = XMLoadFloat4(&m_pParticleData[i].vPosition);
		_vector		vUp = XMLoadFloat4(&m_pParticleData[i].vUp);

		if (m_pParticleData[i].fStartTime <= m_fTime[i]) {
			m_pParticleData[i].vSize.x *= fScale.x;
			m_pParticleData[i].vSize.y *= fScale.y;

			vPosition += XMVector3Normalize(vUp) * m_pParticleData[i].fSpeed * fTimeDelta;

			_vector vGravityDriction = XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMMatrixInverse(nullptr, XMLoadFloat4x4(&ParentMtr)));

			m_pParticleData[i].fGravity -= fGravity;

			vPosition += vGravityDriction * m_pParticleData[i].fGravity * fTimeDelta;

			m_pParticleData[i].fSpeed += 0.01f * iSpeedType;
		}

		if (m_pParticleData[i].fLifeTime < m_fTime[i]) {
			m_pParticleData[i].vSize.x = 0;
			m_pParticleData[i].vSize.y = 0;
		}

		XMStoreFloat4(&m_pParticleData[i].vPosition, vPosition);

		XMStoreFloat4(&m_pParticleData[i].vRight, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vRight)) * m_pParticleData[i].vSize.x);
		XMStoreFloat4(&m_pParticleData[i].vUp, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vUp)) * m_pParticleData[i].vSize.y);
		XMStoreFloat4(&m_pParticleData[i].vLook, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vLook)));
	}

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXPARTICLE*)MappedSubResource.pData)[i].fLifeTime = m_pParticleData[i].fLifeTime;
		((VTXPARTICLE*)MappedSubResource.pData)[i].fStartTime = m_pParticleData[i].fStartTime;
		((VTXPARTICLE*)MappedSubResource.pData)[i].fSpeed = m_pParticleData[i].fSpeed;
		((VTXPARTICLE*)MappedSubResource.pData)[i].vPosition = m_pParticleData[i].vPosition;

		if (m_pParticleData[i].fStartTime <= m_fTime[i]) {
			((VTXPARTICLE*)MappedSubResource.pData)[i].vSize = m_pParticleData[i].vSize;
			((VTXPARTICLE*)MappedSubResource.pData)[i].vRight = m_pParticleData[i].vRight;
			((VTXPARTICLE*)MappedSubResource.pData)[i].vUp = m_pParticleData[i].vUp;
			((VTXPARTICLE*)MappedSubResource.pData)[i].vLook = m_pParticleData[i].vLook;
		}
		else {
			((VTXPARTICLE*)MappedSubResource.pData)[i].vSize = _float2(0.f, 0.f);
			((VTXPARTICLE*)MappedSubResource.pData)[i].vRight = _float4(0.f, 0.f, 0.f, 0.f);
			((VTXPARTICLE*)MappedSubResource.pData)[i].vUp = _float4(0.f, 0.f, 0.f, 0.f);
			((VTXPARTICLE*)MappedSubResource.pData)[i].vLook = _float4(0.f, 0.f, 0.f, 0.f);

		}
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

void CVIBuffer_Particle_Instance::Update(_float fTimeDelta, _float2 fScale, _float2 * vTexScale, _float4x4 ParentMtr, _float * fLifeTime, _float iSpeedType, _float fGravity,
	_float * fSpeed, _uint iParticleType, _uint iSizeX, _uint iSizeY, _float fDirectionX, _float fDirectionY, _float fRemaintingTime)
{
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_fTime[i] += fTimeDelta;

		if ((m_pParticleData[i].fLifeTime < m_fTime[i] || m_pParticleData[i].fSpeed < 0 || m_pParticleData[i].vSize.x < 0 || m_pParticleData[i].vSize.y < 0)) {
			if (fRemaintingTime > fLifeTime[1]) {
				Reset_One(i, vTexScale, ParentMtr, fLifeTime, fSpeed, iParticleType, iSizeX, iSizeY, fDirectionX, fDirectionY);
			}
			else {
				m_pParticleData[i].vSize.x = 0;
				m_pParticleData[i].vSize.y = 0;
			}
		}

		_vector		vPosition = XMLoadFloat4(&m_pParticleData[i].vPosition);
		_vector		vUp = XMLoadFloat4(&m_pParticleData[i].vUp);

		if (m_pParticleData[i].fStartTime <= m_fTime[i]) {
			m_pParticleData[i].vSize.x *= fScale.x;
			m_pParticleData[i].vSize.y *= fScale.y;

			vPosition += XMVector3Normalize(vUp) * m_pParticleData[i].fSpeed * fTimeDelta;

			_vector vGravityDriction = XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMMatrixInverse(nullptr, XMLoadFloat4x4(&ParentMtr)));

			m_pParticleData[i].fGravity -= fGravity;

			vPosition += vGravityDriction * m_pParticleData[i].fGravity * fTimeDelta;

			m_pParticleData[i].fSpeed += 0.01f * iSpeedType;
		}

		XMStoreFloat4(&m_pParticleData[i].vPosition, vPosition);

		XMStoreFloat4(&m_pParticleData[i].vRight, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vRight)) * m_pParticleData[i].vSize.x);
		XMStoreFloat4(&m_pParticleData[i].vUp, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vUp)) * m_pParticleData[i].vSize.y);
		XMStoreFloat4(&m_pParticleData[i].vLook, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vLook)));
	}

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXPARTICLE*)MappedSubResource.pData)[i].fLifeTime = m_pParticleData[i].fLifeTime;
		((VTXPARTICLE*)MappedSubResource.pData)[i].fStartTime = m_pParticleData[i].fStartTime;
		((VTXPARTICLE*)MappedSubResource.pData)[i].fSpeed = m_pParticleData[i].fSpeed;
		((VTXPARTICLE*)MappedSubResource.pData)[i].vPosition = m_pParticleData[i].vPosition;

		if (m_pParticleData[i].fStartTime <= m_fTime[i]) {
			((VTXPARTICLE*)MappedSubResource.pData)[i].vSize = m_pParticleData[i].vSize;
			((VTXPARTICLE*)MappedSubResource.pData)[i].vRight = m_pParticleData[i].vRight;
			((VTXPARTICLE*)MappedSubResource.pData)[i].vUp = m_pParticleData[i].vUp;
			((VTXPARTICLE*)MappedSubResource.pData)[i].vLook = m_pParticleData[i].vLook;
		}
		else {
			((VTXPARTICLE*)MappedSubResource.pData)[i].vSize = _float2(0.f, 0.f);
			((VTXPARTICLE*)MappedSubResource.pData)[i].vRight = _float4(0.f, 0.f, 0.f, 0.f);
			((VTXPARTICLE*)MappedSubResource.pData)[i].vUp = _float4(0.f, 0.f, 0.f, 0.f);
			((VTXPARTICLE*)MappedSubResource.pData)[i].vLook = _float4(0.f, 0.f, 0.f, 0.f);

		}
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

void CVIBuffer_Particle_Instance::Reset(_float * fLifeTime, _float * fSpeed, _float2 * vTexScale, _float4x4 mtrParent, _uint iParticleType, _uint iPartSizeX,
	_uint iPartSizeY, _float fDirectionX, _float fDirectionY, _float StartTurm)
{
	random_device rd;
	mt19937 Seed(rd());

	_float Min = -1.f * (iPartSizeX / 2.f);
	_float Max = -1.f * (iPartSizeY / 2.f);

	uniform_real_distribution<float> Dgree360(0, 360);
	uniform_real_distribution<float> Dgree20(0, 5);
	uniform_real_distribution<float> LifeTimeRand(fLifeTime[0], fLifeTime[1]);
	uniform_real_distribution<float> StartTimeRand(0, StartTurm);
	uniform_real_distribution<float> PartSizeXRand(0, iPartSizeX);
	uniform_real_distribution<float> PartSizeYRand(0, iPartSizeY);
	uniform_real_distribution<float> ConeSizeXRand(Min, iPartSizeX / 2);
	uniform_real_distribution<float> ConeSizeYRand(Max, iPartSizeY / 2);
	uniform_real_distribution<float> DirectionRand(fDirectionX, fDirectionY);
	uniform_real_distribution<float> SpeedRand(fSpeed[0], fSpeed[1]);
	uniform_real_distribution<float> TexSizeXRand(vTexScale[0].x, vTexScale[1].x);
	uniform_real_distribution<float> TexSizeYRand(vTexScale[0].y, vTexScale[1].y);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_pParticleData[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pParticleData[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		m_pParticleData[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		m_pParticleData[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);

		_matrix		RotationMatrix, PositionMatrix, ScaleMatrix, WorldMatrix;

		RotationMatrix = XMMatrixIdentity();
		PositionMatrix = XMMatrixIdentity();
		WorldMatrix = XMMatrixIdentity();
		ScaleMatrix = XMMatrixIdentity();

		m_pParticleData[i].fLifeTime = LifeTimeRand(Seed);
		m_fTime[i] = 0.f;
		m_pParticleData[i].fGravity = 0.f;

		m_pParticleData[i].fSpeed = SpeedRand(Seed);

		m_pParticleData[i].vSize = _float2(TexSizeXRand(Seed), TexSizeYRand(Seed));

		m_pParticleData[i].fStartTime = StartTimeRand(Seed);

		ScaleMatrix = XMMatrixScaling(m_pParticleData[i].vSize.x, m_pParticleData[i].vSize.y, 1.f);

		if (iParticleType == 0) {
			_float x = Dgree360(Seed);
			_float y = Dgree360(Seed);
			_float z = Dgree360(Seed);

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vLook), XMConvertToRadians(z));
		}
		else if (iParticleType == 1) {
			_float z = Dgree360(Seed);
			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&(_float4)mtrParent.m[2]), XMConvertToRadians(z));
		}
		else if (iParticleType == 2) {
			_float fSizeX = PartSizeXRand(Seed);
			_float fSizeZ = PartSizeYRand(Seed);

			PositionMatrix = XMMatrixTranslation(fSizeX, 0.f, fSizeZ);

			_float x = Dgree20(Seed);
			_float y = Dgree360(Seed);
			_float z = Dgree20(Seed);

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vLook), XMConvertToRadians(z));
		}
		else if (iParticleType == 3) {
			_float x = ConeSizeXRand(Seed);
			_float y = Dgree360(Seed);
			_float z = ConeSizeYRand(Seed);

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vLook), XMConvertToRadians(z));
		}
		else if (iParticleType == 4) {
			_float fSizeX = PartSizeXRand(Seed);
			_float fSizeZ = PartSizeYRand(Seed);

			PositionMatrix = XMMatrixTranslation(fSizeX, 0.f, fSizeZ);

			_float x = 0;
			_float y = Dgree360(Seed);
			_float z = DirectionRand(Seed);

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vLook), XMConvertToRadians(z));
		}

		WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix;

		XMStoreFloat4(&m_pParticleData[i].vRight, WorldMatrix.r[0]);
		XMStoreFloat4(&m_pParticleData[i].vUp, WorldMatrix.r[1]);
		XMStoreFloat4(&m_pParticleData[i].vLook, WorldMatrix.r[2]);
		XMStoreFloat4(&m_pParticleData[i].vPosition, WorldMatrix.r[3]);
	}

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXPARTICLE*)MappedSubResource.pData)[i].fLifeTime = m_pParticleData[i].fLifeTime;
		((VTXPARTICLE*)MappedSubResource.pData)[i].fStartTime = m_pParticleData[i].fStartTime;
		((VTXPARTICLE*)MappedSubResource.pData)[i].fSpeed = m_pParticleData[i].fSpeed;
		((VTXPARTICLE*)MappedSubResource.pData)[i].vLook = m_pParticleData[i].vLook;
		((VTXPARTICLE*)MappedSubResource.pData)[i].vPosition = m_pParticleData[i].vPosition;
		((VTXPARTICLE*)MappedSubResource.pData)[i].vRight = m_pParticleData[i].vRight;
		((VTXPARTICLE*)MappedSubResource.pData)[i].vSize = m_pParticleData[i].vSize;
		((VTXPARTICLE*)MappedSubResource.pData)[i].vUp = m_pParticleData[i].vUp;
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

void CVIBuffer_Particle_Instance::Reset_One(_uint iNum, _float2 * vTexScale, _float4x4 ParentMtr, _float * fLifeTime, _float * fSpeed, _uint iParticleType, _uint iSizeX, _uint iSizeY,
	_float fDirectionX, _float fDirectionY)
{
	random_device rd;
	mt19937 Seed(rd());

	uniform_real_distribution<float> Dgree360(0, 360);
	uniform_real_distribution<float> Dgree20(0, 5);
	uniform_real_distribution<float> LifeTimeRand(fLifeTime[0], fLifeTime[1]);
	uniform_real_distribution<float> PartSizeXRand(0, iSizeX);
	uniform_real_distribution<float> PartSizeYRand(0, iSizeY);
	uniform_real_distribution<float> ConeSizeXRand(-1.f * (iSizeX / 2.f), iSizeX / 2.f);
	uniform_real_distribution<float> ConeSizeYRand(-1.f * (iSizeY / 2.f), iSizeY / 2.f);
	uniform_real_distribution<float> DirectionRand(fDirectionX, fDirectionY);
	uniform_real_distribution<float> SpeedRand(fSpeed[0], fSpeed[1]);
	uniform_real_distribution<float> TexSizeXRand(vTexScale[0].x, vTexScale[1].x);
	uniform_real_distribution<float> TexSizeYRand(vTexScale[0].y, vTexScale[1].y);

	m_pParticleData[iNum].vRight = _float4(1.f, 0.f, 0.f, 0.f);
	m_pParticleData[iNum].vUp = _float4(0.f, 1.f, 0.f, 0.f);
	m_pParticleData[iNum].vLook = _float4(0.f, 0.f, 1.f, 0.f);
	m_pParticleData[iNum].vPosition = _float4(0.f, 0.f, 0.f, 1.f);

	_matrix		RotationMatrix, PositionMatrix, ScaleMatrix, WorldMatrix;

	RotationMatrix = XMMatrixIdentity();
	PositionMatrix = XMMatrixIdentity();
	WorldMatrix = XMMatrixIdentity();
	ScaleMatrix = XMMatrixIdentity();

	m_pParticleData[iNum].fLifeTime = LifeTimeRand(Seed);

	m_fTime[iNum] = 0.f;

	m_pParticleData[iNum].fSpeed = SpeedRand(Seed);

	m_pParticleData[iNum].vSize = _float2(TexSizeXRand(Seed), TexSizeYRand(Seed));

	m_pParticleData[iNum].fStartTime = 0.f;

	ScaleMatrix = XMMatrixScaling(m_pParticleData[iNum].vSize.x, m_pParticleData[iNum].vSize.y, 1.f);

	if (iParticleType == 0) {
		_float x = Dgree360(Seed);
		_float y = Dgree360(Seed);
		_float z = Dgree360(Seed);

		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vRight), XMConvertToRadians(x)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vUp), XMConvertToRadians(y)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vLook), XMConvertToRadians(z));
	}
	else if (iParticleType == 1) {
		_float z = Dgree360(Seed);
		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&(_float4)ParentMtr.m[2]), XMConvertToRadians(z));
	}
	else if (iParticleType == 2) {
		_float fSizeX = PartSizeXRand(Seed);
		_float fSizeZ = PartSizeYRand(Seed);

		PositionMatrix = XMMatrixTranslation(fSizeX, 0.f, fSizeZ);

		_float x = Dgree20(Seed);
		_float y = Dgree360(Seed);
		_float z = Dgree20(Seed);

		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vRight), XMConvertToRadians(x)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vUp), XMConvertToRadians(y)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vLook), XMConvertToRadians(z));
	}
	else if (iParticleType == 3) {
		_float x = ConeSizeXRand(Seed);
		_float y = Dgree360(Seed);
		_float z = ConeSizeYRand(Seed);

		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vRight), XMConvertToRadians(x)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vUp), XMConvertToRadians(y)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vLook), XMConvertToRadians(z));
	}
	else if (iParticleType == 4) {
		_float fSizeX = PartSizeXRand(Seed);
		_float fSizeZ = PartSizeYRand(Seed);

		PositionMatrix = XMMatrixTranslation(fSizeX, 0.f, fSizeZ);

		_float x = 0;
		_float y = Dgree360(Seed);
		_float z = DirectionRand(Seed);

		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vRight), XMConvertToRadians(x)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vUp), XMConvertToRadians(y)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vLook), XMConvertToRadians(z));
	}

	WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix;

	XMStoreFloat4(&m_pParticleData[iNum].vRight, WorldMatrix.r[0]);
	XMStoreFloat4(&m_pParticleData[iNum].vUp, WorldMatrix.r[1]);
	XMStoreFloat4(&m_pParticleData[iNum].vLook, WorldMatrix.r[2]);
	XMStoreFloat4(&m_pParticleData[iNum].vPosition, WorldMatrix.r[3]);
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

	Safe_Delete_Array(m_fTime);

	Safe_Delete_Array(m_pParticleData);
}
