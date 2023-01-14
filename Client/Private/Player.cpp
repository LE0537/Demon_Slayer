#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "SoundMgr.h"
#include "VIBuffer_Navigation.h"
#include "Data_Manager.h"	

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObj(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	*(CGameObject**)pArg = this;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	_float4 vLightPos;
	XMStoreFloat4(&vLightPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	vLightPos.y += 300.f;
	LightDesc.eType = LIGHTDESC::TYPE_FIELDSHADOW;
	LightDesc.vDirection = vLightPos;
	LightDesc.vDiffuse = _float4(380.f, 0.f, -1180.f, 1.f);
	LightDesc.vAmbient = _float4(0.f, 0.1f, 0.f, 0.f);


	if (FAILED(pGameInstance->Add_ShadowLight(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	//m_pTransformCom->Set_Scale(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(3.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	/*CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Down(DIK_F4))	
	{
		CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
		char cName[MAX_PATH];
		ZeroMemory(cName, sizeof(char) * MAX_PATH);
		pData_Manager->TCtoC(TEXT("Player"), cName);
		pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
		ERR_MSG(TEXT("Save_Bin_Player"));
		RELEASE_INSTANCE(CData_Manager);
	}

	RELEASE_INSTANCE(CGameInstance);*/

	__super::Tick(fTimeDelta);
	
	Key_Input(fTimeDelta);
		

	Set_ShadowLightPos();
	
	m_pModelCom->Play_Animation(fTimeDelta);

	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CPlayer::Late_Tick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	
	if (g_bDebug)
	{
		m_pRendererCom->Add_Debug(m_pAABBCom);
		m_pRendererCom->Add_Debug(m_pOBBCom);
	}
}

HRESULT CPlayer::Render()
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
HRESULT CPlayer::Render_ShadowDepth()
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
HRESULT CPlayer::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom,&TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Player"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}
void CPlayer::Key_Input(_float fTimeDelta)
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (pGameInstance->Key_Pressing(DIK_W))
	{
		m_pTransformCom->Go_StraightNoNavi(fTimeDelta);
	}

	if (pGameInstance->Key_Pressing(DIK_S))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}


	if (pGameInstance->Key_Pressing(DIK_A))
	{
		m_pTransformCom->Turn2(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-3.f));
	}
	else if (pGameInstance->Key_Pressing(DIK_D))
	{
		m_pTransformCom->Turn2(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(3.f));
	}


	RELEASE_INSTANCE(CGameInstance);
}
void CPlayer::Set_ShadowLightPos()
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

HRESULT CPlayer::SetUp_ShaderResources()
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




CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer*	pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CPlayer::Clone(void * pArg)
{
	CGameObject*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pModelCom);
}
