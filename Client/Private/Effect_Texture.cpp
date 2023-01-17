#include "stdafx.h"
#include "Effect_Texture.h"
#include "GameInstance.h"


CEffect_Texture::CEffect_Texture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect_Texture::CEffect_Texture(const CEffect_Texture & rhs)
	: CGameObj(rhs)
{
}

HRESULT CEffect_Texture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Texture::Initialize(void * pArg)
{
	if (pArg)
		strcpy_s(m_TextureInfo.m_szTextureName, MAX_PATH, (char*)pArg);

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
		// Texture Size ����
		_float fLife = m_TextureInfo.fLifeTime / 3.f;
		_vector vSize;

		if (fLife > m_fTime) {
			_vector vFirstSize = XMVectorSet(m_TextureInfo.vSize[0].x, m_TextureInfo.vSize[0].y, 1.f, 0.f);
			_vector vSecondSize = XMVectorSet(m_TextureInfo.vSize[1].x, m_TextureInfo.vSize[1].y, 1.f, 0.f);

			_float fTime = (m_fTime - m_TextureInfo.fStartTime) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		else if (fLife <= m_fTime && fLife * 2 > m_fTime) {
			_vector vFirstSize = XMVectorSet(m_TextureInfo.vSize[1].x, m_TextureInfo.vSize[1].y, 1.f, 0.f);
			_vector vSecondSize = XMVectorSet(m_TextureInfo.vSize[2].x, m_TextureInfo.vSize[2].y, 1.f, 0.f);

			_float fTime = (m_fTime - m_TextureInfo.fStartTime - fLife) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		else if (fLife * 2 <= m_fTime && fLife * 3 > m_fTime) {
			_vector vFirstSize = XMVectorSet(m_TextureInfo.vSize[2].x, m_TextureInfo.vSize[2].y, 1.f, 0.f);
			_vector vSecondSize = XMVectorSet(m_TextureInfo.vSize[3].x, m_TextureInfo.vSize[3].y, 1.f, 0.f);

			_float fTime = (m_fTime - m_TextureInfo.fStartTime - fLife * 2) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		m_pTransformCom->Set_Scale(vSize);
	}
	else if (m_fTime > m_TextureInfo.fLifeTime + m_TextureInfo.fStartTime) {
		m_bDead = true;
	}
}

void CEffect_Texture::Late_Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_TextureInfo.vPosition.x, m_TextureInfo.vPosition.y, m_TextureInfo.vPosition.z, 1.f)
		+ m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.001f * m_TextureInfo.fSortingFudge);

	_matrix mtrParents = m_pParents->Get_Transform()->Get_WorldMatrix();

	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);

	Compute_CamDistance(XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, m_CombinedWorldMatrix._44));

	if (nullptr != m_pRendererCom && !m_bDead)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CEffect_Texture::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferCom->Render();

	return S_OK;
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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	char szName[MAX_PATH] = "Prototype_Component_Texture_";
	strcat_s(szName, m_TextureInfo.m_szTextureName);

	_tchar			szRealPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, szName, strlen(szName), szRealPath, MAX_PATH);

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, szRealPath, (CComponent**)&m_pTextureCom)))
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
}
