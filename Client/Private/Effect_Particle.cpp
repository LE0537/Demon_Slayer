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

	if (m_fTime > m_ParticleInfo.fStartTime && m_fTime < m_ParticleInfo.fLifeTime + m_ParticleInfo.fStartTime) {
		_float3 fSize;
		_float fLive = m_ParticleInfo.fLifeTime / 4;

		if (m_ParticleInfo.bSizePix)
			fSize = _float3(0.f, 0.f, 0.f);
		else
			fSize = _float3(0.f, 0.005f, 0.f);

		if (m_ParticleInfo.bGravity) {
			if (m_fGravity == 0.f)
				m_fGravity = 2.f;

			m_fGravity -= m_ParticleInfo.fGravitySpeed;

			if (m_fGravity < -3.f && m_ParticleInfo.bRoof) {
				m_fGravity = 0.f;
			}
		}
		else
			m_fGravity = 0.f;

		m_pVIBufferCom->Update(fTimeDelta, fSize, m_CombinedWorldMatrix, m_ParticleInfo.fSpeedType, m_fGravity, m_ParticleInfo.vSize, m_ParticleInfo.bRoof, m_ParticleInfo.fSpeed,
			m_ParticleInfo.iParticleType, m_ParticleInfo.iConeSizeX, m_ParticleInfo.iConeSizeY);
	}
}

void CEffect_Particle::Late_Tick(_float fTimeDelta)
{
	if (m_fTime > m_ParticleInfo.fStartTime && m_fTime < m_ParticleInfo.fLifeTime + m_ParticleInfo.fStartTime) {
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_ParticleInfo.vPosition.x, m_ParticleInfo.vPosition.y, m_ParticleInfo.vPosition.z, 1.f)
			+ m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.001f);

		_matrix mtrParents = m_pParents->Get_Transform()->Get_WorldMatrix();
		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);

		Compute_CamDistance(XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, m_CombinedWorldMatrix._44));

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
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

	_float Time = 1.f;

	if (m_ParticleInfo.iDisappear == CEffect::DISAPPEAR_ALPHA) {
		Time = 1 - m_fTime / m_ParticleInfo.fLifeTime + m_ParticleInfo.fStartTime;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_fEndAlpha", &Time, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_ParticleInfo.vColor, sizeof(_float4))))
		return E_FAIL;

	//	Test
	m_pShaderCom->Set_RawValue("g_bUseRGB", &m_ParticleInfo.bUseRGB, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bUseColor", &m_ParticleInfo.bUseColor, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bMask", &m_ParticleInfo.bUseMask, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bGlow", &m_ParticleInfo.bGlow, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bBillboard", &m_ParticleInfo.bBillboard, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bYBillboard", &m_ParticleInfo.bYBillboard, sizeof(_bool));

	m_pShaderCom->Set_RawValue("g_fPostProcesesingValue", &m_ParticleInfo.fPostProcessingValue, sizeof(_float));

	/*ID3D11ShaderResourceView*		pSRVs[] = {
	m_pNoiseTextureCom->Get_SRV()
	};

	if (FAILED(m_pShaderCom->Set_ShaderResourceViewArray("g_NoiseTexture", pSRVs, 1)))
	return E_FAIL;*/
	//	Test End

	return S_OK;
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

	m_pVIBufferCom->Reset(m_ParticleInfo.vSize, m_ParticleInfo.fLifeTime, m_ParticleInfo.fSpeed, m_ParticleInfo.iParticleType, m_CombinedWorldMatrix, m_ParticleInfo.iConeSizeX,
		m_ParticleInfo.iConeSizeY, m_ParticleInfo.bRoof);

	m_fTime = 0.f;
	m_fGravity = 0.f;
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
