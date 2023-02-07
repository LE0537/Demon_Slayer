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

	return S_OK;
}

HRESULT CEffect_Texture::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Texture::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > m_TextureInfo.fStartTime && m_fTime < m_TextureInfo.fLifeTime + m_TextureInfo.fStartTime) {
		// Texture Size 보간
		_float	fTimefromStart = m_fTime - m_TextureInfo.fStartTime;
		_float fLife = m_TextureInfo.fLifeTime / 3.f;
		_vector vSize = XMVectorSet(0.f, 0.f, 0.f, 0.f);

		if (fLife > m_fTime - m_TextureInfo.fStartTime) {
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
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

		vPos += XMVector3Normalize(XMLoadFloat3(&m_TextureInfo.vMoveDirection)) * fTimeDelta * m_TextureInfo.fMoveSpeed;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	}
}

void CEffect_Texture::Late_Tick(_float fTimeDelta)
{
	if (static_cast<CEffect*>(m_pParents)->Get_EffectMove() == CEffect::EFFMOVE_STOP) {
		if (m_fTime <= m_TextureInfo.fStartTime) {
			_matrix mtrParents = m_pParents->Get_Transform()->Get_WorldMatrix();
			XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);
			XMStoreFloat4x4(&m_ParentsMtr, mtrParents);
		}
		else {
			XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(&m_ParentsMtr));
		}
	}
	else {
		_matrix mtrParents = m_pParents->Get_Transform()->Get_WorldMatrix();
		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);
	}

	Compute_CamDistance(XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, m_CombinedWorldMatrix._44));

	if (m_fTime > m_TextureInfo.fStartTime && m_fTime < m_TextureInfo.fLifeTime + m_TextureInfo.fStartTime) {

		if (m_TextureInfo.bBillboard) {
			CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

			_float4 vCampos = pGameInstance->Get_CamPosition();

			RELEASE_INSTANCE(CGameInstance);

			_matrix mtrCombinedWorld = XMLoadFloat4x4(&m_CombinedWorldMatrix);

			_float3 vScale = m_pTransformCom->Get_Scale();

			_vector vLook = XMVector3Normalize(mtrCombinedWorld.r[3] - XMLoadFloat4(&vCampos));
			_vector vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

			_vector vRight = XMVector3Normalize(XMVector3Cross(vAxisY, vLook));
			_vector	vUp = XMVector3Cross(vLook, vRight);

			_matrix RotationMatrix = XMMatrixRotationAxis(vLook, XMConvertToRadians(m_TextureInfo.fRotation.z));

			mtrCombinedWorld.r[0] = XMVector3Normalize(vRight) * vScale.x;
			mtrCombinedWorld.r[1] = XMVector3Normalize(vUp) * vScale.y;
			mtrCombinedWorld.r[2] = XMVector3Normalize(vLook) * vScale.z;

			mtrCombinedWorld.r[0] = XMVector3TransformNormal(mtrCombinedWorld.r[0], RotationMatrix);
			mtrCombinedWorld.r[1] = XMVector3TransformNormal(mtrCombinedWorld.r[1], RotationMatrix);
			mtrCombinedWorld.r[2] = XMVector3TransformNormal(mtrCombinedWorld.r[2], RotationMatrix);

			XMStoreFloat4x4(&m_CombinedWorldMatrix, mtrCombinedWorld);
		}

		if (nullptr != m_pRendererCom)
		{
			switch (m_TextureInfo.iShader)
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
			case CEffect::SHADER_PRIORITY_TEST:
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
				break;
			}
		}
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
	else if (m_TextureInfo.iShader == CEffect::SHADER_PRIORITY_TEST) {
		m_pShaderCom->Begin(8);
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

	if (strcmp(m_TextureInfo.szTextureMask, "")) {
		char szMaskName[MAX_PATH] = "Prototype_Component_Texture_";
		strcat_s(szMaskName, m_TextureInfo.szTextureMask);

		_tchar			szMaskRealPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szMaskName, (_int)strlen(szMaskName), szMaskRealPath, MAX_PATH);

		if (FAILED(__super::Add_Components(TEXT("Com_MaskTexture"), LEVEL_STATIC, szMaskRealPath, (CComponent**)&m_pMaskTextureCom)))
			return;
	}

	if (strcmp(m_TextureInfo.szTextureDissolve, "")) {
		char szDissolveName[MAX_PATH] = "Prototype_Component_Texture_";
		strcat_s(szDissolveName, m_TextureInfo.szTextureDissolve);

		_tchar			szDissolveRealPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szDissolveName, (_int)strlen(szDissolveName), szDissolveRealPath, MAX_PATH);

		if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, szDissolveRealPath, (CComponent**)&m_pDissolveTextureCom)))
			return;
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_TextureInfo.vPosition.x, m_TextureInfo.vPosition.y, m_TextureInfo.vPosition.z, 1.f)
		+ m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.001f * (_float)m_TextureInfo.iSortingFudge);

	_float3		vRotation = m_TextureInfo.fRotation;
	vRotation.x = XMConvertToRadians(vRotation.x);
	vRotation.y = XMConvertToRadians(vRotation.y);
	vRotation.z = XMConvertToRadians(vRotation.z);
	m_pTransformCom->RotationAll(vRotation);

	XMStoreFloat4x4(&m_ParentsMtr, m_pParents->Get_Transform()->Get_WorldMatrix());
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

	//m_TextureInfo.fDisappearTimeRatio
	Time = 1 - (m_fTime - m_TextureInfo.fStartTime) / m_TextureInfo.fLifeTime;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fEndAlpha", &Time, sizeof(_float))))
		return E_FAIL;

	_float AlphaRatio;
	if (m_TextureInfo.fDisappearTimeRatio >= 1 - Time) {
		_bool bStart = false;
		m_pShaderCom->Set_RawValue("g_bDisappearStart", &bStart, sizeof(_bool));

		AlphaRatio = 0;
	}
	else {
		//0~1
		_bool bStart = true;
		m_pShaderCom->Set_RawValue("g_bDisappearStart", &bStart, sizeof(_bool));

		_float fFullTime = m_TextureInfo.fLifeTime * m_TextureInfo.fDisappearTimeRatio;
		_float fCurTime = m_fTime - m_TextureInfo.fStartTime;

		fCurTime -= fFullTime;
		fFullTime = m_TextureInfo.fLifeTime * (1 - m_TextureInfo.fDisappearTimeRatio);

		AlphaRatio = fCurTime / fFullTime;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &AlphaRatio, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_bDissolve", &m_TextureInfo.bDisappearAlpha, sizeof(_bool));

	if (m_pDissolveTextureCom != nullptr) {
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DissolveTexture", m_pDissolveTextureCom->Get_SRV(0))))
			return E_FAIL;
	}

	if (m_pMaskTextureCom != nullptr) {
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_MaskTexture", m_pMaskTextureCom->Get_SRV(0))))
			return E_FAIL;
	}

	m_pShaderCom->Set_RawValue("g_bUseRGB", &m_TextureInfo.bUseRGB, sizeof(_bool));//	Color * (RGB or A)
	m_pShaderCom->Set_RawValue("g_bUseColor", &m_TextureInfo.bUseColor, sizeof(_bool));//	Color = g_vColor or DiffuseTexture
	m_pShaderCom->Set_RawValue("g_bGlow", &m_TextureInfo.bGlow, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bUseMask", &m_TextureInfo.bUseMask, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_fPostProcesesingValue", &m_TextureInfo.fPostProcessingValue, sizeof(_float));

	m_pShaderCom->Set_RawValue("g_bUseGlowColor", &m_TextureInfo.bUseGlowColor, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_vGlowColor", &m_TextureInfo.vGlowColor, sizeof(_float3));

	if (FAILED(m_pShaderCom->Set_RawValue("g_iNumUV_U", &m_TextureInfo.iNumUV_U, sizeof(_int))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iNumUV_V", &m_TextureInfo.iNumUV_V, sizeof(_int))))
		return E_FAIL;

	_float		fAccTime = m_fTime - m_TextureInfo.fStartTime;
	_float		fAllLifeTime = m_TextureInfo.fLifeTime;
	_float		fAliveTimeRatio = max(fAccTime / fAllLifeTime, 0.f);
	_int		iTexFrame = fAliveTimeRatio * (m_TextureInfo.iNumUV_U * m_TextureInfo.iNumUV_V);
	if (m_TextureInfo.iNumUV_U * m_TextureInfo.iNumUV_V == iTexFrame)
		iTexFrame = m_TextureInfo.iNumUV_U * m_TextureInfo.iNumUV_V - 1;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iFrame", &iTexFrame, sizeof(_int))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionU", &m_TextureInfo.fDistortion_U, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionV", &m_TextureInfo.fDistortion_V, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_vGlowColor", &m_TextureInfo.vGlowColor, sizeof(_float3));

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDisappearTimeRatio", &m_TextureInfo.fDisappearTimeRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionScale", &m_TextureInfo.fDistortionScale, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionBias", &m_TextureInfo.fDistortionBias, sizeof(_float))))
		return E_FAIL;

	m_fMoveUV_U = fAliveTimeRatio * m_TextureInfo.fMove_Value_U;		//	텍스쳐가 텍스쳐의 x축으로 이동
	m_fMoveUV_V = fAliveTimeRatio * m_TextureInfo.fMove_Value_V;		//	텍스쳐가 텍스쳐의 y축으로 이동
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMoveUV_U", &m_fMoveUV_U, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMoveUV_V", &m_fMoveUV_V, sizeof(_float))))
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
	Safe_Release(m_pDissolveTextureCom);
	Safe_Release(m_pMaskTextureCom);
	
}
