#include "stdafx.h"
#include "..\Public\TrainNPC.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "Layer.h"
#include "Level_GamePlay.h"
#include "Data_Manager.h"

CTrainNPC::CTrainNPC(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CTrainNPC::CTrainNPC(const CTrainNPC & rhs)
	: CCharacters(rhs)
{
}

HRESULT CTrainNPC::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrainNPC::Initialize(void * pArg)
{
	CLevel_GamePlay::CHARACTERDESC	tCharacterDesc;
	memcpy(&tCharacterDesc, pArg, sizeof CLevel_GamePlay::CHARACTERDESC);

	m_i1p = tCharacterDesc.i1P2P;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&tCharacterDesc.matWorld));
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	if(m_i1p == 5 || m_i1p == 6)
		vPos.m128_f32[1] = 5.2f;
	else
		vPos.m128_f32[1] = 4.7f;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransformCom->Set_Scale(XMVectorSet(3.f, 3.f, 3.f,0.f));
	if(tCharacterDesc.bSub)
		m_pTransformCom->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_pBattleTarget = tCharacterDesc.pSubChar;

	m_pModelCom[m_i1p]->Set_CurrentAnimIndex(0);
	return S_OK;
}

void CTrainNPC::Tick(_float fTimeDelta)
{

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (pGameInstance->Key_Down(DIK_F5))	// 추가 -> 저장하기
	//{
	//	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//	char cName[MAX_PATH];
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("NPCF1"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[0], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("NPCF2"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[1], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("NPCF3"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[2], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("NPCM1"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[3], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("NPCM2"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[4], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("NPC0"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[5], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("Kyou"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[6], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("KyouBro"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[7], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("KyouDad"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[8], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("Mom"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[9], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("RuiSis"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[10], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("Slayer1"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[11], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("Slayer2"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[12], cName, CData_Manager::DATA_ANIM);
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("Yosi"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom[13], cName, CData_Manager::DATA_ANIM);
	//	ERR_MSG(TEXT("Save_Bin_NPC"));
	//	RELEASE_INSTANCE(CData_Manager);
	//}

	//RELEASE_INSTANCE(CGameInstance);
}

void CTrainNPC::Late_Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vTargetPos = m_pBattleTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION)));


	if (pGameInstance->IsInFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 10.f))
	{
		if (fDist < 100.f)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
			m_pModelCom[m_i1p]->Play_Animation(fTimeDelta);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CTrainNPC::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom[m_i1p]->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom[m_i1p]->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom[m_i1p]->Render(m_pShaderCom, i, 0)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;



}

HRESULT CTrainNPC::Render_ShadowDepth()
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
	else if (g_iLevel == LEVEL_ADVAKAZA)
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



	_uint		iNumMeshes = m_pModelCom[m_i1p]->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom[m_i1p]->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom[m_i1p]->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}



HRESULT CTrainNPC::SetUp_ShaderResources()
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

HRESULT CTrainNPC::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model0"), LEVEL_STATIC, TEXT("NPCF1"), (CComponent**)&m_pModelCom[0])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model1"), LEVEL_STATIC, TEXT("NPCF2"), (CComponent**)&m_pModelCom[1])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model2"), LEVEL_STATIC, TEXT("NPCF3"), (CComponent**)&m_pModelCom[2])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model3"), LEVEL_STATIC, TEXT("NPCM1"), (CComponent**)&m_pModelCom[3])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model4"), LEVEL_STATIC, TEXT("NPCM2"), (CComponent**)&m_pModelCom[4])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model5"), LEVEL_STATIC, TEXT("NPC0"), (CComponent**)&m_pModelCom[5])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model6"), LEVEL_STATIC, TEXT("Kyou"), (CComponent**)&m_pModelCom[6])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model7"), LEVEL_STATIC, TEXT("KyouBro"), (CComponent**)&m_pModelCom[7])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model8"), LEVEL_STATIC, TEXT("KyouDad"), (CComponent**)&m_pModelCom[8])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model9"), LEVEL_STATIC, TEXT("Mom"), (CComponent**)&m_pModelCom[9])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model10"), LEVEL_STATIC, TEXT("RuiSis"), (CComponent**)&m_pModelCom[10])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model11"), LEVEL_STATIC, TEXT("Slayer1"), (CComponent**)&m_pModelCom[11])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model12"), LEVEL_STATIC, TEXT("Slayer2"), (CComponent**)&m_pModelCom[12])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Model13"), LEVEL_STATIC, TEXT("Yosi"), (CComponent**)&m_pModelCom[13])))
		return E_FAIL;


	return S_OK;
}

void CTrainNPC::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CTrainNPC::Get_GuardHit(_int eType)
{
}

void CTrainNPC::Player_TakeDown(_float _fPow, _bool _bJump)
{
}

void CTrainNPC::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
}

void CTrainNPC::Play_Scene()
{
}

CTrainNPC * CTrainNPC::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTrainNPC*	pInstance = new CTrainNPC(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTrainNPC"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrainNPC::Clone(void * pArg)
{
	CGameObject*	pInstance = new CTrainNPC(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTrainNPC"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrainNPC::Free()
{
	__super::Free();

	for (int i = 0; i < 14; ++i)
	{
		Safe_Release(m_pModelCom[i]);

	}

}
