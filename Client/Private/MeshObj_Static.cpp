#include "stdafx.h"
#include "..\Public\MeshObj_Static.h"

#include "GameInstance.h"
#include "Data_Manager.h"
#include "Layer.h"
#include "Camera_Dynamic.h"
CMeshObj_Static::CMeshObj_Static(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMeshObj_Static::CMeshObj_Static(const CMeshObj_Static & rhs)
	: CGameObj(rhs)
{
}

void CMeshObj_Static::Change_Model(_uint iObjNumber, const _tchar * pModel_PrototypeTag)
{
	if (FAILED(Delete_Component(TEXT("Com_Model"))))
		return;

	Safe_Release(m_pModelCom);

	__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, pModel_PrototypeTag, (CComponent**)&m_pModelCom);
	if (nullptr == m_pModelCom)
		return;
}

HRESULT CMeshObj_Static::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMeshObj_Static::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_bRenderShadow = true;
	memcpy(&m_tMyDesc, pArg, sizeof m_tMyDesc);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_matrix		matWorld = XMLoadFloat4x4(&m_tMyDesc.matWorld);
	m_pTransformCom->Set_WorldMatrix(matWorld);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if(true == m_bRenderShadow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_STATIC_SHADOWDEPTH, this);


	_float fFovy = XMConvertToRadians(25.0f);
	_float fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	_float fNear = 0.2f;
	_float fFar = g_fFar;

	XMStoreFloat4x4(&m_matProjOrigin, XMMatrixPerspectiveFovLH(fFovy, fAspect, fNear, fFar));

	if (g_iLevel == LEVEL_ADVAKAZA || g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU || g_iLevel == LEVEL_ENDING)
	{
		switch (m_tMyDesc.iTypeNum)
		{
		case 0:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
			break;
		case 1:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, -300.f, 1.f));
			break;
		case 2:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, -600.f, 1.f));
			break;
		default:
			break;
		}
	}

	return S_OK;
}

void CMeshObj_Static::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (g_iLevel == LEVEL_ADVAKAZA || g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU || g_iLevel == LEVEL_ENDING)
	{
		Move_Mesh(fTimeDelta);
	}
}

void CMeshObj_Static::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	if (!m_bRender)
	{
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
		_matrix		matWorld = m_pTransformCom->Get_WorldMatrix();

		if (m_tMyDesc.iModelIndex == 2101 || m_tMyDesc.iModelIndex == 2102
			|| m_tMyDesc.iModelIndex == 2103 || m_tMyDesc.iModelIndex == 2108)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
		}
		else if (nullptr != m_pRendererCom)
		{
			if (false == m_tMyDesc.bAlphaBlend)
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			else if(true == m_bNonLight)
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
			else
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

		}

		RELEASE_INSTANCE(CGameInstance);
	}
}

HRESULT CMeshObj_Static::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{		
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_SHININESS)))
			return E_FAIL;		//	Glow용 ( Max에서 glossiness 에 넣으면 됨.)
		
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (m_tMyDesc.iModelIndex == 2101)
		{
			if (!m_bNonAlpha && i != 8 && i != 9)
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
					return E_FAIL;
			}
			else if (m_bNonAlpha && (i == 8 || i == 9))
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
					return E_FAIL;
			}
		}
		else if (m_tMyDesc.iModelIndex == 2102)
		{
			if (!m_bNonAlpha && i != 8 && i != 9)
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
					return E_FAIL;
			}
			else if (m_bNonAlpha && (i == 8 || i == 9))
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
					return E_FAIL;
			}
		}
		else if (m_tMyDesc.iModelIndex == 2103)
		{
			if (!m_bNonAlpha && i != 2 && i != 3)
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
					return E_FAIL;
			}
			else if (m_bNonAlpha && (i == 2 || i == 3))
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
					return E_FAIL;
			}
		}
		else if (m_tMyDesc.iModelIndex == 2108)
		{
			if (!m_bNonAlpha && i == 0)
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
					return E_FAIL;
			}
			else if (m_bNonAlpha && i == 1)
			{
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
					return E_FAIL;
			}
		}
		else if (false == m_tMyDesc.bAlphaBlend)
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}		
		else
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))	//	Alphablending
				return E_FAIL;
		}
	}

	m_bNonAlpha = !m_bNonAlpha;

	return S_OK;
}

HRESULT CMeshObj_Static::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	const LIGHTDESC* pLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW);
	if (nullptr != pLightDesc)
	{
		_vector			vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDirection);
		_vector			vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDiffuse);
		_vector			vLightUp = { 0.f, 1.f, 0.f ,0.f };
		_matrix			matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
			return E_FAIL;
	}

	if (LEVEL_ADVAKAZA == m_tMyDesc.iCurrentLevel)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProjOrigin)), sizeof(_float4x4))))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

HRESULT CMeshObj_Static::Delete_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_Components.erase(iter);

	return S_OK;
}

HRESULT CMeshObj_Static::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORMDESC));
	tTransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tTransformDesc.fSpeedPerSec = 200.f;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Ready_ModelComponent()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshObj_Static::SetUp_ShaderResources()
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

	RELEASE_INSTANCE(CGameInstance);


	if (FAILED(m_pShaderCom->Set_RawValue("g_fGlowPower", &m_tMyDesc.fGlowPower, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

HRESULT CMeshObj_Static::Ready_ModelComponent()
{
	_tchar	pPrototypeTag_Model[MAX_PATH] = L"";
	switch (m_tMyDesc.iModelIndex)
	{
	case 2001: lstrcpy(pPrototypeTag_Model, L"BigTree1"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2002: lstrcpy(pPrototypeTag_Model, L"BigTree2"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2003: lstrcpy(pPrototypeTag_Model, L"BigTree3"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2004: lstrcpy(pPrototypeTag_Model, L"TreeFar1"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2005: lstrcpy(pPrototypeTag_Model, L"TreeWillow"); m_fFrustumRadiusRatio = 20.f; break;

	case 2006: lstrcpy(pPrototypeTag_Model, L"TreeBroken1"); m_fFrustumRadiusRatio = 6.f; break;
	case 2007: lstrcpy(pPrototypeTag_Model, L"TreeBroken2"); m_fFrustumRadiusRatio = 6.f; break;
	case 2008: lstrcpy(pPrototypeTag_Model, L"TreeBroken3"); m_fFrustumRadiusRatio = 6.f; break;
	case 2009: lstrcpy(pPrototypeTag_Model, L"TreeBroken4"); m_fFrustumRadiusRatio = 6.f; break;
	case 2010: lstrcpy(pPrototypeTag_Model, L"TreeBroken5"); m_fFrustumRadiusRatio = 6.f; break;
	case 2011: lstrcpy(pPrototypeTag_Model, L"TreeBroken6"); m_fFrustumRadiusRatio = 6.f; break;
	case 2012: lstrcpy(pPrototypeTag_Model, L"TreeBroken7"); m_fFrustumRadiusRatio = 6.f; break;
	case 2013: lstrcpy(pPrototypeTag_Model, L"TreeBroken8"); m_fFrustumRadiusRatio = 6.f; break;
	case 2014: lstrcpy(pPrototypeTag_Model, L"TreeBroken9"); m_fFrustumRadiusRatio = 6.f; break;
	case 2015: lstrcpy(pPrototypeTag_Model, L"TreeBroken10"); m_fFrustumRadiusRatio = 6.f; break;

	case 2016: lstrcpy(pPrototypeTag_Model, L"TreeRoot1"); m_fFrustumRadiusRatio = 10.f; break;
	case 2017: lstrcpy(pPrototypeTag_Model, L"TreeRoot2"); m_fFrustumRadiusRatio = 10.f; break;
	case 2018: lstrcpy(pPrototypeTag_Model, L"TreeRoot3"); m_fFrustumRadiusRatio = 10.f; break;

	case 2019: lstrcpy(pPrototypeTag_Model, L"Rock1"); m_fFrustumRadiusRatio = 50.f; break;
	case 2020: lstrcpy(pPrototypeTag_Model, L"Rock2"); m_fFrustumRadiusRatio = 50.f; break;
	case 2021: lstrcpy(pPrototypeTag_Model, L"Rock3"); m_fFrustumRadiusRatio = 50.f; break;
	case 2022: lstrcpy(pPrototypeTag_Model, L"Rock4"); m_fFrustumRadiusRatio = 50.f; break;
	case 2023: lstrcpy(pPrototypeTag_Model, L"Rock5"); m_fFrustumRadiusRatio = 50.f; break;
	case 2024: lstrcpy(pPrototypeTag_Model, L"Rock6"); m_fFrustumRadiusRatio = 50.f; break;
	case 2025: lstrcpy(pPrototypeTag_Model, L"Rock7"); m_fFrustumRadiusRatio = 50.f; break;
	case 2026: lstrcpy(pPrototypeTag_Model, L"RockSmall"); m_fFrustumRadiusRatio = 10.f; break;

	case 2027: lstrcpy(pPrototypeTag_Model, L"Cliff1"); m_fFrustumRadiusRatio = 12.f; break;
	case 2028: lstrcpy(pPrototypeTag_Model, L"Cliff2"); m_fFrustumRadiusRatio = 12.f; break;
	case 2029: lstrcpy(pPrototypeTag_Model, L"Cliff3"); m_fFrustumRadiusRatio = 12.f; break;
	case 2030: lstrcpy(pPrototypeTag_Model, L"Cliff_Small"); m_fFrustumRadiusRatio = 4.f; break;

	case 2031: lstrcpy(pPrototypeTag_Model, L"Grass1");m_bRenderShadow = false; break;
	case 2032: lstrcpy(pPrototypeTag_Model, L"Grass2");m_bRenderShadow = false; break;
	case 2033: lstrcpy(pPrototypeTag_Model, L"Grass3");m_bRenderShadow = false; break;
	case 2034: lstrcpy(pPrototypeTag_Model, L"Grass4");m_bRenderShadow = false; break;
	case 2035: lstrcpy(pPrototypeTag_Model, L"Grass5");m_bRenderShadow = false; break;
	case 2036: lstrcpy(pPrototypeTag_Model, L"Grass6");m_bRenderShadow = false; break;

	case 2037: lstrcpy(pPrototypeTag_Model, L"Lavender");m_bRenderShadow = false; m_bNonLight = true; break;
	case 2038: lstrcpy(pPrototypeTag_Model, L"Flower1"); m_bRenderShadow = false; break;
	case 2039: lstrcpy(pPrototypeTag_Model, L"Flower2"); m_bRenderShadow = false; break;
	case 2040: lstrcpy(pPrototypeTag_Model, L"Flower3"); m_bRenderShadow = false; break;

	case 2041: lstrcpy(pPrototypeTag_Model, L"Leaf1"); m_bRenderShadow = false; break;
	case 2042: lstrcpy(pPrototypeTag_Model, L"Leaf2"); m_bRenderShadow = false; break;
	case 2043: lstrcpy(pPrototypeTag_Model, L"Leaf3"); m_bRenderShadow = false; break;
	case 2044: lstrcpy(pPrototypeTag_Model, L"Leaf4"); m_bRenderShadow = false; break;

	case 2045: lstrcpy(pPrototypeTag_Model, L"Hill_Far1"); m_fFrustumRadiusRatio = 20000.f; m_bRenderShadow = false; break;
	case 2046: lstrcpy(pPrototypeTag_Model, L"Hill_Far2"); m_fFrustumRadiusRatio = 20000.f; m_bRenderShadow = false; break;
	case 2047: lstrcpy(pPrototypeTag_Model, L"Hill_Far3"); m_fFrustumRadiusRatio = 20000.f; m_bRenderShadow = false; break;
	case 2048: lstrcpy(pPrototypeTag_Model, L"Hill_Far4"); m_fFrustumRadiusRatio = 20000.f; m_bRenderShadow = false; break;
	case 2049: lstrcpy(pPrototypeTag_Model, L"Hill_Far5"); m_fFrustumRadiusRatio = 20000.f; m_bRenderShadow = false; break;
	case 2050: lstrcpy(pPrototypeTag_Model, L"Hill_Far6"); m_fFrustumRadiusRatio = 20000.f; m_bRenderShadow = false; break;

	case 2051: lstrcpy(pPrototypeTag_Model, L"Wall1"); m_fFrustumRadiusRatio = 30.f; break;
	case 2052: lstrcpy(pPrototypeTag_Model, L"Wall2"); m_fFrustumRadiusRatio = 30.f; break;

	case 2053: lstrcpy(pPrototypeTag_Model, L"SpiderWeb1"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;
	case 2054: lstrcpy(pPrototypeTag_Model, L"SpiderWeb2"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;
	case 2055: lstrcpy(pPrototypeTag_Model, L"SpiderWeb3"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;

	case 2056: lstrcpy(pPrototypeTag_Model, L"Bush1"); m_bRenderShadow = false; break;
	case 2057: lstrcpy(pPrototypeTag_Model, L"Bush2"); m_bRenderShadow = false; break;
	case 2058: lstrcpy(pPrototypeTag_Model, L"Bush3"); m_bRenderShadow = false; break;
	case 2059: lstrcpy(pPrototypeTag_Model, L"Bush4"); m_bRenderShadow = false; break;
	case 2060: lstrcpy(pPrototypeTag_Model, L"Bush5"); m_bRenderShadow = false; break;
	case 2061: lstrcpy(pPrototypeTag_Model, L"Bush6"); m_bRenderShadow = false; break;
	case 2062: lstrcpy(pPrototypeTag_Model, L"Bush7"); m_bRenderShadow = false; break;
	case 2063: lstrcpy(pPrototypeTag_Model, L"Bush8"); m_bRenderShadow = false; break;
	case 2064: lstrcpy(pPrototypeTag_Model, L"Bush9"); m_bRenderShadow = false; break;

	case 2065: lstrcpy(pPrototypeTag_Model, L"HomeSmall1"); break;
	case 2066: lstrcpy(pPrototypeTag_Model, L"HomeSmall2"); break;

	case 2067: lstrcpy(pPrototypeTag_Model, L"RiceField1"); m_fFrustumRadiusRatio = 120.f; break;

	case 2068: lstrcpy(pPrototypeTag_Model, L"RuiGround"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2069: lstrcpy(pPrototypeTag_Model, L"UrokodakiGround"); m_fFrustumRadiusRatio = 2000.f; break;

	case 2070: lstrcpy(pPrototypeTag_Model, L"RuiGround2"); m_fFrustumRadiusRatio = 2000.f; break;
	case 2071: lstrcpy(pPrototypeTag_Model, L"Home1"); break;
	case 2072: lstrcpy(pPrototypeTag_Model, L"Rubble1"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2073: lstrcpy(pPrototypeTag_Model, L"Rubble2"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2074: lstrcpy(pPrototypeTag_Model, L"SpiderWeb4"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;
	case 2075: lstrcpy(pPrototypeTag_Model, L"SpiderWeb5"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;
	case 2076: lstrcpy(pPrototypeTag_Model, L"SpiderWeb6"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;
	case 2077: lstrcpy(pPrototypeTag_Model, L"SpiderWeb7"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;
	case 2078: lstrcpy(pPrototypeTag_Model, L"SpiderWeb8"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;
	case 2079: lstrcpy(pPrototypeTag_Model, L"SpiderWeb9"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; m_bNonLight = true; break;
	case 2080: lstrcpy(pPrototypeTag_Model, L"Tree_Jenitsu"); m_fFrustumRadiusRatio = 70.f; break;
	case 2081: lstrcpy(pPrototypeTag_Model, L"TreeFar2"); m_fFrustumRadiusRatio = 5.f; m_bRenderShadow = false;break;
	case 2082: lstrcpy(pPrototypeTag_Model, L"TreeFar3"); m_fFrustumRadiusRatio = 5.f; m_bRenderShadow = false;break;

	case 2083: lstrcpy(pPrototypeTag_Model, L"Moon"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2084: lstrcpy(pPrototypeTag_Model, L"MoonLight"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2085: lstrcpy(pPrototypeTag_Model, L"Hut"); m_fFrustumRadiusRatio = 20.f; break;
	case 2086:lstrcpy(pPrototypeTag_Model, L"RuiMap"); m_fFrustumRadiusRatio = 5000.f; m_bRenderShadow = false; break;
	case 2090: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Rail1"); m_fFrustumRadiusRatio = 30.f; break;
	case 2091:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Rail2"); m_fFrustumRadiusRatio = 30.f; break;
	case 2092: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BrokenTrain1"); m_fFrustumRadiusRatio = 30.f; break;
	case 2093:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BrokenTrain2"); m_fFrustumRadiusRatio = 30.f;  break;
	case 2094: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BrokenTrain3"); m_fFrustumRadiusRatio = 30.f; break;
	case 2095:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BrokenTrain4"); m_fFrustumRadiusRatio = 30.f;  break;
	case 2096: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_BrokenTrain5"); m_fFrustumRadiusRatio = 30.f; break;
	case 2097:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_TrainMap"); m_fFrustumRadiusRatio = 5000.f; m_bRenderShadow = false; break;
	case 2098:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Enmu_Neck0"); m_fFrustumRadiusRatio = 30.f; break;
	case 2099: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Enmu_Neck1"); m_fFrustumRadiusRatio = 30.f; break;
	case 2100:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Enmu_Neck2"); m_fFrustumRadiusRatio = 30.f; break;
	case 2101: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Train1"); m_fFrustumRadiusRatio = 30.f; break;
	case 2102:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Train2"); m_fFrustumRadiusRatio = 30.f;  break;
	case 2103: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Train3"); m_fFrustumRadiusRatio = 30.f; break;
	case 2104:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Trainbottom1"); m_fFrustumRadiusRatio = 30.f; break;
	case 2105: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Trainbottom2"); m_fFrustumRadiusRatio = 30.f; break;
	case 2106:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_TrainChiar1"); m_fFrustumRadiusRatio = 5.f; break;
	case 2107: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_TrainChiar2"); m_fFrustumRadiusRatio = 5.f; break;
	case 2108:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_TrainDoor"); m_fFrustumRadiusRatio = 5.f; break;
	case 2109:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Enmu_Train"); m_fFrustumRadiusRatio = 500.f; break;
	case 2110:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Enmu_NeckShield1"); m_fFrustumRadiusRatio = 20.f; break;
	case 2111:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Enmu_NeckShield2"); m_fFrustumRadiusRatio = 20.f; break;
	case 2112:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Enmu_Wall"); m_fFrustumRadiusRatio = 20.f; break;
	case 2113:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Enmu_Ground"); m_fFrustumRadiusRatio = 5000.f; break;


}

	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, pPrototypeTag_Model, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CMeshObj_Static::Move_Mesh(_float fTimeDelta)
{
	switch (m_tMyDesc.iModelIndex)
	{
	case 2097:
		m_pTransformCom->Go_StraightNoNavi(fTimeDelta);

		switch (m_tMyDesc.iTypeNum)
		{
		case 0:
			if (350.f < XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)))
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
			break;
		case 1:
			if (50.f < XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)))
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, -300.f, 1.f));
			break;
		case 2:
			if (-250.f < XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)))
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, -600.f, 1.f));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (g_iLevel == LEVEL_BOSSENMU)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		if (m_tMyDesc.iModelIndex == 2113 && dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Get_EnmuGround())
		{
			Set_Dead();
		}

		RELEASE_INSTANCE(CGameInstance);
	}
}

CMeshObj_Static * CMeshObj_Static::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMeshObj_Static*	pInstance = new CMeshObj_Static(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshObj_Static"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMeshObj_Static::Clone(void * pArg)
{
	CMeshObj_Static*	pInstance = new CMeshObj_Static(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshObj_Static"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshObj_Static::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
