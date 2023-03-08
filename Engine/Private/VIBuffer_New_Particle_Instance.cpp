#include "..\Public\VIBuffer_New_Particle_Instance.h"

CVIBuffer_NewParticle_Instance::CVIBuffer_NewParticle_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instance(pDevice, pContext)
{
}

CVIBuffer_NewParticle_Instance::CVIBuffer_NewParticle_Instance(const CVIBuffer_NewParticle_Instance & rhs)
	: CVIBuffer_Instance(rhs)
{
}

HRESULT CVIBuffer_NewParticle_Instance::Initialize_Prototype(_uint iNumInstance)
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

HRESULT CVIBuffer_NewParticle_Instance::Initialize(void * pArg)
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

	m_iInstanceStride = sizeof(VTXNEWPARTICLE);
	m_iNumVertices = m_iNumInstance;
	m_iNumIndicesPerInstance = 1;

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	VTXNEWPARTICLE*			pInstanceVtx = new VTXNEWPARTICLE[m_iNumVertices];


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVtx[i].vRight = _float4(0.1f, 0.f, 0.f, 0.f);
		pInstanceVtx[i].vUp = _float4(0.f, 0.1f, 0.f, 0.f);
		pInstanceVtx[i].vLook = _float4(0.f, 0.f, 0.1f, 0.f);
		pInstanceVtx[i].vPosition = _float4(0.f, 0.f, 0.1f, 0.f);
		pInstanceVtx[i].fLifeTime = 0.f;
		pInstanceVtx[i].fSpeed = 0.f;
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstanceVtx;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pInstanceBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVtx);
#pragma endregion

	m_pParticleData = new VTXNEWPARTICLE[m_iNumInstance];
	ZeroMemory(m_pParticleData, sizeof(VTXNEWPARTICLE) * m_iNumInstance);

	m_matWorld = new _float4x4[m_iNumInstance];
	ZeroMemory(m_matWorld, sizeof(_float4x4) * m_iNumInstance);

	m_vGo = new _float4[m_iNumInstance];
	ZeroMemory(m_vGo, sizeof(_float4) * m_iNumInstance);

	return S_OK;
}

void CVIBuffer_NewParticle_Instance::Update(_float fTimeDelta, _float2 fScaleReduction, _float4x4 ParentMtr, _float fSpeedReduction, _float fGravity,
	_bool bSpeedKill, _matrix matParent)
{
	random_device rd;
	mt19937 Seed(rd());

	uniform_real_distribution<float> LifeTimeRand(m_fLifeTime[0], m_fLifeTime[1]);

	m_fDurationTime += fTimeDelta;
	m_fShotTime += fTimeDelta;

	if (m_fDurationTime < m_fDuration) {
		if (m_fShotTime > m_fShot) {
			m_fShotTime = 0.f;

			_int iNum = -1;

			for (_uint i = 0; i < m_iParticleNum; ++i) {
				for (_uint j = 0; j < m_iMaxParticleNum; ++j) {
					if (m_pParticleData[j].fLifeTime == 0) {
						iNum = j;
						break;
					}
				}
				if (iNum != -1) {
					m_pParticleData[iNum].fLifeTime = LifeTimeRand(Seed);
					XMStoreFloat4x4(&m_matWorld[iNum], matParent);
				}
			}
		}
	}

	for (_uint i = 0; i < m_iMaxParticleNum; ++i) {
		if (m_pParticleData[i].fLifeTime != 0.f) {
			m_pParticleData[i].fTime += fTimeDelta;

			_vector		vPosition = XMLoadFloat4(&m_pParticleData[i].vPosition);
			_vector		vUp = XMLoadFloat4(&m_vGo[i]);

			m_pParticleData[i].fSpeed *= fSpeedReduction;

			vPosition += XMVector3Normalize(vUp) * m_pParticleData[i].fSpeed * fTimeDelta;

			_vector vGravityDriction = XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMMatrixInverse(nullptr, XMLoadFloat4x4(&ParentMtr)));

			m_pParticleData[i].fGravity -= fGravity;

			vPosition += vGravityDriction * m_pParticleData[i].fGravity * fTimeDelta;

			m_pParticleData[i].vSize.x *= fScaleReduction.x;
			m_pParticleData[i].vSize.y *= fScaleReduction.y;

			if (m_bGravityTurn) {
				_vector vUpGravity = XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vPosition) - vPosition);
				_vector	vAxisY = XMVectorSet(0.f, 0.f, 1.f, 0.f);

				_vector	vRight = XMVector3Cross(vAxisY, vUpGravity);

				XMStoreFloat4(&m_pParticleData[i].vRight, vRight);
				XMStoreFloat4(&m_pParticleData[i].vUp, vUpGravity);
			}

			XMStoreFloat4(&m_pParticleData[i].vPosition, vPosition);

			if (m_pParticleData[i].fTime > m_pParticleData[i].fLifeTime ||
				(bSpeedKill && m_pParticleData[i].fSpeed < 0.5f)) {
				Reset_One(i, matParent);
			}

			XMStoreFloat4(&m_pParticleData[i].vRight, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vRight)) * m_pParticleData[i].vSize.x);
			XMStoreFloat4(&m_pParticleData[i].vUp, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vUp)) * m_pParticleData[i].vSize.y);
			XMStoreFloat4(&m_pParticleData[i].vLook, XMVector3Normalize(XMLoadFloat4(&m_pParticleData[i].vLook)));
		}
	}

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iMaxParticleNum; ++i)
	{
		if (m_pParticleData[i].fLifeTime == 0.f) {
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vSize = _float2(0.f, 0.f);
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vRight = _float4(0.f, 0.f, 0.f, 0.f);
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vUp = _float4(0.f, 0.f, 0.f, 0.f);
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vLook = _float4(0.f, 0.f, 0.f, 0.f);

			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fLifeTime = 0.f;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fTime = 0.f;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fSpeed = 0.f;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fGravity = 0.f;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
		}
		else {
			if (m_bFollow) {
				XMStoreFloat4(&((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vPosition,
					XMVector3TransformCoord(XMLoadFloat4(&m_pParticleData[i].vPosition), XMLoadFloat4x4(&m_matWorld[i])));
			}
			else {
				((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vPosition = m_pParticleData[i].vPosition;
			}
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vSize = m_pParticleData[i].vSize;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vRight = m_pParticleData[i].vRight;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vUp = m_pParticleData[i].vUp;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vLook = m_pParticleData[i].vLook;

			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fLifeTime = m_pParticleData[i].fLifeTime;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fTime = m_pParticleData[i].fTime;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fSpeed = m_pParticleData[i].fSpeed;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fGravity = m_pParticleData[i].fGravity;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vColor = m_pParticleData[i].vColor;
		}
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

void CVIBuffer_NewParticle_Instance::Reset(_uint iMaxParticleNumber, _float* fLifeTime, _float* fSpeed, _float2* vTexScale, _uint iParticleType,
	_float fAngle, _float fRadius, _float3 vSize, _float3* vRotation, _float4 vColor, _float fDuration, _float fShotTime, _uint iOneParticleNumber,
	_float fCircleY, _float fCircleAngle, _bool bFollow, _bool bGravityTurn, _matrix matParent)
{
	m_iMaxParticleNum = iMaxParticleNumber;
	m_fDuration = fDuration;
	m_fShot = fShotTime;
	m_iParticleNum = iOneParticleNumber;
	m_fDurationTime = 0.f;
	m_fShotTime = 0.f;
	m_fLifeTime[0] = fLifeTime[0];
	m_fLifeTime[1] = fLifeTime[1];
	m_fSpeed[0] = fSpeed[0];
	m_fSpeed[1] = fSpeed[1];
	m_vParticleSize[0] = vTexScale[0];
	m_vParticleSize[1] = vTexScale[1];
	m_fAngle = fAngle;
	m_fRadius = fRadius;
	m_vSize = vSize;
	m_vParticleRotation[0] = vRotation[0];
	m_vParticleRotation[1] = vRotation[1];
	m_vColor = vColor;
	m_iShape = iParticleType;
	m_fCircleAngle = fCircleAngle;
	m_fCircleY = fCircleY;
	m_bFollow = bFollow;
	m_bGravityTurn = bGravityTurn;

	random_device rd;
	mt19937 Seed(rd());

	uniform_real_distribution<float> Dgree360(0, 360);
	uniform_real_distribution<float> Dgree20(0, 5);
	uniform_real_distribution<float> LifeTimeRand(fLifeTime[0], fLifeTime[1]);
	uniform_real_distribution<float> SpeedRand(fSpeed[0], fSpeed[1]);
	uniform_real_distribution<float> CircleAngleRand(0, m_fCircleAngle);
	uniform_real_distribution<float> CircleYRand(0, m_fCircleY);
	uniform_real_distribution<float> TexSizeXRand(vTexScale[0].x, vTexScale[1].x);
	uniform_real_distribution<float> TexSizeYRand(vTexScale[0].y, vTexScale[1].y);
	uniform_real_distribution<float> AngleRand(-1.f * (fAngle / 2.f), (fAngle / 2.f));
	uniform_real_distribution<float> BoxSizeXRand(0, vSize.x);
	uniform_real_distribution<float> BoxSizeYRand(0, vSize.y);
	uniform_real_distribution<float> BoxSizeZRand(0, vSize.z);
	uniform_real_distribution<float> RotationXRand(vRotation[0].x, vRotation[1].x);
	uniform_real_distribution<float> RotationYRand(vRotation[0].y, vRotation[1].y);
	uniform_real_distribution<float> RotationZRand(vRotation[0].z, vRotation[1].z);

	for (_uint i = 0; i < m_iMaxParticleNum; ++i)
	{
		m_pParticleData[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pParticleData[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		m_pParticleData[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		m_pParticleData[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		m_pParticleData[i].fLifeTime = 0.f;
		m_pParticleData[i].fGravity = 0.f;
		m_pParticleData[i].fTime = 0.f;
		m_pParticleData[i].fSpeed = SpeedRand(Seed);
		m_pParticleData[i].vSize = _float2(TexSizeXRand(Seed), TexSizeYRand(Seed));
		m_pParticleData[i].vColor = vColor;

		_matrix RotationMatrix, PositionMatrix, ScaleMatrix, RevolveMatrix, WorldMatrix;

		RotationMatrix = XMMatrixIdentity();
		PositionMatrix = XMMatrixIdentity();
		WorldMatrix = XMMatrixIdentity();
		ScaleMatrix = XMMatrixIdentity();
		RevolveMatrix = XMMatrixIdentity();

		ScaleMatrix = XMMatrixScaling(m_pParticleData[i].vSize.x, m_pParticleData[i].vSize.y, 1.f);

		if (iParticleType == 0) { // 구
			_float x = Dgree360(Seed);
			_float y = Dgree360(Seed);
			_float z = Dgree360(Seed);

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vLook), XMConvertToRadians(z));
		}
		else if (iParticleType == 1) { // 원
			_float Revolvez = CircleAngleRand(Seed);

			_float x = RotationXRand(Seed);
			_float y = RotationYRand(Seed);
			_float z = RotationZRand(Seed);

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vLook), XMConvertToRadians(z));

			RevolveMatrix = XMMatrixRotationZ(XMConvertToRadians(Revolvez));
			PositionMatrix = XMMatrixTranslation(0.f, fRadius, CircleYRand(Seed));
		}
		else if (iParticleType == 2) { // 상자
			_float fSizeX = BoxSizeXRand(Seed);
			_float fSizeY = BoxSizeYRand(Seed);
			_float fSizeZ = BoxSizeZRand(Seed);

			PositionMatrix = XMMatrixTranslation(fSizeX, fSizeY, fSizeZ);

			_float x = RotationXRand(Seed);
			_float y = RotationYRand(Seed);
			_float z = RotationZRand(Seed);

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vLook), XMConvertToRadians(z));
		}
		else if (iParticleType == 3) { // 콘
			_float x = AngleRand(Seed);
			_float y = Dgree360(Seed);
			_float z = AngleRand(Seed);

			RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vRight), XMConvertToRadians(x)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vUp), XMConvertToRadians(y)) *
				XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[i].vLook), XMConvertToRadians(z));
		}

		XMStoreFloat4x4(&m_matWorld[i], matParent);

		WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix * RevolveMatrix;

		XMStoreFloat4(&m_pParticleData[i].vRight, WorldMatrix.r[0]);
		XMStoreFloat4(&m_pParticleData[i].vUp, WorldMatrix.r[1]);
		XMStoreFloat4(&m_vGo[i], WorldMatrix.r[1]);
		XMStoreFloat4(&m_pParticleData[i].vLook, WorldMatrix.r[2]);
		XMStoreFloat4(&m_pParticleData[i].vPosition, WorldMatrix.r[3]);
	}

	for (_uint i = 0; i < m_iParticleNum; ++i)
	{
		m_pParticleData[i].fLifeTime = LifeTimeRand(Seed);
	}

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		if (m_pParticleData[i].fLifeTime == 0.f) {
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vSize = _float2(0.f, 0.f);
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vRight = _float4(0.f, 0.f, 0.f, 0.f);
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vUp = _float4(0.f, 0.f, 0.f, 0.f);
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vLook = _float4(0.f, 0.f, 0.f, 0.f);

			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fLifeTime = 0.f;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fTime = 0.f;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fSpeed = 0.f;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fGravity = 0.f;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
		}
		else {
			if (m_bFollow) {
				XMStoreFloat4(&((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vPosition,
					XMVector3TransformCoord(XMLoadFloat4(&m_pParticleData[i].vPosition), XMLoadFloat4x4(&m_matWorld[i])));
			}
			else {
				((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vPosition = m_pParticleData[i].vPosition;
			}
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vSize = m_pParticleData[i].vSize;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vRight = m_pParticleData[i].vRight;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vUp = m_pParticleData[i].vUp;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vLook = m_pParticleData[i].vLook;

			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fLifeTime = m_pParticleData[i].fLifeTime;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fTime = m_pParticleData[i].fTime;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fSpeed = m_pParticleData[i].fSpeed;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].fGravity = m_pParticleData[i].fGravity;
			((VTXNEWPARTICLE*)MappedSubResource.pData)[i].vColor = m_pParticleData[i].vColor;
		}
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

void CVIBuffer_NewParticle_Instance::Reset_One(_uint iNum, _matrix matParent)
{
	random_device rd;
	mt19937 Seed(rd());

	uniform_real_distribution<float> Dgree360(0, 360);
	uniform_real_distribution<float> Dgree20(0, 5);
	uniform_real_distribution<float> CircleAngleRand(0, m_fCircleAngle);
	uniform_real_distribution<float> CircleYRand(0, m_fCircleY);
	uniform_real_distribution<float> LifeTimeRand(m_fLifeTime[0], m_fLifeTime[1]);
	uniform_real_distribution<float> SpeedRand(m_fSpeed[0], m_fSpeed[1]);
	uniform_real_distribution<float> TexSizeXRand(m_vParticleSize[0].x, m_vParticleSize[1].x);
	uniform_real_distribution<float> TexSizeYRand(m_vParticleSize[0].y, m_vParticleSize[1].y);
	uniform_real_distribution<float> AngleRand(-1.f * (m_fAngle / 2.f), (m_fAngle / 2.f));
	uniform_real_distribution<float> BoxSizeXRand(0, m_vSize.x);
	uniform_real_distribution<float> BoxSizeYRand(0, m_vSize.y);
	uniform_real_distribution<float> BoxSizeZRand(0, m_vSize.z);
	uniform_real_distribution<float> RotationXRand(m_vParticleRotation[0].x, m_vParticleRotation[1].x);
	uniform_real_distribution<float> RotationYRand(m_vParticleRotation[0].y, m_vParticleRotation[1].y);
	uniform_real_distribution<float> RotationZRand(m_vParticleRotation[0].z, m_vParticleRotation[1].z);

	m_pParticleData[iNum].vRight = _float4(1.f, 0.f, 0.f, 0.f);
	m_pParticleData[iNum].vUp = _float4(0.f, 1.f, 0.f, 0.f);
	m_pParticleData[iNum].vLook = _float4(0.f, 0.f, 1.f, 0.f);
	m_pParticleData[iNum].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	m_pParticleData[iNum].fLifeTime = 0.f;
	m_pParticleData[iNum].fGravity = 0.f;
	m_pParticleData[iNum].fTime = 0.f;
	m_pParticleData[iNum].fSpeed = SpeedRand(Seed);
	m_pParticleData[iNum].vSize = _float2(TexSizeXRand(Seed), TexSizeYRand(Seed));

	_matrix RotationMatrix, PositionMatrix, ScaleMatrix, RevolveMatrix, WorldMatrix;

	RotationMatrix = XMMatrixIdentity();
	PositionMatrix = XMMatrixIdentity();
	WorldMatrix = XMMatrixIdentity();
	ScaleMatrix = XMMatrixIdentity();
	RevolveMatrix = XMMatrixIdentity();

	ScaleMatrix = XMMatrixScaling(m_pParticleData[iNum].vSize.x, m_pParticleData[iNum].vSize.y, 1.f);

	if (m_iShape == 0) { // 구
		_float x = Dgree360(Seed);
		_float y = Dgree360(Seed);
		_float z = Dgree360(Seed);

		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vRight), XMConvertToRadians(x)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vUp), XMConvertToRadians(y)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vLook), XMConvertToRadians(z));
	}
	else if (m_iShape == 1) { // 원
		_float Revolvez = CircleAngleRand(Seed);

		_float x = RotationXRand(Seed);
		_float y = RotationYRand(Seed);
		_float z = RotationZRand(Seed);

		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vRight), XMConvertToRadians(x)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vUp), XMConvertToRadians(y)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vLook), XMConvertToRadians(z));

		RevolveMatrix = XMMatrixRotationZ(XMConvertToRadians(Revolvez));
		PositionMatrix = XMMatrixTranslation(0.f, m_fRadius, CircleYRand(Seed));
	}
	else if (m_iShape == 2) { // 상자
		_float fSizeX = BoxSizeXRand(Seed);
		_float fSizeY = BoxSizeYRand(Seed);
		_float fSizeZ = BoxSizeZRand(Seed);

		PositionMatrix = XMMatrixTranslation(fSizeX, fSizeY, fSizeZ);

		_float x = RotationXRand(Seed);
		_float y = RotationYRand(Seed);
		_float z = RotationZRand(Seed);

		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vRight), XMConvertToRadians(x)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vUp), XMConvertToRadians(y)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vLook), XMConvertToRadians(z));
	}
	else if (m_iShape == 3) { // 콘
		_float x = AngleRand(Seed);
		_float y = Dgree360(Seed);
		_float z = AngleRand(Seed);

		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vRight), XMConvertToRadians(x)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vUp), XMConvertToRadians(y)) *
			XMMatrixRotationAxis(XMLoadFloat4(&m_pParticleData[iNum].vLook), XMConvertToRadians(z));
	}

	XMStoreFloat4x4(&m_matWorld[iNum], matParent);

	WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix * RevolveMatrix;

	XMStoreFloat4(&m_pParticleData[iNum].vRight, WorldMatrix.r[0]);
	XMStoreFloat4(&m_pParticleData[iNum].vUp, WorldMatrix.r[1]);
	XMStoreFloat4(&m_vGo[iNum], WorldMatrix.r[1]);
	XMStoreFloat4(&m_pParticleData[iNum].vLook, WorldMatrix.r[2]);
	XMStoreFloat4(&m_pParticleData[iNum].vPosition, WorldMatrix.r[3]);

}

CVIBuffer_NewParticle_Instance * CVIBuffer_NewParticle_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance)
{
	CVIBuffer_NewParticle_Instance*	pInstance = new CVIBuffer_NewParticle_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_NewParticle_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_NewParticle_Instance::Clone(void * pArg)
{
	CVIBuffer_NewParticle_Instance*	pInstance = new CVIBuffer_NewParticle_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CVIBuffer_NewParticle_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_NewParticle_Instance::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pParticleData);

	Safe_Delete_Array(m_matWorld);
	Safe_Delete_Array(m_vGo);
}
