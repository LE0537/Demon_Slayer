#include "stdafx.h"
#include "..\Public\Door.h"

#include "GameInstance.h"
#include "Data_Manager.h"
#include "UI_Manager.h"
#include "Tanjiro.h"
#include "Layer.h"
CDoor::CDoor(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CDoor::CDoor(const CDoor & rhs)
	: CGameObj(rhs)
{
}
HRESULT CDoor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tMyDesc, pArg, sizeof m_tMyDesc);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_matrix		matWorld = XMLoadFloat4x4(&m_tMyDesc.matWorld);
	m_pTransformCom->Set_WorldMatrix(matWorld);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	pUI_Manager->Set_vecDoorInfo(m_tMyDesc.iModelIndex, this);

	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

void CDoor::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	Move_Mesh(fTimeDelta);
	if(m_tMyDesc.iModelIndex == 0)
		Event_Check(fTimeDelta);
}

void CDoor::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);


	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CDoor::Render()
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
		if (!m_bNonAlpha && i == 0)
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}
		else if (m_bNonAlpha && i == 1)
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
				return E_FAIL;
		}
	}

	m_bNonAlpha = !m_bNonAlpha;

	return S_OK;
}

HRESULT CDoor::Render_ShadowDepth()
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
		_vector			vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDirection);
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

HRESULT CDoor::Delete_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return E_FAIL;

	Safe_Release(iter->second);
	m_Components.erase(iter);

	return S_OK;
}

HRESULT CDoor::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(CTransform::TRANSFORMDESC));
	tTransformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tTransformDesc.fSpeedPerSec = 20.f;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &tTransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Ready_ModelComponent()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor::SetUp_ShaderResources()
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

HRESULT CDoor::Ready_ModelComponent()
{

	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, L"Prototype_Component_Model_TrainDoor", (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CDoor::Move_Mesh(_float fTimeDelta)
{
	_vector vPos = m_tMyDesc.m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float fDist = XMVectorGetX(XMVector3Length(vPos - vMyPos));

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager*		pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (fDist < 3.f)
	{
		if(!m_bTurn && m_fTurnAngle < 80.f)
			dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Set_Stop(true);
		m_bColCheck = true;
		if (!m_bUICreat)
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InteractionUI"), LEVEL_ADVAKAZA, TEXT("Layer_InteractionUI"), &m_tMyDesc.iModelIndex)))
				return;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_QuiestKeyUI"), LEVEL_ADVAKAZA, TEXT("Layer_KeyUI"), &m_tMyDesc.iModelIndex)))
				return;
			m_bUICreat = true;
		}

		if (m_bTurn == true && !m_bInteractionCheck)
		{
			pUI_Manager->Set_InteractionOn();
		}
		else if (m_bTurn == false && !m_bInteractionCheck)
		{
			pUI_Manager->Set_InteractionOn();
		}

		if (pGameInstance->Key_Down(DIK_F))
		{
			pUI_Manager->Set_InteractionOff();
			m_bTurn = !m_bTurn;
			m_bInteractionCheck = true;
			m_bKeyDownCheck = true;
		}

		if (m_bTurn && m_fTurnAngle < 90.f)
		{
			m_pTransformCom->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-3.f));
			m_fTurnAngle += 3.f;
		}
		else if (!m_bTurn && m_fTurnAngle > 0.f)
		{
			m_pTransformCom->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(3.f));
			m_fTurnAngle -= 3.f;
		}
		if (m_tMyDesc.iModelIndex == 4 && m_fTurnAngle >= 90.f)
		{
			//엔무 조우 연출 
			/*CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
			pUIManager->Set_EnmuBattle(true);
			RELEASE_INSTANCE(CUI_Manager);*/
		}

		if (m_fTurnAngle <= 0.f && m_bKeyDownCheck)
		{
			m_bInteractionCheck = false;
			m_bUICreat = false;
			m_bKeyDownCheck = false;
		}
		else if (m_fTurnAngle >= 90.f && m_bKeyDownCheck)
		{
			m_bInteractionCheck = false;
			m_bUICreat = false;
			m_bKeyDownCheck = false;
			dynamic_cast<CTanjiro*>(pGameInstance->Find_Layer(g_iLevel, TEXT("Layer_Tanjiro"))->Get_LayerFront())->Set_Stop(false);
		}
		
	}
	else
	{
		m_bInteractionCheck = false;
		m_bColCheck = false;
		m_bUICreat = false;
	}
	

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CDoor::Event_Check(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (m_bTurn && !m_bEventCheck)
	{
		m_fEventStart = fTimeDelta;

		if (m_fEventStart >= 3.f)
		{
			switch (pUI_Manager->Get_MsgCount())
			{
			case 0:
				pUI_Manager->Set_MsgOn();
				pUI_Manager->Set_MsgName(TEXT("카마도 탄지로"));
				pUI_Manager->Set_Msg(TEXT("사람들이 잠들어 있다...?"));
				break;
			case 1:
				pUI_Manager->Set_MsgOn();
				pUI_Manager->Set_MsgName(TEXT("카마도 탄지로"));
				pUI_Manager->Set_Msg(TEXT("혈귀가 수를 쓴거야"));
				break;
			case 2:
				pUI_Manager->Set_MsgOn();
				pUI_Manager->Set_MsgName(TEXT("카마도 탄지로"));
				pUI_Manager->Set_Msg(TEXT("어떻게 이토록 강한 혈귀술을..."));
				pUI_Manager->Reset_MsgCount();
				m_bEventCheck = true;
				break;
			default:
				break;
			}

		}
		
		
	}

	RELEASE_INSTANCE(CUI_Manager);
}

CDoor * CDoor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDoor*	pInstance = new CDoor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CDoor::Clone(void * pArg)
{
	CDoor*	pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
