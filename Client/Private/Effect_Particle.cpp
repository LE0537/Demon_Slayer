#include "stdafx.h"
#include "Effect_Particle.h"
#include "GameInstance.h"
#include "Effect.h"

CEffect_Particle::CEffect_Particle(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect_Particle::CEffect_Particle(const CEffect_Particle & rhs)
	: CGameObj(rhs)
{
}

HRESULT CEffect_Particle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Particle::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Particle::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > m_ParticleInfo.fStartTime && m_fTime < m_ParticleInfo.fLifeTime[1] + m_ParticleInfo.fStartTime) {

		_float2 fSize;

		if (m_ParticleInfo.bSizePix)
			fSize = _float2(1.f, 1.f);
		else
			fSize = _float2(m_ParticleInfo.fSizeFalloffX, m_ParticleInfo.fSizeFalloffY);

		if (m_ParticleInfo.bRoof) {
			m_pVIBufferCom->Update(fTimeDelta, fSize, m_ParticleInfo.vSize, m_CombinedWorldMatrix, m_ParticleInfo.fRoofTime, m_ParticleInfo.fSpeedType, m_ParticleInfo.fGravitySpeed, m_ParticleInfo.fSpeed,
				m_ParticleInfo.iParticleType, m_ParticleInfo.iConeSizeX, m_ParticleInfo.iConeSizeY, m_ParticleInfo.fDirectionX, m_ParticleInfo.fDirectionY, m_ParticleInfo.fLifeTime[1] - m_fTime);
		}
		else {
			m_pVIBufferCom->Update(fTimeDelta, fSize, m_CombinedWorldMatrix, m_ParticleInfo.fLifeTime, m_ParticleInfo.fSpeedType, m_ParticleInfo.fGravitySpeed);
		}
	}
}

void CEffect_Particle::Late_Tick(_float fTimeDelta)
{

	/*if (static_cast<CEffect*>(m_pParents)->Get_EffectMove() == CEffect::EFFMOVE_STOP) {
		if (m_fTime <= m_ParticleInfo.fStartTime) {
			_matrix mtrParents = XMLoadFloat4x4(&m_pParents->Get_CombinedWorldMatrix());
			XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);
			XMStoreFloat4x4(&m_ParentsMtr, mtrParents);
		}
		else {
			XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(&m_ParentsMtr));
		}
	}
	else {
		_matrix mtrParents = XMLoadFloat4x4(&m_pParents->Get_CombinedWorldMatrix());
		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);
	}*/

	if (m_fTime > m_ParticleInfo.fStartTime && m_fTime < m_ParticleInfo.fLifeTime[1] + m_ParticleInfo.fStartTime) {

		if (m_ParticleInfo.bRotationPix) {
			_matrix mtrParents = XMLoadFloat4x4(&m_pParents->Get_CombinedWorldMatrix());
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_ParticleInfo.vPosition.x, m_ParticleInfo.vPosition.y, m_ParticleInfo.vPosition.z, 0.f) + mtrParents.r[3]);

			XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix());
		}
		else {
			_matrix mtrParents = XMLoadFloat4x4(&m_pParents->Get_CombinedWorldMatrix());
			XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);

			Compute_CamDistance(XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, m_CombinedWorldMatrix._44));
		}

		if (nullptr != m_pRendererCom && !m_bDead) {
			switch (m_ParticleInfo.iShader)
			{
			case CEffect::SHADER_DISTORTION:
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DISTORTION, this);
				break;
			case CEffect::SHADER_GRAYSCALE:
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GRAYSCALE, this);
				break;
			case CEffect::SHADER_ALPHATEST:
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
				break;
			case CEffect::SHADER_ALPHABLEND:
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
				break;
			}
		}
	}
}

HRESULT CEffect_Particle::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (m_ParticleInfo.iShader == CEffect::SHADER_DISTORTION) {
		m_pShaderCom->Begin(3);
		m_pVIBufferCom->Render();
		return S_OK;
	}
	if (m_ParticleInfo.iShader == CEffect::SHADER_GRAYSCALE) {
		m_pShaderCom->Begin(4);
		m_pVIBufferCom->Render();
		return S_OK;
	}

	if (m_ParticleInfo.iShader == CEffect::SHADER_ALPHABLEND) {
		m_pShaderCom->Begin(1);
	}
	else if (m_ParticleInfo.iShader == CEffect::SHADER_ALPHATEST) {
		m_pShaderCom->Begin(2);
	}
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Particle::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxEffParticleInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Particle::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_CombinedWorldMatrix)));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_ParticleInfo.iTextureType))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &m_ParticleInfo.fDisappearTimeRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_ParticleInfo.vColor, sizeof(_float4))))
		return E_FAIL;

	//	Test
	m_pShaderCom->Set_RawValue("g_bUseRGB", &m_ParticleInfo.bUseRGB, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bUseColor", &m_ParticleInfo.bUseColor, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bMask", &m_ParticleInfo.bUseMask, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bGlow", &m_ParticleInfo.bGlow, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bBillboard", &m_ParticleInfo.bBillboard, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bDisappearStart", &m_ParticleInfo.bDisappearStart, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bDissolve", &m_ParticleInfo.iDisappear, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bUseGlowColor", &m_ParticleInfo.bUseGlowColor, sizeof(_bool));

	m_pShaderCom->Set_RawValue("g_vGlowColor", &m_ParticleInfo.vGlowColor, sizeof(_float3));
}

void CEffect_Particle::Set_ParticleInfo(PARTICLE_INFO ParticleInfo)
{
	m_ParticleInfo = ParticleInfo;

	/* For.Com_VIBuffer */
	char szName[MAX_PATH] = "Prototype_Component_VIBuffer_Particle_Instance_";
	char szNum[MAX_PATH] = "";

	_itoa_s(m_ParticleInfo.iParticleNum, szNum, 10);

	strcat_s(szName, szNum);

	_tchar			szRealPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, szName, (int)strlen(szName), szRealPath, MAX_PATH);

	_float4 vSize = _float4(0.01f, 0.01f, 0.01f, 1.f);
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, szRealPath, (CComponent**)&m_pVIBufferCom, &vSize)))
		return;

	if (m_ParticleInfo.bRoof) {
		m_pVIBufferCom->Reset(m_ParticleInfo.fRoofTime, m_ParticleInfo.fSpeed, m_ParticleInfo.vSize, m_pTransformCom->Get_World4x4(),
			m_ParticleInfo.iParticleType, m_ParticleInfo.iConeSizeX, m_ParticleInfo.iConeSizeY, m_ParticleInfo.fDirectionX, m_ParticleInfo.fDirectionY, m_ParticleInfo.fStartTurm);
	}
	else {
		m_pVIBufferCom->Reset(m_ParticleInfo.fLifeTime, m_ParticleInfo.fSpeed, m_ParticleInfo.vSize, m_pTransformCom->Get_World4x4(),
			m_ParticleInfo.iParticleType, m_ParticleInfo.iConeSizeX, m_ParticleInfo.iConeSizeY, m_ParticleInfo.fDirectionX, m_ParticleInfo.fDirectionY, m_ParticleInfo.fStartTurm);
	}

	m_fTime = 0.f;

	_matrix mtrParents = XMLoadFloat4x4(&m_pParents->Get_CombinedWorldMatrix());
	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_ParticleInfo.vPosition.x, m_ParticleInfo.vPosition.y, m_ParticleInfo.vPosition.z, 1.f));

	_float3		vRotation = m_ParticleInfo.vRotation;
	vRotation.x = XMConvertToRadians(vRotation.x);
	vRotation.y = XMConvertToRadians(vRotation.y);
	vRotation.z = XMConvertToRadians(vRotation.z);
	m_pTransformCom->RotationAll(vRotation);
}

CEffect_Particle * CEffect_Particle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_Particle*	pInstance = new CEffect_Particle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect_Texture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_Particle::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect_Particle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect_Texture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Particle::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
