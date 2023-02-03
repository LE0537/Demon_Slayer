#include "stdafx.h"
#include "..\Public\Kyoujuro.h"
#include "Layer.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "KyoujuroSheath.h"
#include "Camera_Dynamic.h"
#include "KyoujuroState.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroMoveState.h"
#include "KyoujuroHitState.h"
#include "KyoujuroGuardHitState.h"
#include "KyoujuroToolState.h"
#include "ImGuiManager.h"
#include "Level_GamePlay.h"
#include "KyoujuroBattleSTState.h"

using namespace Kyoujuro;

#include "UI_Manager.h"

CKyoujuro::CKyoujuro(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CKyoujuro::CKyoujuro(const CKyoujuro & rhs)
	: CCharacters(rhs)
{
}

HRESULT CKyoujuro::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CKyoujuro::Initialize(void * pArg)
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
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
		m_bChange = true;
		if(m_i1p == 1)
			CUI_Manager::Get_Instance()->Set_1P_2(this);
		else if(m_i1p == 2)
			CUI_Manager::Get_Instance()->Set_2P_2(this);
	}
	//CKyoujuroState* pState = new CIdleState();
	//m_pKyoujuroState = m_pKyoujuroState->ChangeState(this, m_pKyoujuroState, pState);


	//m_pTransformCom->Set_PlayerLookAt(m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	CKyoujuroState* pState = new CBattleStartState();
	m_pKyoujuroState = m_pKyoujuroState->ChangeState(this, m_pKyoujuroState, pState);

	CImGuiManager::Get_Instance()->Add_LiveCharacter(this);

	return S_OK;
}


void CKyoujuro::Tick(_float fTimeDelta)
{
	if (!m_bChange)
	{
		__super::Tick(fTimeDelta);
		m_fDelta = fTimeDelta;
		if (m_tInfo.fHitTime > 0.f)
			m_tInfo.fHitTime -= fTimeDelta;
		if (m_fChangeDelay > 0.f)
			m_fChangeDelay -= fTimeDelta;
		if (m_tInfo.fHitTime <= 0.f && !m_tInfo.bSub)
			HandleInput();

		TickState(fTimeDelta);

		m_pModelCom->Get_PivotFloat4x4();
		m_pTransformCom->Get_World4x4Ptr();
		CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("C_Spine_3");
		if (nullptr == pSocket)
			return;
		_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

		m_pSphereCom->Update(matColl);


	}
	if (m_pKyoujuroState->Get_TanjiroState() == CKyoujuroState::STATE_JUMP
		|| m_pKyoujuroState->Get_TanjiroState() == CKyoujuroState::STATE_CHANGE)
		m_tInfo.bJump = true;
	else
		m_tInfo.bJump = false;

}

void CKyoujuro::Late_Tick(_float fTimeDelta)
{

	if (!m_bChange)
	{
		LateTickState(fTimeDelta);

		m_pWeapon->Tick(fTimeDelta);
		m_pSheath->Tick(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		dynamic_cast<CKyoujuroWeapon*>(m_pWeapon)->Set_Render(true);
		dynamic_cast<CKyoujuroSheath*>(m_pSheath)->Set_Render(true);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pWeapon);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, m_pSheath);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pWeapon);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_pSheath);

		
			if (g_bCollBox)
			{
				m_pRendererCom->Add_Debug(m_pSphereCom);
			}
		
	}


}

HRESULT CKyoujuro::Render()
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
			
			//aiTextureType_NORMALS
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 2)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}
	}
	if (!m_tInfo.bChange && m_fChangeDelay <= 0.f)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
		vPos.m128_f32[1] += 15.f;
		switch (m_i1p)
		{
		case 1:
			if (m_tInfo.iFriendBar >= 100 && pGameInstance->Key_Pressing(DIK_U))
			{
				m_fChangeTime += m_fDelta;
				if (m_fChangeTime > 0.5f)
				{
					m_tInfo.iFriendBar -= 100;
					m_tInfo.bSub = true;
					CUI_Manager::Get_Instance()->Set_1P(m_pSubChar);
					CUI_Manager::Get_Instance()->Set_1P_2(this);
					m_pSubChar->Set_ChangeDelay(1.f);
					m_pSubChar->Set_Sub(false);
					m_pSubChar->Change_Info(m_tInfo);
					m_pSubChar->Set_ChangeInfo(true);
					m_pSubChar->Set_Change(false, vPos);
					m_pSubChar->Set_BattleTarget(m_pBattleTarget);
					m_pBattleTarget->Set_BattleTarget(m_pSubChar);
					m_fChangeTime = 0.f;
				}
			}
			else
			{
				m_fChangeTime = 0.f;
			}
			break;
		case 2:
			if (m_tInfo.iFriendBar >= 100 && pGameInstance->Key_Pressing(DIK_V))
			{
				m_fChangeTime += m_fDelta;
				if (m_fChangeTime > 0.5f)
				{
					m_tInfo.iFriendBar -= 100;
					m_tInfo.bSub = true;
					CUI_Manager::Get_Instance()->Set_2P(m_pSubChar);
					CUI_Manager::Get_Instance()->Set_2P_2(this);
					m_pSubChar->Set_ChangeDelay(1.f);
					m_pSubChar->Set_Sub(false);
					m_pSubChar->Change_Info(m_tInfo);
					m_pSubChar->Set_ChangeInfo(true);
					m_pSubChar->Set_Change(false, vPos);
					m_pSubChar->Set_BattleTarget(m_pBattleTarget);
					m_pBattleTarget->Set_BattleTarget(m_pSubChar);
					m_fChangeTime = 0.f;
				}
			}
			else
			{
				m_fChangeTime = 0.f;
			}
			break;
		default:
			break;
		}
	}
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CKyoujuro::Render_ShadowDepth()
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

HRESULT CKyoujuro::SetUp_ShaderResources()
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

HRESULT CKyoujuro::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Kyoujuro"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(130.f, 130.f, 130.f);
	ColliderDesc.vPosition = _float3(-30.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Rui"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;


	return S_OK;
}

void CKyoujuro::Set_ShadowLightPos()
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
HRESULT CKyoujuro::Ready_Parts()
{

	/* For.Weapon */
	//CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("R_Hand_1_Lct");
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("R_Hand_1_P0012_V00_C00_Lct");
	//CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("R_Hand_1_End");

	if (nullptr == pSocket)
		return E_FAIL;

	CKyoujuroWeapon::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeapon = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_KyoujuroWeapon"), &WeaponDesc);
	if (nullptr == m_pWeapon)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CKyoujuro::Ready_Parts2()
{
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CKyoujuroSheath::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pSheath = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_KyoujuroSheath"), &WeaponDesc);
	if (nullptr == m_pSheath)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CKyoujuro::Set_Info()
{
	m_tInfo.strName = TEXT("ÄìÁÖ·Î");
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
	m_tInfo.iFriendBar = m_tInfo.iFriendMaxBar;
	m_tInfo.bGuard = false;
	m_tInfo.bChange = false;
}
void CKyoujuro::Take_Damage(_float _fPow, _bool _bJumpHit)
{
	if (m_pKyoujuroState->Get_TanjiroState() == CKyoujuroState::STATE_HIT)
		m_pModelCom->Reset_Anim(CKyoujuro::ANIMID::ANIM_HIT);

	CKyoujuroState* pState = new CHitState(_fPow, _bJumpHit);
	m_pKyoujuroState = m_pKyoujuroState->ChangeState(this, m_pKyoujuroState, pState);

}
void CKyoujuro::Get_GuardHit(_int eType)
{
	CKyoujuroState* pState;
	if (eType == CKyoujuroState::STATE_TYPE::TYPE_START)
	{
		m_pModelCom->Reset_Anim(CKyoujuro::ANIMID::ANIM_GUARD_HIT_0);
		pState = new CGuardHitState(CKyoujuroState::STATE_TYPE::TYPE_START);
	}
	else
	{
		m_pModelCom->Reset_Anim(CKyoujuro::ANIMID::ANIM_GUARD_HIT_1);
		pState = new CGuardHitState(CKyoujuroState::STATE_TYPE::TYPE_LOOP);
	}

	m_pKyoujuroState = m_pKyoujuroState->ChangeState(this, m_pKyoujuroState, pState);
}


void CKyoujuro::Set_ToolState(_uint iAnimIndex, _uint iAnimIndex_2, _uint iAnimIndex_3, _uint iTypeIndex, _bool bIsContinue)
{
	CKyoujuroState* pState = new CToolState(iAnimIndex, iAnimIndex_2, iAnimIndex_3, static_cast<CKyoujuroState::STATE_TYPE>(iTypeIndex), bIsContinue);
	m_pKyoujuroState = m_pKyoujuroState->ChangeState(this, m_pKyoujuroState, pState);
}


void CKyoujuro::HandleInput()
{
	CKyoujuroState* pNewState = m_pKyoujuroState->HandleInput(this);

	if (pNewState)
		m_pKyoujuroState = m_pKyoujuroState->ChangeState(this, m_pKyoujuroState, pNewState);

}
void CKyoujuro::TickState(_float fTimeDelta)
{
	CKyoujuroState* pNewState = m_pKyoujuroState->Tick(this, fTimeDelta);

	if (pNewState)
		m_pKyoujuroState = m_pKyoujuroState->ChangeState(this, m_pKyoujuroState, pNewState);


}
void CKyoujuro::LateTickState(_float fTimeDelta)
{
	CKyoujuroState* pNewState = m_pKyoujuroState->Late_Tick(this, fTimeDelta);

	if (pNewState)
		m_pKyoujuroState = m_pKyoujuroState->ChangeState(this, m_pKyoujuroState, pNewState);

}
CKyoujuro * CKyoujuro::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CKyoujuro*	pInstance = new CKyoujuro(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CKyoujuro"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CKyoujuro::Clone(void * pArg)
{
	CGameObject*	pInstance = new CKyoujuro(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CKyoujuro"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKyoujuro::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pSheath);
	Safe_Release(m_pNavigationCom);

	Safe_Delete(m_pKyoujuroState);
}
