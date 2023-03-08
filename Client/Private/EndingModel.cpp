#include "stdafx.h"
#include "..\Public\EndingModel.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "Data_Manager.h"
#include "SoundMgr.h"
#include "Data_Manager.h"
CEndingModel::CEndingModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CEndingModel::CEndingModel(const CEndingModel & rhs)
	: CCharacters(rhs)
{
}

HRESULT CEndingModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEndingModel::Initialize(void * pArg)
{
	m_iModelNum = *(_int*)pArg;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_vector vPos;
	switch (m_iModelNum)
	{
	case 0:
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ENDING, TEXT("Layer_Camera"))->Get_LayerFront())->Set_CamType(true);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ENDING, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Player(this);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ENDING, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);

		vPos = { -0.011f,16.412f,163.451f,1.f };
		m_pModelCom[m_iModelNum]->Set_CurrentAnimIndex(0);
		m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		break;
	case 1:
		vPos = { -0.341f,5.701f,214.219f,1.f };
		m_pModelCom[m_iModelNum]->Set_CurrentAnimIndex(66);
		m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		break;
	case 2:
		vPos = { -0.292f,5.75f,282.702f,1.f };
		m_pModelCom[m_iModelNum]->Set_CurrentAnimIndex(48);
		m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		break;
	case 3:
		vPos = { -0.477f,5.701f,366.157f,1.f };
		m_pModelCom[m_iModelNum]->Set_CurrentAnimIndex(29);
		m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		break;
	case 4:
		vPos = { -5.454f,5.701f,402.395f,1.f };
		m_pModelCom[m_iModelNum]->Set_CurrentAnimIndex(60);
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
		break;
	default:
		break;
	}

	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);


	RELEASE_INSTANCE(CGameInstance);


	
	return S_OK;
}

void CEndingModel::Tick(_float fTimeDelta)
{


	m_pModelCom[m_iModelNum]->Play_Animation(fTimeDelta);
}

void CEndingModel::Late_Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (pGameInstance->IsInFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 10.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CEndingModel::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom[m_iModelNum]->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom[m_iModelNum]->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom[m_iModelNum]->Render(m_pShaderCom, i, 0)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;



}

HRESULT CEndingModel::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	_vector vLightEye, vLightAt, vLightUp;
	_matrix matLightView;
	if (g_iLevel == LEVEL_GAMEPLAY)
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	else
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom[m_iModelNum]->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom[m_iModelNum]->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom[m_iModelNum]->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

HRESULT CEndingModel::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_fFar", &g_fFar, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEndingModel::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 15.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model0"), LEVEL_STATIC, TEXT("Muzan"), (CComponent**)&m_pModelCom[0])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model1"), LEVEL_STATIC, TEXT("Shinobu"), (CComponent**)&m_pModelCom[1])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model2"), LEVEL_STATIC, TEXT("Akaza"), (CComponent**)&m_pModelCom[2])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model3"), LEVEL_STATIC, TEXT("Nezuko"), (CComponent**)&m_pModelCom[3])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model4"), LEVEL_STATIC, TEXT("Rui"), (CComponent**)&m_pModelCom[4])))
		return E_FAIL;

	return S_OK;
}

void CEndingModel::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CEndingModel::Get_GuardHit(_int eType)
{
}

void CEndingModel::Player_TakeDown(_float _fPow, _bool _bJump)
{
}

void CEndingModel::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
}

void CEndingModel::Play_Scene()
{
}

CEndingModel * CEndingModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEndingModel*	pInstance = new CEndingModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEndingModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEndingModel::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEndingModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEndingModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEndingModel::Free()
{
	__super::Free();

	for (int i = 0; i < 5; ++i)
	{
		Safe_Release(m_pModelCom[i]);
	}
}
