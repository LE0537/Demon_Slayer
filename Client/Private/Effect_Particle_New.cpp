#include "stdafx.h"
#include "Effect_Particle_New.h"
#include "GameInstance.h"
#include "Effect.h"

CEffect_Particle_New::CEffect_Particle_New(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect_Particle_New::CEffect_Particle_New(const CEffect_Particle_New & rhs)
	: CGameObj(rhs)
{
}

HRESULT CEffect_Particle_New::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Particle_New::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Particle_New::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > m_ParticleInfo.fStartTime && m_fTime < m_ParticleInfo.fLifeTime[1] + m_ParticleInfo.fStartTime + m_ParticleInfo.fDuration) {
		m_pVIBufferCom->Update(fTimeDelta, m_ParticleInfo.vSizeReduction, m_CombinedWorldMatrix, m_ParticleInfo.fSpeedReduction, m_ParticleInfo.fGravity, m_ParticleInfo.bSpeedkill,
			XMLoadFloat4x4(&m_CombinedWorldMatrix));
	}
}

void CEffect_Particle_New::Late_Tick(_float fTimeDelta)
{
	if (static_cast<CEffect*>(m_pParents)->Get_EffectMove() == CEffect::EFFMOVE_STOP) {
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
	}

	if (m_fTime > m_ParticleInfo.fStartTime && m_fTime < m_ParticleInfo.fLifeTime[1] + m_ParticleInfo.fStartTime + m_ParticleInfo.fDuration) {
		Compute_CamDistance(XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, m_CombinedWorldMatrix._44));
		
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

HRESULT CEffect_Particle_New::Render()
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

HRESULT CEffect_Particle_New::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNewParticleInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_NewParticle_Instance"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Particle_New::SetUp_ShaderResources()
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_vGlowColor", &m_ParticleInfo.vGlowColor, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_iNumUV_U", &m_ParticleInfo.iNumUV_U, sizeof(_int))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iNumUV_V", &m_ParticleInfo.iNumUV_V, sizeof(_int))))
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_bUseRGB", &m_ParticleInfo.bUseRGB, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bUseColor", &m_ParticleInfo.bUseColor, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bGlow", &m_ParticleInfo.bUseGlow, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bBillboard", &m_ParticleInfo.bBillboard, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bYBillboard", &m_ParticleInfo.bYBillboard, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bBillboardTurn", &m_ParticleInfo.bBillboardTurn, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bFollow", &m_ParticleInfo.bFollow, sizeof(_bool));

	return S_OK;
}

void CEffect_Particle_New::Set_ParticleInfo(PARTICLE_INFO ParticleInfo)
{
	m_ParticleInfo = ParticleInfo;

	m_fTime = 0.f;

	_float3		vRotation = m_ParticleInfo.vRotation;
	vRotation.x = XMConvertToRadians(vRotation.x);
	vRotation.y = XMConvertToRadians(vRotation.y);
	vRotation.z = XMConvertToRadians(vRotation.z);
	m_pTransformCom->RotationAll(vRotation);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_ParticleInfo.vPosition.x, m_ParticleInfo.vPosition.y, m_ParticleInfo.vPosition.z, 1.f));

	_matrix mtrParents = XMLoadFloat4x4(&m_pParents->Get_CombinedWorldMatrix());
	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);

	m_pVIBufferCom->Reset(m_ParticleInfo.iMaxParticleNumber, m_ParticleInfo.fLifeTime, m_ParticleInfo.fSpeed, m_ParticleInfo.vParticleSize, m_ParticleInfo.iShape, m_ParticleInfo.fAngle,
		m_ParticleInfo.fRadius, m_ParticleInfo.vSize, m_ParticleInfo.vParticleRotation, m_ParticleInfo.vColor, m_ParticleInfo.fDuration, m_ParticleInfo.fShotTime, m_ParticleInfo.iOneParticleNumber
		, m_ParticleInfo.fCircleY, m_ParticleInfo.fCircleAngle, m_ParticleInfo.bFollow, m_ParticleInfo.bGravityTurn, XMLoadFloat4x4(&m_CombinedWorldMatrix));
}

CEffect_Particle_New * CEffect_Particle_New::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_Particle_New*	pInstance = new CEffect_Particle_New(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect_Particle_New"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_Particle_New::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect_Particle_New(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect_Particle_New"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Particle_New::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
