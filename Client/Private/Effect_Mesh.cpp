#include "stdafx.h"
#include "Effect_Mesh.h"
#include "GameInstance.h"
#include "Effect.h"

CEffect_Mesh::CEffect_Mesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect_Mesh::CEffect_Mesh(const CEffect_Mesh & rhs)
	: CGameObj(rhs),
	m_MeshName(rhs.m_MeshName)
{
}

HRESULT CEffect_Mesh::Initialize_Prototype()
{
	m_MeshName.push_back("EffectSlash1");
	m_MeshName.push_back("EffectSlash2");
	m_MeshName.push_back("EffectSlash3");
	m_MeshName.push_back("EffectSlash4");
	m_MeshName.push_back("EffectSlash5");
	m_MeshName.push_back("EffectSlash6");
	m_MeshName.push_back("EffectSlash7");
	m_MeshName.push_back("EffectSlash8");
	m_MeshName.push_back("Water_6_Ground");
	m_MeshName.push_back("Water_6_Main");
	m_MeshName.push_back("Water_6_Wind");
	m_MeshName.push_back("Water_2_Side");
	m_MeshName.push_back("Water_2_Ring");
	m_MeshName.push_back("BrokenGround1");
	m_MeshName.push_back("BrokenGround2");
	m_MeshName.push_back("UpSwing");
	m_MeshName.push_back("Rengoku_Nobori");
	m_MeshName.push_back("Rengoku_OgiAtk1");
	m_MeshName.push_back("Rengoku_OgiAtk2");
	m_MeshName.push_back("Rengoku_Swing");
	m_MeshName.push_back("Rengoku_Tiger");
	m_MeshName.push_back("Ring1");
	m_MeshName.push_back("Ring4");
	m_MeshName.push_back("Ring5");
	m_MeshName.push_back("Ring6");
	m_MeshName.push_back("Ring14");
	m_MeshName.push_back("Ring15");
	m_MeshName.push_back("Ring16");
	m_MeshName.push_back("Ring17_1");
	m_MeshName.push_back("Ring17_2");
	m_MeshName.push_back("Ring18");
	m_MeshName.push_back("Ring19");
	m_MeshName.push_back("Ring20");
	m_MeshName.push_back("Smoke1");
	m_MeshName.push_back("Smoke2");
	m_MeshName.push_back("Smoke3");
	m_MeshName.push_back("Twist1");
	m_MeshName.push_back("Twist2");
	m_MeshName.push_back("Wind");
	m_MeshName.push_back("Wind1");
	m_MeshName.push_back("Wind2_L");
	m_MeshName.push_back("Wind3_L");
	m_MeshName.push_back("Slash9");
	m_MeshName.push_back("Slash10");
	m_MeshName.push_back("Wind4");
	m_MeshName.push_back("Wind5");
	m_MeshName.push_back("Wind6");
	m_MeshName.push_back("Wind7");
	m_MeshName.push_back("Ring21");
	m_MeshName.push_back("Ring22");
	m_MeshName.push_back("Ring23");
	m_MeshName.push_back("Ring24");
	m_MeshName.push_back("SwordTrail1");
	m_MeshName.push_back("SwordTrail2");
	m_MeshName.push_back("Sphere1");
	m_MeshName.push_back("Sphere2");
	m_MeshName.push_back("Slash11");
	m_MeshName.push_back("Slash12");
	m_MeshName.push_back("Slash13");
	m_MeshName.push_back("Slash14");
	m_MeshName.push_back("Slash15");
	m_MeshName.push_back("Slash16");



	return S_OK;
}

HRESULT CEffect_Mesh::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Mesh::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > m_MeshInfo.fStartTime && m_fTime < m_MeshInfo.fLifeTime + m_MeshInfo.fStartTime) {
		_float fLife = m_MeshInfo.fLifeTime / 3.f;
		_vector vSize = XMVectorSet(1.f, 1.f, 1.f, 0.f);

		if (fLife > m_fTime) {
			_vector vFirstSize = XMVectorSet(m_MeshInfo.vSize[0].x, m_MeshInfo.vSize[0].y, m_MeshInfo.vSize[0].z, 0.f);
			_vector vSecondSize = XMVectorSet(m_MeshInfo.vSize[1].x, m_MeshInfo.vSize[1].y, m_MeshInfo.vSize[1].z, 0.f);

			_float fTime = (m_fTime - m_MeshInfo.fStartTime) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		else if (fLife <= m_fTime && fLife * 2 > m_fTime) {
			_vector vFirstSize = XMVectorSet(m_MeshInfo.vSize[1].x, m_MeshInfo.vSize[1].y, m_MeshInfo.vSize[1].z, 0.f);
			_vector vSecondSize = XMVectorSet(m_MeshInfo.vSize[2].x, m_MeshInfo.vSize[2].y, m_MeshInfo.vSize[2].z, 0.f);

			_float fTime = (m_fTime - m_MeshInfo.fStartTime - fLife) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		else if (fLife * 2 <= m_fTime && fLife * 3 > m_fTime) {
			_vector vFirstSize = XMVectorSet(m_MeshInfo.vSize[2].x, m_MeshInfo.vSize[2].y, m_MeshInfo.vSize[2].z, 0.f);
			_vector vSecondSize = XMVectorSet(m_MeshInfo.vSize[3].x, m_MeshInfo.vSize[3].y, m_MeshInfo.vSize[3].z, 0.f);

			_float fTime = (m_fTime - m_MeshInfo.fStartTime - fLife * 2) / fLife;

			vSize = XMVectorLerp(vFirstSize, vSecondSize, fTime);
		}
		m_pTransformCom->Set_Scale(vSize);
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), m_MeshInfo.fTurn);
	}
}

void CEffect_Mesh::Late_Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_MeshInfo.vPosition.x, m_MeshInfo.vPosition.y, m_MeshInfo.vPosition.z, 1.f));

	_matrix mtrParents = m_pParents->Get_Transform()->Get_WorldMatrix();

	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParents);

	Compute_CamDistance(XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, m_CombinedWorldMatrix._44));

	if (nullptr != m_pRendererCom ) {
		switch (m_MeshInfo.iShader)
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

HRESULT CEffect_Mesh::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (!m_MeshInfo.bUseMeshDiffuse) {
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
		}
		else {
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pDiffuseTextureCom->Get_SRV(0))))
				return E_FAIL;
		}

		if (m_MeshInfo.bMaskTest) {
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_OPACITY)))
				return E_FAIL;
		}

		if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_MeshInfo.vColor[i], sizeof(_float4))))
			return E_FAIL;

		if (m_MeshInfo.iShader == CEffect::SHADER_DISTORTION) {
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 6)))
				return E_FAIL;
		}
		if (m_MeshInfo.iShader == CEffect::SHADER_GRAYSCALE) {
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 7)))
				return E_FAIL;
		}


		if (m_MeshInfo.iShader == CEffect::SHADER_ALPHABLEND) {
			if (true == m_MeshInfo.bUseFlowMap)
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 9)))
					return E_FAIL;
			}		//	FlowMap
			else
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 4)))
					return E_FAIL;
			}
		}
		else if (m_MeshInfo.iShader == CEffect::SHADER_ALPHATEST) {
			if (true == m_MeshInfo.bUseFlowMap)
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 10)))
					return E_FAIL;
			}		//	FlowMap_AlphaTest
			else
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 5)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

void CEffect_Mesh::Set_MeshInfo(MESH_INFO MeshInfo)
{
	m_MeshInfo = MeshInfo;

	/* For.Com_Mesh */
	char szName[MAX_PATH] = "Prototype_Component_Model_";
	strcat_s(szName, m_MeshName[MeshInfo.iMeshType]);

	_tchar			szRealPath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, szName, (_int)strlen(szName), szRealPath, MAX_PATH);

	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szRealPath, (CComponent**)&m_pModelCom)))
		return;

	if (strcmp("", m_MeshInfo.szMeshDissolve) != 0) {
		char szDissolveName[MAX_PATH] = "Prototype_Component_Texture_";
		strcat_s(szDissolveName, m_MeshInfo.szMeshDissolve);
		_tchar			szDissolveRealPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szDissolveName, (_int)strlen(szDissolveName), szDissolveRealPath, MAX_PATH);

		if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, szDissolveRealPath, (CComponent**)&m_pDissolveTextureCom)))
			return;
	}

	if (strcmp("", m_MeshInfo.szMeshType) != 0) {
		char szDiffuseName[MAX_PATH] = "Prototype_Component_Texture_";
		strcat_s(szDiffuseName, m_MeshInfo.szMeshType);

		_tchar			szDiffuseRealPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szDiffuseName, (_int)strlen(szDiffuseName), szDiffuseRealPath, MAX_PATH);

		if (FAILED(__super::Add_Components(TEXT("Com_DiffuseTexture"), LEVEL_STATIC, szDiffuseRealPath, (CComponent**)&m_pDiffuseTextureCom)))
			return;
	}

	_float3		vRotation = m_MeshInfo.vRotation;
	vRotation.x = XMConvertToRadians(vRotation.x);
	vRotation.y = XMConvertToRadians(vRotation.y);
	vRotation.z = XMConvertToRadians(vRotation.z);
	m_pTransformCom->RotationAll(vRotation);
}

HRESULT CEffect_Mesh::SetUp_ShaderResources()
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

	_float Time = 1.f;
	//m_TextureInfo.fDisappearTimeRatio
	Time = 1 - m_fTime / m_MeshInfo.fLifeTime + m_MeshInfo.fStartTime;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fEndALPHA", &Time, sizeof(_float))))
		return E_FAIL;

	_float AlphaRatio;
	if (m_MeshInfo.fDisappearTimeRatio >= 1 - Time) {
		_bool bStart = false;
		m_pShaderCom->Set_RawValue("g_bDisappearStart", &bStart, sizeof(_bool));

		AlphaRatio = 0;
	}
	else {
		_bool bStart = true;
		m_pShaderCom->Set_RawValue("g_bDisappearStart", &bStart, sizeof(_bool));

		_float fFullTime = (m_MeshInfo.fLifeTime + m_MeshInfo.fStartTime);
		_float fCurTime = m_fTime;

		fCurTime -= fFullTime * m_MeshInfo.fDisappearTimeRatio;
		fFullTime -= fFullTime * m_MeshInfo.fDisappearTimeRatio;

		AlphaRatio = fCurTime / fFullTime;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlphaRatio", &AlphaRatio, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_bDissolve", &m_MeshInfo.bDisappearAlpha, sizeof(_bool));

	if (m_pDissolveTextureCom != nullptr) {
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DissolveTexture", m_pDissolveTextureCom->Get_SRV(0))))
			return E_FAIL;
	}

	m_pShaderCom->Set_RawValue("g_bUseRGB", &m_MeshInfo.bUseRGB, sizeof(_bool));//	Color * (RGB or A)
	m_pShaderCom->Set_RawValue("g_bUseColor", &m_MeshInfo.bUseColor, sizeof(_bool));//	Color = g_vColor or DiffuseTexture
	m_pShaderCom->Set_RawValue("g_bGlow", &m_MeshInfo.bGlow, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_UseMask", &m_MeshInfo.bMaskTest, sizeof(_bool));
	m_pShaderCom->Set_RawValue("g_bUseGlowColor", &m_MeshInfo.bUseGlowColor, sizeof(_bool));

	m_pShaderCom->Set_RawValue("g_fPostProcesesingValue", &m_MeshInfo.fPostProcessingValue, sizeof(_float));

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_NoiseTexture", m_pNoiseTextureCom->Get_SRV())))
		return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DissolveTexture", m_pDissolveTextureCom->Get_SRV(0))))
	//return E_FAIL;

	m_pShaderCom->Set_RawValue("g_vGlowColor", &m_MeshInfo.vGlowColor, sizeof(_float3));

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionU", &m_MeshInfo.fDistortion_U, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionV", &m_MeshInfo.fDistortion_V, sizeof(_float))))
		return E_FAIL;

	_float		fAccTime = m_fTime - m_MeshInfo.fStartTime;
	_float		fAllLifeTime = m_MeshInfo.fLifeTime - m_MeshInfo.fStartTime;
	_float		fAliveTimeRatio = max(fAccTime / fAllLifeTime, 0.f);
	m_fMoveUV_U += fAliveTimeRatio * m_MeshInfo.fMove_Value_U;		//	텍스쳐가 텍스쳐의 x축으로 이동
	m_fMoveUV_V += fAliveTimeRatio * m_MeshInfo.fMove_Value_V;		//	텍스쳐가 텍스쳐의 y축으로 이동
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMoveUV_U", &m_fMoveUV_U, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMoveUV_V", &m_fMoveUV_V, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDisappearTimeRatio", &m_MeshInfo.fDisappearTimeRatio, sizeof(_float))))
		return E_FAIL;			//	사라지는 시간의 비율입니다.
								//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseMaskTextureCom", m_pDiffuseMaskTextureCom->Get_SRV())))
								//return E_FAIL;			//	DiffuseTex의 RGBA중 A를 담당합니다.


	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionScale", &m_MeshInfo.fDistortionScale, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionBias", &m_MeshInfo.fDistortionBias, sizeof(_float))))
		return E_FAIL;

	_int		iMulUV_U = 1;
	_int		iMulUV_V = 1;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iMulUV_U", &iMulUV_U, sizeof(_int))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iMulUV_V", &iMulUV_V, sizeof(_int))))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_Mesh::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxEffModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_NoiseTexture */
	if (FAILED(__super::Add_Components(TEXT("Com_NoiseTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Noise"), (CComponent**)&m_pNoiseTextureCom)))
		return E_FAIL;

	return S_OK;
}

CEffect_Mesh * CEffect_Mesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect_Mesh*	pInstance = new CEffect_Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect_Mesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_Mesh::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect_Mesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Mesh::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pNoiseTextureCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pDissolveTextureCom);

}
