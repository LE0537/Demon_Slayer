#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObj(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	NEWTERRAINDESC	tMyDesc;
	ZeroMemory(&tMyDesc, sizeof tMyDesc);
	memcpy(&tMyDesc, pArg, sizeof tMyDesc);
	m_iNumVerticesX = tMyDesc.tTerrainDesc.iSizeX;
	m_iNumVerticesZ = tMyDesc.tTerrainDesc.iSizeZ;

	if (FAILED(Ready_Components(&tMyDesc.tTerrainDesc)))
		return E_FAIL;
	{
		ZeroMemory(&m_tTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		m_tTextureDesc.Width = m_iNumVerticesX;
		m_tTextureDesc.Height = m_iNumVerticesZ;
		m_tTextureDesc.MipLevels = 1;
		m_tTextureDesc.ArraySize = 1;
		m_tTextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

		m_tTextureDesc.SampleDesc.Quality = 0;
		m_tTextureDesc.SampleDesc.Count = 1;

		m_tTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_tTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_tTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_tTextureDesc.MiscFlags = 0;
	}

	m_pPixel = new _uint[m_tTextureDesc.Width * m_tTextureDesc.Height];

	if (0 != strlen(tMyDesc.tTerrainDesc.strFileName))
	{
		if (FAILED(Load_FilterTexture(tMyDesc.tTerrainDesc.strFileName)))
			return E_FAIL;

		if (FAILED(Load_FilterTexNumber(tMyDesc.tTerrainDesc.strFileName)))
			return E_FAIL;
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CTerrain::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	if (!m_bRender)
	{
		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	}
}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pNew_VIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pNew_VIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrain::Ready_Components(void* pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainDiffuse"), (CComponent**)&m_pTerrain_TextureCom[TYPE_DIFFUSE])))
		return E_FAIL;

	/* For.Com_NormalTexture */
	if (FAILED(__super::Add_Components(TEXT("Com_NormalTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainNormal"), (CComponent**)&m_pTerrain_TextureCom[TYPE_NORMAL])))
		return E_FAIL;

	///* For.Com_VTXColor_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_VTXColor_Texture"), LEVEL_STATIC, TEXT("Prototype_Texture_VTXColor"), (CComponent**)&m_pVTXColor_TextureCom)))
	//	return E_FAIL;

	///* For.Com_Brush */
	//if (FAILED(__super::Add_Components(TEXT("Com_Brush"), LEVEL_STATIC, TEXT("Prototype_Texture_Brush"), (CComponent**)&m_pTerrain_TextureCom[TYPE_BRUSH])))
	//	return E_FAIL;

	/* For.Com_Filter */
	if (FAILED(__super::Add_Components(TEXT("Com_Filter"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainDiffuse"), (CComponent**)&m_pTerrain_TextureCom[TYPE_FILTER])))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_NewTerrain"), (CComponent**)&m_pNew_VIBufferCom, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	/* For.Lights */

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (pLightDesc->eType == LIGHTDESC::TYPE_DIRECTIONAL)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
			return E_FAIL;

		m_iPassIndex = 0;
	}
	else
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fLightRange", &pLightDesc->fRange, sizeof(_float))))
			return E_FAIL;

		m_iPassIndex = 1;
	}



	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	/*
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_BrushTexture", m_pTextureCom[TYPE_BRUSH]->Get_SRV())))
	return E_FAIL;*/
	/*
	if (FAILED(m_pShaderCom->Set_RawValue("g_vBrushPos", &m_vPickingPos, sizeof(_float4))))
	return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBrushRange", &m_fPickingRadius, sizeof(_float))))
	return E_FAIL;
	*/
	//if (false == m_bRenderWireFrame)
	{
		ID3D11ShaderResourceView*		pSRVs[] = {
			m_pTerrain_TextureCom[TYPE_DIFFUSE]->Get_SRV(m_iTerrainSRVNum[0]),
			m_pTerrain_TextureCom[TYPE_DIFFUSE]->Get_SRV(m_iTerrainSRVNum[1]),
			m_pTerrain_TextureCom[TYPE_DIFFUSE]->Get_SRV(m_iTerrainSRVNum[2]),
			m_pTerrain_TextureCom[TYPE_DIFFUSE]->Get_SRV(m_iTerrainSRVNum[3]),
		};
		if (FAILED(m_pShaderCom->Set_ShaderResourceViewArray("g_DiffuseTexture", pSRVs, 4)))
			return E_FAIL;

		ID3D11ShaderResourceView*		pNormalSRVs[] = {
			m_pTerrain_TextureCom[TYPE_NORMAL]->Get_SRV(m_iTerrainSRVNum[0]),
			m_pTerrain_TextureCom[TYPE_NORMAL]->Get_SRV(m_iTerrainSRVNum[1]),
			m_pTerrain_TextureCom[TYPE_NORMAL]->Get_SRV(m_iTerrainSRVNum[2]),
			m_pTerrain_TextureCom[TYPE_NORMAL]->Get_SRV(m_iTerrainSRVNum[3]),
		};
		if (FAILED(m_pShaderCom->Set_ShaderResourceViewArray("g_NormalTexture", pNormalSRVs, 4)))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_FilterTexture", m_pFilterTexture)))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTerrain::Load_FilterTexture(char* strName)
{
	char	strFileName[MAX_PATH] = "";
	char	strExtTemp[MAX_PATH] = "";
	_splitpath_s(strName, nullptr, 0, nullptr, 0, strFileName, MAX_PATH, strExtTemp, MAX_PATH);

	char	strFullPath_Multi[MAX_PATH] = "../Bin/Resources/Map/FilterMap/";
	char	strFileExt[MAX_PATH] = ".dds";

	strcat_s(strFullPath_Multi, strFileName);
	strcat_s(strFullPath_Multi, strFileExt);

	_tchar	strFilePath_Wide[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, strFullPath_Multi, (_int)strlen(strFullPath_Multi), strFilePath_Wide, MAX_PATH);

	if (FAILED(DirectX::CreateDDSTextureFromFileEx(m_pDevice, strFilePath_Wide,
		0, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0,
		true, (ID3D11Resource**)&m_pTexture2D, &m_pFilterTexture)))
		return E_FAIL;
	/*
	if (FAILED(DirectX::CreateDDSTextureFromFile(m_pDevice, strFilePath_Wide,
	(ID3D11Resource**)&m_pTexture2D, &m_pFilterTexture)))
	return E_FAIL;


	D3D11_MAPPED_SUBRESOURCE			SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	if (FAILED(m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
	return E_FAIL;

	memcpy(m_pPixel, SubResource.pData, sizeof(_uint) * m_tTextureDesc.Width * m_tTextureDesc.Height);

	m_pContext->Unmap(m_pTexture2D, 0);
	*/


	return S_OK;
}

HRESULT CTerrain::Load_FilterTexNumber(char * strName)
{
	char	strFileName[MAX_PATH] = "";
	char	strExtTemp[MAX_PATH] = "";
	_splitpath_s(strName, nullptr, 0, nullptr, 0, strFileName, MAX_PATH, strExtTemp, MAX_PATH);

	char	strFullPath_Multi[MAX_PATH] = "../Bin/Resources/Map/FilterNum/";
	char	strFileExt[MAX_PATH] = ".ftn";

	strcat_s(strFullPath_Multi, strFileName);
	strcat_s(strFullPath_Multi, strFileExt);

	_tchar	strFilePath_Wide[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, strFullPath_Multi, (_int)strlen(strFullPath_Multi), strFilePath_Wide, MAX_PATH);

	HANDLE		hFile = CreateFile(strFilePath_Wide, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to Load : FilterTex Number");

		return E_FAIL;
	}

	_int		iIndex = 0;

	DWORD			dwByte = 0;
	_int*			pFilterTexNumber = new _int;

	while (true)
	{
		ReadFile(hFile, pFilterTexNumber, sizeof(_int), &dwByte, nullptr);

		if (0 == dwByte ||
			FILTER_END <= iIndex)
		{
			break;
		}

		m_iTerrainSRVNum[iIndex++] = *pFilterTexNumber;
	}
	CloseHandle(hFile);

	Safe_Delete(pFilterTexNumber);

	return S_OK;
}

CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTerrain*	pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	for (_uint i = 0; i < TYPE_END; ++i)
		Safe_Release(m_pTerrain_TextureCom[i]);

	Safe_Release(m_pNew_VIBufferCom);

	//	Filter
	Safe_Delete_Array(m_pPixel);
	Safe_Release(m_pVTXColor_TextureCom);
	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pTexture2D);
}
