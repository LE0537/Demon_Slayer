#include "stdafx.h"
#include "RuiSister.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "Data_Manager.h"

CRuiSister::CRuiSister(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CRuiSister::CRuiSister(const CRuiSister & rhs)
	: CCharacters(rhs)
{
}

HRESULT CRuiSister::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiSister::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;
	
	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	//m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
	//m_pTransformCom->Set_Scale(XMVectorSet(1.f, 1.f, 1.f, 0.f));
	m_pBattleTarget = tCharacterDesc.pSubChar;
	m_pModelCom->Set_CurrentAnimIndex(0);

	_vector vPos = { -860.374f,92.52f,-68.017f,1.f };
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

	m_pNavigationCom->Find_CurrentCellIndex(vPos);


	return S_OK;
}

void CRuiSister::Tick(_float fTimeDelta)
{

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float4 fPos;
	XMStoreFloat4(&fPos, vPos);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Down(DIK_F6))	// 추가 -> 저장하기
	{
		CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
		char cName[MAX_PATH];
		ZeroMemory(cName, sizeof(char) * MAX_PATH);
		pData_Manager->TCtoC(TEXT("RuiSister"), cName);
		pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
		ERR_MSG(TEXT("Save_Bin_RuiSister"));
		RELEASE_INSTANCE(CData_Manager);
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CRuiSister::Late_Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));

	if (pGameInstance->IsInFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 10.f))
	{
		if (fDist < 60.f)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			m_pModelCom->Play_Animation(fTimeDelta);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CRuiSister::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;



}

HRESULT CRuiSister::Render_ShadowDepth()
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
	else if (g_iLevel == LEVEL_ADVRUI)
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



HRESULT CRuiSister::SetUp_ShaderResources()
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

	return S_OK;
}

HRESULT CRuiSister::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("RuiSister"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_RuiStory"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	return S_OK;
}

void CRuiSister::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CRuiSister::Get_GuardHit(_int eType)
{
}

void CRuiSister::Player_TakeDown(_float _fPow, _bool _bJump)
{
}

void CRuiSister::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
}

void CRuiSister::Play_Scene()
{
}

CRuiSister * CRuiSister::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiSister*	pInstance = new CRuiSister(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiSister"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRuiSister::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiSister(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiSister"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiSister::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);
}
