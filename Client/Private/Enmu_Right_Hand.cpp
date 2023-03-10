#include "stdafx.h"
#include "..\Public\Enmu_Right_Hand.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"

#include "ImGuiManager.h"
#include "EnmuBoss.h"
#include "Effect_Manager.h"


CEnmu_Right_Hand::CEnmu_Right_Hand(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CEnmu_Right_Hand::CEnmu_Right_Hand(const CEnmu_Right_Hand & rhs)
	: CCharacters(rhs)
{
}

HRESULT CEnmu_Right_Hand::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnmu_Right_Hand::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_vector vPos = { 12.989f, 16.6f, 177.414f,1.f };
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->Set_Scale(XMVectorSet(0.75f, 0.75f, 0.75f, 0.f));

	//CUI_Manager::Get_Instance()->Set_2P(this);

	CEnmuBoss::Get_Instance()->Add_EnmuParts(this);
	//CImGuiManager::Get_Instance()->Add_LiveCharacter(this);
	Set_Info();
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTanjiro = dynamic_cast<CCharacters*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());


	m_vOriginalLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CEnmu_Right_Hand::Tick(_float fTimeDelta)
{

	//m_pModelCom->Play_Animation(fTimeDelta);

	HandleInput();
	TickState(fTimeDelta);

	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("R_Hand");
	if (nullptr == pSocket)
		return;

	_matrix			matColl = pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4()) * XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

	XMStoreFloat4x4(&m_WeaponWorld, matColl);

	m_pOBBCom->Update(matColl);

}

void CEnmu_Right_Hand::Late_Tick(_float fTimeDelta)
{

	LateTickState(fTimeDelta);
	if (!m_bCollBox)
	{
		m_bHit = false;
		m_bIsCreate = false;
	}
	else if (m_bCollBox)
	{
		
		CCollider*	pTargetCollider = m_pTanjiro->Get_SphereCollider();

		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return;

			if (m_pOBBCom->Collision(pTargetCollider))
			{

				if (m_pTanjiro->Get_PlayerInfo().bGuard && m_pTanjiro->Get_PlayerInfo().fGuardTime <= 0.f)
				{
					m_pTanjiro->Get_GuardHit(0);
					m_pTanjiro->Set_GuardHp(-60);
					if (m_pTanjiro->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTanjiro);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTanjiro->Set_ResetGuardHp();
						m_pTanjiro->Set_GuardTime(2.f);
					}
				}
				else if (m_pTanjiro->Get_GodMode() == false)
				{
					m_pTanjiro->Set_Hp(-50);

					if (m_bIsCreate == false)
					{
						//m_pTanjiro->Take_Damage(0.0f, false);
						m_pTanjiro->Player_UpperDown(Client::CCharacters::HIT_KNOCKBACK, 15.f, 20.f, 8.f);
						m_bIsCreate = true;
					}

				}
				if (m_pTanjiro->Get_GodMode() == false)
				{
					_int iDest = rand() % 5;
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					switch (iDest)
					{
					case 0:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTanjiro);
						break;
					case 1:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTanjiro);
						break;
					case 2:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTanjiro);
						break;
					case 3:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTanjiro);
						break;
					case 4:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTanjiro);
						break;
					default:
						break;
					}


					RELEASE_INSTANCE(CEffect_Manager);

					m_bHit = true;
				}


			}
		}
	}
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
	if (g_bCollBox)
	{
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}

}

HRESULT CEnmu_Right_Hand::Render()
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

		if (i == 0 || i == 2)
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

HRESULT CEnmu_Right_Hand::Render_ShadowDepth()
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

void CEnmu_Right_Hand::HandleInput()
{

}

void CEnmu_Right_Hand::TickState(_float fTimeDelta)
{

}

void CEnmu_Right_Hand::LateTickState(_float fTimeDelta)
{

}

HRESULT CEnmu_Right_Hand::SetUp_ShaderResources()
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

HRESULT CEnmu_Right_Hand::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Right_Hand"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Enmu_Normal"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(900.f, 250.f, 250.f);
	ColliderDesc.vPosition = _float3(0.f, 1.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CEnmu_Right_Hand::Set_Info()
{
	m_tInfo.strName = TEXT("엔무_오른손");
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

_float4 CEnmu_Right_Hand::Check_CollPos()
{

	CHierarchyNode*		pRightHand = m_pModelCom->Get_BonePtr("R_FingerE_End");
	CHierarchyNode*		pLeftHand = m_pModelCom->Get_BonePtr("R_FingerA_End");

	_matrix matPivot = XMLoadFloat4x4(&m_pModelCom->Get_PivotFloat4x4());
	_matrix matWorld = XMLoadFloat4x4(m_pTransformCom->Get_World4x4Ptr());

	_matrix matRightHand = pRightHand->Get_CombinedTransformationMatrix() * matPivot * matWorld;
	_matrix matLeftHand = pLeftHand->Get_CombinedTransformationMatrix() * matPivot * matWorld;


	_vector RightHandPos = matRightHand.r[3];
	_vector LeftHandPos = matLeftHand.r[3];

	_vector vPos = RightHandPos;
	_vector vLook = RightHandPos - LeftHandPos;

	_float fDist = XMVectorGetX(XMVector3Length(vLook));

	vPos -= XMVector3Normalize(vLook) * (fDist * 0.5f);
	_float4 vReturnPos;
	XMStoreFloat4(&vReturnPos, vPos);

	return vReturnPos;
}

void CEnmu_Right_Hand::Take_Damage(_float _fPow, _bool _bJumpHit)
{



}

void CEnmu_Right_Hand::Get_GuardHit(_int eType)
{

}

void CEnmu_Right_Hand::Player_TakeDown(_float _fPow, _bool _bJump)
{


}

void CEnmu_Right_Hand::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{


}

void CEnmu_Right_Hand::Play_Scene()
{
}

CEnmu_Right_Hand * CEnmu_Right_Hand::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEnmu_Right_Hand*	pInstance = new CEnmu_Right_Hand(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEnmu_Right_Hand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEnmu_Right_Hand::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEnmu_Right_Hand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEnmu_Right_Hand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnmu_Right_Hand::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
}
