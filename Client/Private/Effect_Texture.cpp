#include "stdafx.h"
#include "Effect_Texture.h"
#include "GameInstance.h"
#include "Effect.h"


CEffect_Texture::CEffect_Texture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect_Texture::CEffect_Texture(const CEffect_Texture & rhs)
	: CGameObj(rhs),
	m_TextureName(rhs.m_TextureName)
{
}

HRESULT CEffect_Texture::Initialize_Prototype()
{
	/*m_TextureName.push_back("Eff_Circle");
	m_TextureName.push_back("Eff_Sprk");
	m_TextureName.push_back("Xef_Base");
	m_TextureName.push_back("Eff_Tap");
	m_TextureName.push_back("Xef_Burst");
	m_TextureName.push_back("Xef_Dust");
	m_TextureName.push_back("Xef_Fade00");
	m_TextureName.push_back("Xef_Fade01");
	m_TextureName.push_back("Xef_Fade02");
	m_TextureName.push_back("Xef_Fade03");
	m_TextureName.push_back("Xef_Light00");
	m_TextureName.push_back("Eff_Light00");
	m_TextureName.push_back("Xef_Light01");
	m_TextureName.push_back("Xef_Line01");
	m_TextureName.push_back("Xef_Squ01");
	m_TextureName.push_back("Xef_Sun00");
	m_TextureName.push_back("Xef_Sun01");
	m_TextureName.push_back("Xef_Sun02");
	m_TextureName.push_back("Xef_Uzu00");
	m_TextureName.push_back("Spike00");
	m_TextureName.push_back("Spike01");
	m_TextureName.push_back("Wind02");*/

	return S_OK;
}

HRESULT CEffect_Texture::Initialize(void * pArg)
{
	//if (pArg)
		//strcpy_s(m_TextureInfo.m_szTextureName, MAX_PATH, (char*)pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Texture::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_TextureInfo.bBillboard) {
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		_float4x4		ViewMatrix;
		ViewMatrix = pGameInstance->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);

		RELEASE_INSTANCE(CGameInstance);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4((_float4*)&ViewMatrix.m[0][0]));
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMLoadFloat4((_float4*)&ViewMatrix.m[1][0]));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMLoadFloat4((_float4*)&ViewMatrix.m[2][0]));

		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(m_TextureInfo.fRotation));
	}

	if (m_fTime > m_TextureInfo.fStartTime && m_fTime < m_TextureInfo.fLifeTime + m_TextureInfo.fStartTime) {
		// Texture Size º¸°£
		_float	fTimefromStart = m_fTime - m_TextureInfo.fStartTime;
		_float fLife = m_TextureInfo.fLifeTime / 3.f;
		_vector vSize;

		if (fLife > fTimefromStart) {
			_vector vFirstSize = XMVectorSet(m_TextureInfo.vSize[0].x, m_TextureInfo.vSize[0].y, 1.f, 0.f);
			_vector vSecondSize = XMVectorSet(m_TextureInfo.vSize[1].x, m_TextureInfo.vSize[1].y, 1.f, 0.f);

			_float fTime = (m_fTime - m_TextureInfo.fStartTime) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		else if (fLife <= fTimefromStart && fLife * 2 > fTimefromStart) {
			_vector vFirstSize = XMVectorSet(m_TextureInfo.vSize[1].x, m_TextureInfo.vSize[1].y, 1.f, 0.f);
			_vector vSecondSize = XMVectorSet(m_TextureInfo.vSize[2].x, m_TextureInfo.vSize[2].y, 1.f, 0.f);

			_float fTime = (m_fTime - m_TextureInfo.fStartTime - fLife) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		else if (fLife * 2 <= fTimefromStart && fLife * 3 > fTimefromStart) {
			_vector vFirstSize = XMVectorSet(m_TextureInfo.vSize[2].x, m_TextureInfo.vSize[2].y, 1.f, 0.f);
			_vector vSecondSize = XMVectorSet(m_TextureInfo.vSize[3].x, m_TextureInfo.vSize[3].y, 1.f, 0.f);

			_float fTime = (m_fTime - m_TextureInfo.fStartTime - fLife * 2) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		m_pTransformCom->Set_Scale(vSize);
	}
}

void CEffect_Texture::Late_Tick(_float fTimeDelta)
{
	if (m_fTime > m_TextureInfo.fStartTime && m_fTime < m_TextureInfo.fLifeTime + m_TextureInfo.fStartTime) {
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_TextureInfo.vPosition.x, m_TextureInfo.vPosition.y, m_TextureInfo.vPosition.z, 1.f)
			+ m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.001f * (_float)m_TextureInfo.iSortingFudge);

		_matrix mtrParents = m_pParents->Get_Transform()->Get_WorldMatrix();

		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);

		Compute_CamDistance(XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, m_CombinedWorldMatrix._44));

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	}
}

HRESULT CEffect_Texture::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (m_TextureInfo.iShader == CEffect::SHADER_DISTORTION) {
		m_pShaderCom->Begin(3);
		m_pVIBufferCom->Render();
		return S_OK;
	}
	if (m_TextureInfo.iShader == CEffect::SHADER_GRAYSCALE) {
		m_pShaderCom->Begin(4);
		m_pVIBufferCom->Render();
		return S_OK;
	}


	if (m_TextureInfo.iShader == CEffect::SHADER_ALPHABLEND) {
		if (true == m_TextureInfo.bUseFlowMap)
			m_pShaderCom->Begin(6);		//	FlowMap
		else
			m_pShaderCom->Begin(5);
	}
	else if (m_TextureInfo.iShader == CEffect::SHADER_ALPHATEST) {
		if (true == m_TextureInfo.bUseFlowMap)
			m_pShaderCom->Begin(7);		//	FlowMap_AlphaTest
		else
			m_pShaderCom->Begin(2);
	}
	m_pVIBufferCom->Render();

	return S_OK;
}

void CEffect_Texture::Set_TexInfo(TextureInfo TexInfo)
{
	m_TextureInfo = TexInfo;

	/* For.Com_Texture */
	char szName[MAX_PATH] = "Prototype_Component_Texture_";
	strcat_s(szName, m_TextureInfo.szTextureType);

	_tchar			szRealPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, szName, (_int)strlen(szName), szRealPath, MAX_PATH);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, szRealPath, (CComponent**)&m_pTextureCom)))
		return;
}

HRESULT CEffect_Texture::SetUp_ShaderResources()
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

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_TextureInfo.vColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	_float Time = 1.f;

	if (m_TextureInfo.iDisappear == CEffect::DISAPPEAR_ALPHA) {
		Time = 1 - m_fTime / m_TextureInfo.fLifeTime + m_TextureInfo.fStartTime;
	}

	if (m_TextureInfo.iDisappear == CEffect::DISAPPEAR_DISJOLVE) {
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_fEndALPHA", &Time, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_bUseRGB", &m_TextureInfo.bUseRGB, sizeof(_bool));//	Color * (RGB or A)
	m_pShaderCom->Set_RawValue("g_bUseColor", &m_TextureInfo.bUseColor, sizeof(_bool));//	Color = g_vColor or DiffuseTexture
	m_pShaderCom->Set_RawValue("g_bGlow", &m_TextureInfo.bGlow, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_fPostProcesesingValue", &m_TextureInfo.fPostProcessingValue, sizeof(_float));

	m_pShaderCom->Set_RawValue("g_bUseGlowColor", &m_TextureInfo.bUseGlowColor, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_vGlowColor", &m_TextureInfo.vGlowColor, sizeof(_float3));

	_float		fAccTime = m_fTime - m_TextureInfo.fStartTime;
	_float		fAllLifeTime = m_TextureInfo.fLifeTime - m_TextureInfo.fStartTime;
	_float		fAliveTimeRatio = max(fAccTime / fAllLifeTime, 0.f);
	m_pShaderCom->Set_RawValue("g_fAliveTimeRatio", &fAliveTimeRatio, sizeof(_float));

	ID3D11ShaderResourceView*		pSRVs[] = {
		m_pNoiseTextureCom->Get_SRV()
	};

	if (FAILED(m_pShaderCom->Set_ShaderResourceViewArray("g_NoiseTexture", pSRVs, 1)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionScale", &m_TextureInfo.fDistortionScale, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionBias", &m_TextureInfo.fDistortionBias, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Texture::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Shader_VtxEffTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_NoiseTexture */
	if (FAILED(__super::Add_Components(TEXT("Com_NoiseTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Noise"), (CComponent**)&m_pNoiseTextureCom)))
		return E_FAIL;
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

CEffect_Texture * CEffect_Texture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_Texture*	pInstance = new CEffect_Texture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect_Texture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_Texture::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect_Texture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect_Texture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Texture::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pNoiseTextureCom);
}
