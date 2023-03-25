#include "stdafx.h"
#include "..\Public\Enmu_ChokChok.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "RuiDadIdleState.h"
#include "ImGuiManager.h"
#include "EnmuBoss.h"
#include "Effect_Manager.h"
#include "Tanjiro.h"
CEnmu_ChokChok::CEnmu_ChokChok(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CEnmu_ChokChok::CEnmu_ChokChok(const CEnmu_ChokChok & rhs)
	: CCharacters(rhs)
{
}

HRESULT CEnmu_ChokChok::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnmu_ChokChok::Initialize(void * pArg)
{

	if (pArg != nullptr)
	{
		m_tChokChokInfo.bEffect = ((CHOKCHOK*)(pArg))->bEffect;
		m_tChokChokInfo.vPosition = ((CHOKCHOK*)(pArg))->vPosition;

		//memcpy(&m_vOriginPosition, (_vector*)(pArg), sizeof(m_vOriginPosition));
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (m_tChokChokInfo.bEffect == true)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		CGameInstance* pGameInstance1 = CGameInstance::Get_Instance();
		m_tChokChokInfo.vPosition = XMVectorSetY(m_tChokChokInfo.vPosition, dynamic_cast<CTanjiro*>(pGameInstance1->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Get_NavigationHeight().y);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT2_GROUND, &m_tChokChokInfo.vPosition);

		RELEASE_INSTANCE(CEffect_Manager);
	}
	else
	{
		m_pTransformCom->Set_Scale(XMVectorSet(0.5f, 0.5f, 0.5f, 1.f));
		CGameInstance* pGameInstance1 = CGameInstance::Get_Instance();
		m_tChokChokInfo.vPosition = XMVectorSetY(m_tChokChokInfo.vPosition, dynamic_cast<CTanjiro*>(pGameInstance1->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Get_NavigationHeight().y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_tChokChokInfo.vPosition);
		m_pModelCom->Set_CurrentAnimIndex(0);
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_pTanjiro = dynamic_cast<CCharacters*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront());

		RELEASE_INSTANCE(CGameInstance);



		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_ENMUBOSS_PAT2_CHOK, &m_tChokChokInfo.vPosition);

		RELEASE_INSTANCE(CEffect_Manager);
	}


	return S_OK;
}

void CEnmu_ChokChok::Tick(_float fTimeDelta)
{
	if (m_tChokChokInfo.bEffect == true)
	{
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.8f)
		{
			CEnmu_ChokChok::CHOKCHOK tInfo{};
			ZeroMemory(&tInfo, sizeof(tInfo));
			tInfo.bEffect = false;
			tInfo.vPosition = m_tChokChokInfo.vPosition;
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			pGameInstance->Add_GameObject(L"Prototype_GameObject_Enmu_ChokChok", g_iLevel, L"Layer_ChokChok", &tInfo);
			m_bDead = true;
		}

	}
	else
	{

		if (m_pModelCom->Get_End(0))
		{
			m_pModelCom->Set_End(0);
			m_bDead = true;
		}


		HandleInput();
		TickState(fTimeDelta);
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	}

}

void CEnmu_ChokChok::Late_Tick(_float fTimeDelta)
{
	if (m_tChokChokInfo.bEffect == false)
	{
		m_pModelCom->Play_Animation(fTimeDelta);
		LateTickState(fTimeDelta);

		m_fMove += fTimeDelta;

		if (m_fMove > 0.3f)
		{

			CCollider*	pTargetCollider = m_pTanjiro->Get_SphereCollider();

			if (m_fMove < 0.5f && !m_bHit)
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
							m_pTanjiro->Set_GodMode(true);
							m_pTanjiro->Take_Damage(0.0f, false);
							//m_pTanjiro->Player_UpperDown(CCharacters::HIT_TYPE::HIT_BOUND, 20.f, 30.f, 0.f);
							//m_pTanjiro->Take_Damage(0.0f, false);
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
			m_pRendererCom->Add_CollBox(m_pOBBCom);
		}
	}

}

HRESULT CEnmu_ChokChok::Render()
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

HRESULT CEnmu_ChokChok::Render_ShadowDepth()
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

void CEnmu_ChokChok::HandleInput()
{

}

void CEnmu_ChokChok::TickState(_float fTimeDelta)
{

}

void CEnmu_ChokChok::LateTickState(_float fTimeDelta)
{

}

HRESULT CEnmu_ChokChok::SetUp_ShaderResources()
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

HRESULT CEnmu_ChokChok::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Enmu_Chok"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(6.f, 12.f, 6.f);
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

void CEnmu_ChokChok::Set_Info()
{
	m_tInfo.strName = TEXT("¿£¹«_ÃË¼ö");
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

void CEnmu_ChokChok::Take_Damage(_float _fPow, _bool _bJumpHit)
{

}

void CEnmu_ChokChok::Get_GuardHit(_int eType)
{

}

void CEnmu_ChokChok::Player_TakeDown(_float _fPow, _bool _bJump)
{


}

void CEnmu_ChokChok::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{


}

void CEnmu_ChokChok::Play_Scene()
{
}

CEnmu_ChokChok * CEnmu_ChokChok::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEnmu_ChokChok*	pInstance = new CEnmu_ChokChok(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEnmu_Chok"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEnmu_ChokChok::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEnmu_ChokChok(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEnmu_Chok"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnmu_ChokChok::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSphereCom);

}
