#include "stdafx.h"
#include "..\Public\Tanjiro.h"
#include "Layer.h"
#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
// state
#include "TanjiroState.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
#include "TanjiroHitState.h"
#include "TanjiroWeapon.h"
#include "TanjiroSheath.h"
#include "TanjiroGuardHitState.h"
#include "TanjiroToolState.h"
#include "ImGuiManager.h"
#include "Level_GamePlay.h"

using namespace Tanjiro;


CTanjiro::CTanjiro(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CTanjiro::CTanjiro(const CTanjiro & rhs)
	: CCharacters(rhs)
{
}

HRESULT CTanjiro::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTanjiro::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	if (FAILED(Ready_Parts2()))
		return E_FAIL;

	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;
	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
	m_pNavigationCom->Set_NaviIndex(tCharacterDesc.iNaviIndex);

	Set_Info();


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

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

	CTanjiroState* pState = new CIdleState();
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);


	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);


	return S_OK;
}

void CTanjiro::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	HandleInput();
	TickState(fTimeDelta);

	m_pModelCom->Get_PivotFloat4x4();
	m_pTransformCom->Get_World4x4Ptr();
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");
	if (nullptr == pSocket)
		return;
	_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

	m_pSphereCom->Update(matColl);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Down(DIK_F3))
		m_bIsKagura = m_bIsKagura == true ? false : true;

	RELEASE_INSTANCE(CGameInstance);


	if (m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_JUMP)
		m_tInfo.bJump = true;
	else
		m_tInfo.bJump = false;

}

void CTanjiro::Late_Tick(_float fTimeDelta)
{

	LateTickState(fTimeDelta);
	m_pWeapon->Tick(fTimeDelta);
	m_pSheath->Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	dynamic_cast<CTanjiroWeapon*>(m_pWeapon)->Set_Render(true);
	dynamic_cast<CTanjiroSheath*>(m_pSheath)->Set_Render(true);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pSheath);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pSheath);

	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pSphereCom);
	}
}

HRESULT CTanjiro::Render()
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
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_MaskTexture", i, aiTextureType_NORMALS)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 2)))
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

HRESULT CTanjiro::Render_ShadowDepth()
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


void CTanjiro::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	if(m_pTanjiroState->Get_TanjiroState() == CTanjiroState::STATE_HIT)
		m_pModelCom->Reset_Anim(CTanjiro::ANIMID::ANIM_HIT);

	CTanjiroState* pState = new CHitState(_fPow, _bJumpHit);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}

void CTanjiro::Get_GuardHit(_int eType)
{
	CTanjiroState* pState;
	if (eType == CTanjiroState::STATE_TYPE::TYPE_START)
	{
		m_pModelCom->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_HIT_0);
		pState = new CGuardHitState(CTanjiroState::STATE_TYPE::TYPE_START);
	}
	else
	{
		m_pModelCom->Reset_Anim(CTanjiro::ANIMID::ANIM_GUARD_HIT_1);
		pState = new CGuardHitState(CTanjiroState::STATE_TYPE::TYPE_LOOP);
	}
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}

void CTanjiro::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CTanjiroState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CTanjiroState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pState);
}


HRESULT CTanjiro::SetUp_ShaderResources()
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

HRESULT CTanjiro::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Tanjiro"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(100.f, 100.f, 100.f);
	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Rui"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;


	return S_OK;
}

void CTanjiro::HandleInput()
{

	CTanjiroState* pNewState = m_pTanjiroState->HandleInput(this);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);


}

void CTanjiro::TickState(_float fTimeDelta)
{
	CTanjiroState* pNewState = m_pTanjiroState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);
}

void CTanjiro::LateTickState(_float fTimeDelta)
{
	CTanjiroState* pNewState = m_pTanjiroState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pTanjiroState = m_pTanjiroState->ChangeState(this, m_pTanjiroState, pNewState);
}
HRESULT CTanjiro::Ready_Parts()
{

	/* For.Weapon */
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("R_Hand_1_Lct");
	if (nullptr == pSocket)
		return E_FAIL;

	CTanjiroWeapon::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeapon = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_TanjiroWeapon"), &WeaponDesc);
	if (nullptr == m_pWeapon)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTanjiro::Ready_Parts2()
{
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CTanjiroSheath::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pSheath = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_TanjiroSheath"), &WeaponDesc);
	if (nullptr == m_pSheath)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
void CTanjiro::Set_Info()
{
	m_tInfo.strName = TEXT("ÅºÁö·Î");
	m_tInfo.bOni = false;
	m_tInfo.iMaxHp = 1000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iSkMaxBar = 1000;
	m_tInfo.iSkBar = m_tInfo.iSkMaxBar;
	m_tInfo.iUnicMaxBar = 1000;
	m_tInfo.iUnicBar = 0;
	m_tInfo.iDmg = 10;
	m_tInfo.iCombo = 0;
	m_tInfo.fComboTime = 0.f;
	m_tInfo.bPowerUp = false;
	m_tInfo.fPowerUpTime = 0.f;
	m_tInfo.iFriendMaxBar = 1000;
	m_tInfo.iFriendBar = 0;
	m_tInfo.bGuard = false;
}
CTanjiro * CTanjiro::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTanjiro*	pInstance = new CTanjiro(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTanjiro"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTanjiro::Clone(void * pArg)
{
	CGameObject*	pInstance = new CTanjiro(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTanjiro"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTanjiro::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pModelCom);
	Safe_Delete(m_pTanjiroState);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pSheath);
	Safe_Release(m_pNavigationCom);
}
