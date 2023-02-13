#include "stdafx.h"
#include "..\Public\RuiDad.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "RuiDadIdleState.h"

using namespace RuiDad;

CRuiDad::CRuiDad(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CRuiDad::CRuiDad(const CRuiDad & rhs)
	: CCharacters(rhs)
{
}

HRESULT CRuiDad::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRuiDad::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;
	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (m_i1p != 10)
	{
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
		m_pNavigationCom->Set_NaviIndex(tCharacterDesc.iNaviIndex);

		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
		if (!m_tInfo.bSub)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			*(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar) = this;
			if (m_i1p == 1)
			{
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Player(this);

				CUI_Manager::Get_Instance()->Set_1P(this);
			}
			else if (m_i1p == 2)
			{
				dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);

				CUI_Manager::Get_Instance()->Set_2P(this);
			}

			RELEASE_INSTANCE(CGameInstance);

		}
		else
		{
			m_pSubChar = *(CCharacters**)(&((CLevel_GamePlay::CHARACTERDESC*)pArg)->pSubChar);
			m_pSubChar->Set_SubChar(this);


		}
	}
	else if (m_i1p == 10)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_ADVRUI, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Target(this);
		RELEASE_INSTANCE(CGameInstance);
		_vector vPos = { -860.374f,92.52f,-68.017f,1.f };
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	//	m_pTransformCom->Set_Scale(XMVectorSet(0.25f, 0.25f, 0.25f, 0.f));
		m_pNavigationCom->Find_CurrentCellIndex(vPos);

		m_tInfo.bSub = tCharacterDesc.bSub;
		m_bChange = tCharacterDesc.bSub;
		//CUI_Manager::Get_Instance()->Set_2P(this);
	}
	CRuiDadState* pState = new CIdleState();
	m_pRuiDadState = m_pRuiDadState->ChangeState(this, m_pRuiDadState, pState);

	//CImGuiManager::Get_Instance()->Add_LiveCharacter(this);
	Set_Info();
	return S_OK;
}

void CRuiDad::Tick(_float fTimeDelta)
{
	HandleInput();
	TickState(fTimeDelta);
}

void CRuiDad::Late_Tick(_float fTimeDelta)
{
	LateTickState(fTimeDelta);

	m_pModelCom->Set_CurrentAnimIndex(38);
	m_pModelCom->Play_Animation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pSphereCom);
	}
}

HRESULT CRuiDad::Render()
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

HRESULT CRuiDad::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	_vector vLightEye, vLightAt, vLightUp;
	_matrix matLightView;
	if (g_iLevel == 1)
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	}
	else if (g_iLevel == 2)
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

void CRuiDad::HandleInput()
{
}

void CRuiDad::TickState(_float fTimeDelta)
{
}

void CRuiDad::LateTickState(_float fTimeDelta)
{
}

HRESULT CRuiDad::SetUp_ShaderResources()
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

HRESULT CRuiDad::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("RuiDad"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(130.f, 130.f, 130.f);
	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_RuiStory"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	return S_OK;
}

void CRuiDad::Set_Info()
{
	m_tInfo.strName = TEXT("아빠 거미");
	m_tInfo.bOni = true;
	m_tInfo.iMaxHp = 1000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iSkMaxBar = 1000;
	m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
	m_tInfo.iUnicMaxBar = 1000;
	m_tInfo.iUnicBar = 0;
	m_tInfo.iUnicCount = 0;
	m_tInfo.iDmg = 10;
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

void CRuiDad::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CRuiDad::Get_GuardHit(_int eType)
{
}

CRuiDad * CRuiDad::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRuiDad*	pInstance = new CRuiDad(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRuiDad"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRuiDad::Clone(void * pArg)
{
	CGameObject*	pInstance = new CRuiDad(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRuiDad"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuiDad::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pNavigationCom);

	Safe_Delete(m_pRuiDadState);
}
