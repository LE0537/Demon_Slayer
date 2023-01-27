#include "stdafx.h"
#include "..\Public\Rui.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "ImGuiManager.h"
#include "RuiState.h"
#include "RuiIdleState.h"
#include "RuiToolState.h"
using namespace Rui;


CRui::CRui(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CRui::CRui(const CRui & rhs)
	: CCharacters(rhs)
{
}

HRESULT CRui::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRui::Initialize(void * pArg)
{
	memcpy(&m_i1p, pArg, sizeof(_int));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_i1p == 1)
	{
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Player(this);

		Set_Info();
		CUI_Manager::Get_Instance()->Set_1P(this);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-3.f, 0.f, 0.f, 1.f));
	}
	else if (m_i1p == 2)
	{
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);

		Set_Info();
		CUI_Manager::Get_Instance()->Set_2P(this);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	}
	RELEASE_INSTANCE(CGameInstance);


	CRuiState* pState = new CIdleState();
	m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);


	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);


	return S_OK;
}

void CRui::Tick(_float fTimeDelta)
{
	/*
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (pGameInstance->Key_Down(DIK_F5))	
	//{
	//	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//	char cName[MAX_PATH];
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("Rui"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	//	ERR_MSG(TEXT("Save_Bin_Rui"));
	//	RELEASE_INSTANCE(CData_Manager);
	//}

	RELEASE_INSTANCE(CGameInstance);
*/

	Set_ShadowLightPos();

	HandleInput(); 
	TickState(fTimeDelta);

	
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");
	if (nullptr == pSocket)
		return;
	_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

	m_pSphereCom->Update(matColl);
}

void CRui::Late_Tick(_float fTimeDelta)
{
	LateTickState(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pSphereCom);
	}
}

HRESULT CRui::Render()
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

		//aiTextureType_AMBIENT
	}

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}
HRESULT CRui::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	_vector			vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
	_vector			vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
	_vector			vLightUp = { 0.f, 1.f, 0.f ,0.f };
	_matrix			matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

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
HRESULT CRui::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Rui"), (CComponent**)&m_pModelCom)))
		return E_FAIL;




	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(100.f, 100.f, 100.f);
	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}


void CRui::Set_ShadowLightPos()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	_float4 vAt = vPos;

	vPos.x -= 15.f;
	vPos.y += 30.f;
	vPos.z -= 30.f;

	pGameInstance->Set_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW, vPos, vAt);

	RELEASE_INSTANCE(CGameInstance);
}

void CRui::Set_Info()
{
	m_tInfo.strName = TEXT("∑Á¿Ã");
	m_tInfo.bOni = true;
	m_tInfo.iMaxHp = 1000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iSkMaxBar = 1000;
	m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
	m_tInfo.iUnicMaxBar = 1000;
	m_tInfo.iUnicBar = 0;
	m_tInfo.iDmg = 30;
	m_tInfo.iCombo = 0;
	m_tInfo.fComboTime = 0.f;
	m_tInfo.bPowerUp = false;
	m_tInfo.fPowerUpTime = 0.f;
	m_tInfo.iFriendMaxBar = 100;
	m_tInfo.iFriendBar;
}

void CRui::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CRuiState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CRuiState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pState);
}

void CRui::HandleInput()
{
	CRuiState* pNewState = m_pRuiState->HandleInput(this);

	if (pNewState)
		m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pNewState);
}


void CRui::TickState(_float fTimeDelta)
{
	CRuiState* pNewState = m_pRuiState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pNewState);
}

void CRui::LateTickState(_float fTimeDelta)
{
	CRuiState* pNewState = m_pRuiState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pRuiState = m_pRuiState->ChangeState(this, m_pRuiState, pNewState);

}

void CRui::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CRui::Get_GuardHit(_int eType)
{
}

HRESULT CRui::SetUp_ShaderResources()
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




CRui * CRui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRui*	pInstance = new CRui(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRui"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRui::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRui(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRui"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRui::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Delete(m_pRuiState);
}
