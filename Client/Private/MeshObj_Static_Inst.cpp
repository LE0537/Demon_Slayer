#include "stdafx.h"
#include "..\Public\MeshObj_Static_Inst.h"

#include "GameInstance.h"
#include "Data_Manager.h"
CMeshObj_Static_Inst::CMeshObj_Static_Inst(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CMeshObj_Static_Inst::CMeshObj_Static_Inst(const CMeshObj_Static_Inst & rhs)
	: CGameObj(rhs)
{
}

void CMeshObj_Static_Inst::Change_Model(_uint iObjNumber, const _tchar * pModel_PrototypeTag)
{
	if (FAILED(Delete_Component(TEXT("Com_Model"))))
		return;

	Safe_Release(m_pModelCom);

	__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, pModel_PrototypeTag, (CComponent**)&m_pModelCom);
	if (nullptr == m_pModelCom)
		return;
}

HRESULT CMeshObj_Static_Inst::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMeshObj_Static_Inst::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_bRenderShadow = true;
	memcpy(&m_tMyDesc, pArg, sizeof m_tMyDesc);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	for (_uint i = 0; i < m_tMyDesc.iNumInstancing; ++i)
	{
		VTXMATRIX	VtxMatrix;
		memcpy(&VtxMatrix, &m_tMyDesc.pWorld[i], sizeof VtxMatrix);

		m_vecMatrix.push_back(VtxMatrix);
	}

	if(true == m_bRenderShadow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_STATIC_SHADOWDEPTH, this);

	_float fFovy = XMConvertToRadians(25.0f);
	_float fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	_float fNear = 0.2f;
	_float fFar = 1800.f;

	XMStoreFloat4x4(&m_matProjOrigin, XMMatrixPerspectiveFovLH(fFovy, fAspect, fNear, fFar));
	
	m_bInit = false;

	if (g_iLevel == 3 || g_iLevel == 4)
	{
		switch (m_tMyDesc.iTypeNum)
		{
		case 0:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
			break;
		case 1:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, -500.f, 1.f));
			break;
		case 2:
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 500.f, 1.f));
			break;
		default:
			break;
		}
	}

	return S_OK;
}

void CMeshObj_Static_Inst::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (pGameInstance->Key_Down(DIK_F6))
	//{

	//	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//	char cName[MAX_PATH];
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	if (2106 == m_tMyDesc.iModelIndex)
	//	{
	//		pData_Manager->TCtoC(TEXT("TrainChiar1_Instancing"), cName);
	//		ERR_MSG(L"Clear1");
	//	}
	//	if (2107 == m_tMyDesc.iModelIndex)
	//	{
	//		pData_Manager->TCtoC(TEXT("TrainChiar2_Instancing"), cName);
	//		ERR_MSG(L"Clear1");
	//	}
	//	if (2108 == m_tMyDesc.iModelIndex)
	//	{
	//		pData_Manager->TCtoC(TEXT("TrainDoor_Instancing"), cName);
	//		ERR_MSG(L"Clear1");
	//	}
	//	pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_NONANIM_INSTANCING);
	//	RELEASE_INSTANCE(CData_Manager);

	//}

	//RELEASE_INSTANCE(CGameInstance);
	if (false == m_bInit)
	{
		m_pModelCom->Update_Instancing(m_vecMatrix, 300000, fTimeDelta);
		m_bInit = true;
	}
	else
		m_pModelCom->Update_Instancing(m_vecMatrix, m_fFrustumRadiusRatio, fTimeDelta);

	if (g_iLevel == 3 || g_iLevel == 4)
		Move_Mesh(fTimeDelta);
}

void CMeshObj_Static_Inst::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	}
}

HRESULT CMeshObj_Static_Inst::Render()
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
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}



	return S_OK;
}

HRESULT CMeshObj_Static_Inst::Render_ShadowDepth()
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
		_vector			vLightEye = XMLoadFloat4(&pLightDesc->vDirection);
		_vector			vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDiffuse);
		_vector			vLightUp = { 0.f, 1.f, 0.f ,0.f };
		_matrix			matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
			return E_FAIL;
	}
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProjOrigin)), sizeof(_float4x4))))
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

HRESULT CMeshObj_Static_Inst::Delete_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_Components.erase(iter);

	return S_OK;
}

HRESULT CMeshObj_Static_Inst::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORMDESC));
	tTransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tTransformDesc.fSpeedPerSec = 100.f;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Ready_ModelComponent()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshObj_Static_Inst::SetUp_ShaderResources()
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


	return S_OK;
}

HRESULT CMeshObj_Static_Inst::Ready_ModelComponent()
{
	_tchar	pPrototypeTag_Model[MAX_PATH] = L"";
	CModel::MESHINSTANCINGDESC tMeshInstancingDesc;
	tMeshInstancingDesc.iNumMeshInstancing = m_tMyDesc.iNumInstancing;
	switch (m_tMyDesc.iModelIndex)
	{
	case 2001: lstrcpy(pPrototypeTag_Model, L"BigTree1_Instancing"); m_fFrustumRadiusRatio = 7000.f; break;
	case 2002: lstrcpy(pPrototypeTag_Model, L"BigTree2_Instancing"); m_fFrustumRadiusRatio = 7000.f; break;
	case 2003: lstrcpy(pPrototypeTag_Model, L"BigTree3_Instancing"); m_fFrustumRadiusRatio = 7000.f; break;
	case 2004: lstrcpy(pPrototypeTag_Model, L"TreeFar1_Instancing"); m_fFrustumRadiusRatio = 7.f;  m_bRenderShadow = false; break;
	case 2005: lstrcpy(pPrototypeTag_Model, L"TreeWillow_Instancing"); m_fFrustumRadiusRatio = 20.f; break;

	case 2006: lstrcpy(pPrototypeTag_Model, L"TreeBroken1_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2007: lstrcpy(pPrototypeTag_Model, L"TreeBroken2_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2008: lstrcpy(pPrototypeTag_Model, L"TreeBroken3_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2009: lstrcpy(pPrototypeTag_Model, L"TreeBroken4_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2010: lstrcpy(pPrototypeTag_Model, L"TreeBroken5_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2011: lstrcpy(pPrototypeTag_Model, L"TreeBroken6_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2012: lstrcpy(pPrototypeTag_Model, L"TreeBroken7_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2013: lstrcpy(pPrototypeTag_Model, L"TreeBroken8_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2014: lstrcpy(pPrototypeTag_Model, L"TreeBroken9_Instancing"); m_fFrustumRadiusRatio = 6.f; break;
	case 2015: lstrcpy(pPrototypeTag_Model, L"TreeBroken10_Instancing"); m_fFrustumRadiusRatio = 6.f; break;

	case 2016: lstrcpy(pPrototypeTag_Model, L"TreeRoot1_Instancing"); m_fFrustumRadiusRatio = 10.f; break;
	case 2017: lstrcpy(pPrototypeTag_Model, L"TreeRoot2_Instancing"); m_fFrustumRadiusRatio = 10.f; break;
	case 2018: lstrcpy(pPrototypeTag_Model, L"TreeRoot3_Instancing"); m_fFrustumRadiusRatio = 10.f; break;

	case 2019: lstrcpy(pPrototypeTag_Model, L"Rock1_Instancing"); m_fFrustumRadiusRatio = 50.f; break;
	case 2020: lstrcpy(pPrototypeTag_Model, L"Rock2_Instancing"); m_fFrustumRadiusRatio = 50.f; break;
	case 2021: lstrcpy(pPrototypeTag_Model, L"Rock3_Instancing"); m_fFrustumRadiusRatio = 50.f; break;
	case 2022: lstrcpy(pPrototypeTag_Model, L"Rock4_Instancing"); m_fFrustumRadiusRatio = 50.f; break;
	case 2023: lstrcpy(pPrototypeTag_Model, L"Rock5_Instancing"); m_fFrustumRadiusRatio = 50.f; break;
	case 2024: lstrcpy(pPrototypeTag_Model, L"Rock6_Instancing"); m_fFrustumRadiusRatio = 50.f; break;
	case 2025: lstrcpy(pPrototypeTag_Model, L"Rock7_Instancing"); m_fFrustumRadiusRatio = 50.f; break;
	case 2026: lstrcpy(pPrototypeTag_Model, L"Rock_Small_Instancing"); m_fFrustumRadiusRatio = 10.f; break;

	case 2027: lstrcpy(pPrototypeTag_Model, L"Cliff1_Instancing"); m_fFrustumRadiusRatio = 12.f; break;
	case 2028: lstrcpy(pPrototypeTag_Model, L"Cliff2_Instancing"); m_fFrustumRadiusRatio = 12.f; break;
	case 2029: lstrcpy(pPrototypeTag_Model, L"Cliff3_Instancing"); m_fFrustumRadiusRatio = 12.f; break;
	case 2030: lstrcpy(pPrototypeTag_Model, L"Cliff_Small_Instancing"); m_fFrustumRadiusRatio = 4.f; break;

	case 2031: lstrcpy(pPrototypeTag_Model, L"Grass1_Instancing"); m_bRenderShadow = false; break;
	case 2032: lstrcpy(pPrototypeTag_Model, L"Grass2_Instancing"); m_bRenderShadow = false; break;
	case 2033: lstrcpy(pPrototypeTag_Model, L"Grass3_Instancing"); m_bRenderShadow = false; break;
	case 2034: lstrcpy(pPrototypeTag_Model, L"Grass4_Instancing"); m_bRenderShadow = false; break;
	case 2035: lstrcpy(pPrototypeTag_Model, L"Grass5_Instancing"); m_bRenderShadow = false; break;
	case 2036: lstrcpy(pPrototypeTag_Model, L"Grass6_Instancing"); m_bRenderShadow = false; break;

	case 2037: lstrcpy(pPrototypeTag_Model, L"Lavender_Instancing"); m_bRenderShadow = false; break;
	case 2038: lstrcpy(pPrototypeTag_Model, L"Flower1_Instancing"); m_bRenderShadow = false; break;
	case 2039: lstrcpy(pPrototypeTag_Model, L"Flower2_Instancing"); m_bRenderShadow = false; break;
	case 2040: lstrcpy(pPrototypeTag_Model, L"Flower3_Instancing"); m_bRenderShadow = false; break;

	case 2041: lstrcpy(pPrototypeTag_Model, L"Leaf1_Instancing"); m_bRenderShadow = false; break;
	case 2042: lstrcpy(pPrototypeTag_Model, L"Leaf2_Instancing"); m_bRenderShadow = false; break;
	case 2043: lstrcpy(pPrototypeTag_Model, L"Leaf3_Instancing"); m_bRenderShadow = false; break;
	case 2044: lstrcpy(pPrototypeTag_Model, L"Leaf4_Instancing"); m_bRenderShadow = false; break;

	case 2045: lstrcpy(pPrototypeTag_Model, L"Hill_Far1_Instancing"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2046: lstrcpy(pPrototypeTag_Model, L"Hill_Far2_Instancing"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2047: lstrcpy(pPrototypeTag_Model, L"Hill_Far3_Instancing"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2048: lstrcpy(pPrototypeTag_Model, L"Hill_Far4_Instancing"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2049: lstrcpy(pPrototypeTag_Model, L"Hill_Far5_Instancing"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2050: lstrcpy(pPrototypeTag_Model, L"Hill_Far6_Instancing"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;

	case 2051: lstrcpy(pPrototypeTag_Model, L"Wall1_Instancing"); m_fFrustumRadiusRatio = 30.f; break;
	case 2052: lstrcpy(pPrototypeTag_Model, L"Wall2_Instancing"); m_fFrustumRadiusRatio = 30.f; break;

	case 2053: lstrcpy(pPrototypeTag_Model, L"SpiderWeb1_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false;break;
	case 2054: lstrcpy(pPrototypeTag_Model, L"SpiderWeb2_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false;break;
	case 2055: lstrcpy(pPrototypeTag_Model, L"SpiderWeb3_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false;break;

	case 2056: lstrcpy(pPrototypeTag_Model, L"Bush1_Instancing"); m_bRenderShadow = false; break;
	case 2057: lstrcpy(pPrototypeTag_Model, L"Bush2_Instancing"); m_bRenderShadow = false; break;
	case 2058: lstrcpy(pPrototypeTag_Model, L"Bush3_Instancing"); m_bRenderShadow = false; break;
	case 2059: lstrcpy(pPrototypeTag_Model, L"Bush4_Instancing"); m_bRenderShadow = false; break;
	case 2060: lstrcpy(pPrototypeTag_Model, L"Bush5_Instancing"); m_bRenderShadow = false; break;
	case 2061: lstrcpy(pPrototypeTag_Model, L"Bush6_Instancing"); m_bRenderShadow = false; break;
	case 2062: lstrcpy(pPrototypeTag_Model, L"Bush7_Instancing"); m_bRenderShadow = false; break;
	case 2063: lstrcpy(pPrototypeTag_Model, L"Bush8_Instancing"); m_bRenderShadow = false; break;
	case 2064: lstrcpy(pPrototypeTag_Model, L"Bush9_Instancing"); m_bRenderShadow = false; break;

	case 2065: lstrcpy(pPrototypeTag_Model, L"HomeSmall1_Instancing"); break;
	case 2066: lstrcpy(pPrototypeTag_Model, L"HomeSmall2_Instancing"); break;

	case 2067: lstrcpy(pPrototypeTag_Model, L"RiceField1_Instancing"); m_fFrustumRadiusRatio = 120.f; break;

	case 2068: lstrcpy(pPrototypeTag_Model, L"RuiGround_Instancing"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2069: lstrcpy(pPrototypeTag_Model, L"UrokodakiGround_Instancing"); m_fFrustumRadiusRatio = 2000.f; break;

	case 2070: lstrcpy(pPrototypeTag_Model, L"RuiGround2_Instancing"); m_fFrustumRadiusRatio = 2000.f; break;
	case 2071: lstrcpy(pPrototypeTag_Model, L"Home1_Instancing"); break;
	case 2072: lstrcpy(pPrototypeTag_Model, L"Rubble1_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2073: lstrcpy(pPrototypeTag_Model, L"Rubble2_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2074: lstrcpy(pPrototypeTag_Model, L"SpiderWeb4_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2075: lstrcpy(pPrototypeTag_Model, L"SpiderWeb5_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2076: lstrcpy(pPrototypeTag_Model, L"SpiderWeb6_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2077: lstrcpy(pPrototypeTag_Model, L"SpiderWeb7_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2078: lstrcpy(pPrototypeTag_Model, L"SpiderWeb8_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2079: lstrcpy(pPrototypeTag_Model, L"SpiderWeb9_Instancing"); m_fFrustumRadiusRatio = 7.f; m_bRenderShadow = false; break;
	case 2080: lstrcpy(pPrototypeTag_Model, L"Tree_Jenitsu_Instancing"); m_fFrustumRadiusRatio = 70.f; break;
	case 2081: lstrcpy(pPrototypeTag_Model, L"TreeFar2_Instancing"); m_fFrustumRadiusRatio = 5000.f; m_bRenderShadow = false; break;
	case 2082: lstrcpy(pPrototypeTag_Model, L"TreeFar3_Instancing"); m_fFrustumRadiusRatio = 5.f; m_bRenderShadow = false; break;

	case 2083: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_Moon_Instancing"); m_fFrustumRadiusRatio = 2000.f; m_bRenderShadow = false; break;
	case 2106: lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_TrainChiar1_Instancing"); m_fFrustumRadiusRatio = 5.f; break;
	case 2107:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_TrainChiar2_Instancing"); m_fFrustumRadiusRatio = 5.f;  break;
	case 2108:lstrcpy(pPrototypeTag_Model, L"Prototype_Component_Model_TrainDoor_Instancing"); m_fFrustumRadiusRatio = 5.f;  break;

	}


	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, pPrototypeTag_Model, (CComponent**)&m_pModelCom, &tMeshInstancingDesc)))
		return E_FAIL;

	return S_OK;
}

void CMeshObj_Static_Inst::Move_Mesh(_float fTimeDelta)
{
	switch (m_tMyDesc.iModelIndex)
	{
	case 2001:
	case 2002:
	case 2003:
	case 2004:
	case 2005:

	case 2006:
	case 2007:
	case 2008:
	case 2009:
	case 2010:
	case 2011:
	case 2012:
	case 2013:
	case 2014:
	case 2015:

	case 2016:
	case 2017:
	case 2018:

	case 2019:
	case 2020:
	case 2021:
	case 2022:
	case 2023:
	case 2024:
	case 2025:
	case 2026:

	case 2027:
	case 2028:
	case 2029:
	case 2030:

	case 2031:
	case 2032:
	case 2033:
	case 2034:
	case 2035:
	case 2036:

	case 2037:
	case 2038:
	case 2039:
	case 2040:

	case 2041:
	case 2042:
	case 2043:
	case 2044:

	case 2045:
	case 2046:
	case 2047:
	case 2048:
	case 2049:
	case 2050:

	case 2051:
	case 2052:

	case 2053:
	case 2054:
	case 2055:

	case 2056:
	case 2057:
	case 2058:
	case 2059:
	case 2060:
	case 2061:
	case 2062:
	case 2063:
	case 2064:

	case 2065:
	case 2066:

	case 2067:

	case 2068:
	case 2069:

	case 2070:
	case 2071:
	case 2072:
	case 2073:
	case 2074:
	case 2075:
	case 2076:
	case 2077:
	case 2078:
	case 2079:
	case 2080:
	case 2081:
	case 2082:
	case 2083:
		m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
		if (550.f < XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)))
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, -800.f, 1.f));
		break;
	default:
		break;
	}
}

CMeshObj_Static_Inst * CMeshObj_Static_Inst::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMeshObj_Static_Inst*	pInstance = new CMeshObj_Static_Inst(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshObj_Static_Inst"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMeshObj_Static_Inst::Clone(void * pArg)
{
	CMeshObj_Static_Inst*	pInstance = new CMeshObj_Static_Inst(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshObj_Static_Inst"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshObj_Static_Inst::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
