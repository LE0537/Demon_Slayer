#include "stdafx.h"
#include "..\Public\Enmu_Left_Hand.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "RuiDadIdleState.h"
#include "ImGuiManager.h"
#include "Tanjiro.h"
#include "RuiDadHitState.h"
#include "RuiDadGuardHitState.h"
#include "AngryState.h"


CEnmu_Left_Hand::CEnmu_Left_Hand(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CEnmu_Left_Hand::CEnmu_Left_Hand(const CEnmu_Left_Hand & rhs)
	: CCharacters(rhs)
{
}

HRESULT CEnmu_Left_Hand::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnmu_Left_Hand::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_vector vPos = { -12.012f, 16.6f, 176.414f,1.f };
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

	CUI_Manager::Get_Instance()->Set_2P(this);

	m_pModelCom->Set_CurrentAnimIndex(40);

	//CImGuiManager::Get_Instance()->Add_LiveCharacter(this);
	Set_Info();
	return S_OK;
}

void CEnmu_Left_Hand::Tick(_float fTimeDelta)
{

	m_pModelCom->Play_Animation(fTimeDelta);

	HandleInput();
	TickState(fTimeDelta);

}

void CEnmu_Left_Hand::Late_Tick(_float fTimeDelta)
{

	LateTickState(fTimeDelta);
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}


}

HRESULT CEnmu_Left_Hand::Render()
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

		if (i == 0 || i == 1)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_NormalTexture", m_pTextureCom->Get_SRV(1))))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}

	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;



}

HRESULT CEnmu_Left_Hand::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (m_bShadowAlphaIncrease == false)
		m_ShadowMatrix = m_pTransformCom->Get_WorldMatrix();

	if (m_bShadowAlphaDecrease == true)
		m_ShadowMatrix = m_pTransformCom->Get_WorldMatrix();


	_float4 vTemp = *(_float4*)&m_ShadowMatrix.r[3];

	if (m_bShadowAlphaIncrease == true)
	{
		vTemp.x += 100.f;
		vTemp.y += 100.f;
		vTemp.z += 100.f;
	}

	*(_float4*)&m_ShadowMatrix.r[3] = vTemp;

	//_float4x4 WorldMatrix = m_pTransformCom->Get_World4x4();

	_float4x4	TransposeMatrix;
	XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(m_ShadowMatrix));


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &TransposeMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_vector vLightEye, vLightAt, vLightUp;
	_matrix matLightView;

	vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
	vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
	vLightUp = { 0.f, 1.f, 0.f ,0.f };
	matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

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

void CEnmu_Left_Hand::HandleInput()
{

}

void CEnmu_Left_Hand::TickState(_float fTimeDelta)
{

}

void CEnmu_Left_Hand::LateTickState(_float fTimeDelta)
{

}

HRESULT CEnmu_Left_Hand::SetUp_ShaderResources()
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

HRESULT CEnmu_Left_Hand::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Left_Hand"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Enmu_Normal"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

void CEnmu_Left_Hand::Set_Info()
{
	m_tInfo.strName = TEXT("¿£¹«(½¯µå)");
	m_tInfo.bOni = true;
	m_tInfo.iMaxHp = 300;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iSkMaxBar = 1000;
	m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
	m_tInfo.iUnicMaxBar = 1000;
	m_tInfo.iUnicBar = 0;
	m_tInfo.iUnicCount = 0;
	m_tInfo.iDmg = 20;
	m_tInfo.iCombo = 0;
	m_tInfo.fComboTime = 0.f;
	m_tInfo.fPowerUp = 1.f;
	m_tInfo.fPowerUpTime = 0.f;
	m_tInfo.iFriendMaxBar = 1000;
	m_tInfo.iFriendBar = m_tInfo.iFriendMaxBar;
	m_tInfo.bGuard = false;
	m_tInfo.bChange = false;
	m_tInfo.iMaxGuard = 500;
	m_tInfo.iGuard = m_tInfo.iMaxGuard;
}

void CEnmu_Left_Hand::Take_Damage(_float _fPow, _bool _bJumpHit)
{

}

void CEnmu_Left_Hand::Get_GuardHit(_int eType)
{

}

void CEnmu_Left_Hand::Player_TakeDown(_float _fPow, _bool _bJump)
{


}

void CEnmu_Left_Hand::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{


}

void CEnmu_Left_Hand::Play_Scene()
{
}

CEnmu_Left_Hand * CEnmu_Left_Hand::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEnmu_Left_Hand*	pInstance = new CEnmu_Left_Hand(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEnmu_Left_Hand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEnmu_Left_Hand::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEnmu_Left_Hand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEnmu_Left_Hand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnmu_Left_Hand::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);

}
