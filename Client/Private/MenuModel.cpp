#include "stdafx.h"
#include "..\Public\MenuModel.h"

#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "SoundMgr.h"
#include "VIBuffer_Navigation.h"
#include "Data_Manager.h"	
#include "UI_Manager.h"
#include "TanjiroWeapon.h"
#include "TanjiroSheath.h"
#include "KyoujuroWeapon.h"
#include "KyoujuroSheath.h"
#include "ShinobuWeapon.h"
#include "ShinobuSheath.h"
#include "KyoujuroWeaponMenu.h"
#include "ShinobuWeaponMenu.h"

CMenuModel::CMenuModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacters(pDevice, pContext)
{
}

CMenuModel::CMenuModel(const CMenuModel & rhs)
	: CCharacters(rhs)
{
}

HRESULT CMenuModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMenuModel::Initialize(void * pArg)
{
	*(CCharacters**)pArg = this;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_TanjiroParts()))
		return E_FAIL;
	if (FAILED(Ready_TanjiroParts2()))
		return E_FAIL;
	if (FAILED(Ready_KyoujuroParts()))
		return E_FAIL;
	if (FAILED(Ready_KyoujuroParts2()))
		return E_FAIL;
	if (FAILED(Ready_ShinobuParts()))
		return E_FAIL;
	if (FAILED(Ready_ShinobuParts2()))
		return E_FAIL;
	_vector		vLook = { 0.3f,0.f,-1.f,0.f };

	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector		vRight = XMVector3Cross(vAxisY, vLook);

	_vector		vUp = XMVector3Cross(vLook, vRight);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, -500.f, 100.f)));
	

	m_iModelIndex = 0;
	m_iAnimIndex = 1;
	m_pModelCom[m_iModelIndex]->Set_CurrentAnimIndex(m_iAnimIndex);
	m_pModelCom[m_iModelIndex]->Set_Loop(m_iAnimIndex);
	m_pModelCom[m_iModelIndex]->Set_LinearTime(m_iAnimIndex, 0.01f);
	m_fSizeX = 500.f;
	m_fSizeY = 500.f;
	_float fX = 350.f;
	_float fY = 350.f;
	_float3 vScale2 = { fX,fY,350.f };
	_float3 vScale = { m_fSizeX,m_fSizeY,500.f };
	dynamic_cast<CTanjiroWeapon*>(m_pWeapon[0])->Get_Transform()->Set_Scale(XMLoadFloat3(&vScale2));
	dynamic_cast<CTanjiroWeapon*>(m_pWeapon[0])->Get_Transform()->Set_Rotation(_float3(0.f, 95.f, 90.f));
	//	dynamic_cast<CTanjiroWeapon*>(m_pWeapon[0])->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));
	dynamic_cast<CTanjiroSheath*>(m_pSheath[0])->Get_Transform()->Set_Scale(XMLoadFloat3(&vScale2));
	dynamic_cast<CTanjiroSheath*>(m_pSheath[0])->Get_Transform()->Set_Rotation(_float3(0.f, 90.f, 90.f));
	//	dynamic_cast<CTanjiroSheath*>(m_pSheath[0])->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));

	dynamic_cast<CKyoujuroWeaponMenu*>(m_pWeapon[1])->Get_Transform()->Set_Scale(XMLoadFloat3(&vScale2));
	dynamic_cast<CKyoujuroWeaponMenu*>(m_pWeapon[1])->Get_Transform()->Set_Rotation(_float3(0.f, 95.f, 90.f));
	//	dynamic_cast<CKyoujuroWeapon*>(m_pWeapon[1])->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));
	dynamic_cast<CKyoujuroSheath*>(m_pSheath[1])->Get_Transform()->Set_Scale(XMLoadFloat3(&vScale2));
	dynamic_cast<CKyoujuroSheath*>(m_pSheath[1])->Get_Transform()->Set_Rotation(_float3(0.f, 90.f, 90.f));
	//	dynamic_cast<CKyoujuroSheath*>(m_pSheath[1])->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));

	dynamic_cast<CShinobuWeaponMenu*>(m_pWeapon[2])->Get_Transform()->Set_Scale(XMLoadFloat3(&vScale2));
	dynamic_cast<CShinobuWeaponMenu*>(m_pWeapon[2])->Get_Transform()->Set_Rotation(_float3(0.f, 95.f, 90.f));
	//	dynamic_cast<CShinobuWeapon*>(m_pWeapon[2])->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));
	dynamic_cast<CShinobuSheath*>(m_pSheath[2])->Get_Transform()->Set_Scale(XMLoadFloat3(&vScale2));
	dynamic_cast<CShinobuSheath*>(m_pSheath[2])->Get_Transform()->Set_Rotation(_float3(0.f, 90.f, 90.f));
	//	dynamic_cast<CShinobuSheath*>(m_pSheath[2])->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));


	return S_OK;
}

void CMenuModel::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bPos)
	{
		Set_Pos();
		m_bPos = true;
	}
	if (m_iModelIndex != 99)
	{
		if (m_iAnimIndex == 1 && m_pModelCom[m_iModelIndex]->Get_End(1))
		{
			m_pModelCom[m_iModelIndex]->Set_End(1);
			m_pModelCom[m_iModelIndex]->Set_CurrentAnimIndex(0);
		}

		m_iCurrentModelIndex = m_iModelIndex;

	}
	
}

void CMenuModel::Late_Tick(_float fTimeDelta)
{
	if (m_iModelIndex != 99)
	{
		if (m_iCurrentModelIndex != m_iModelIndex)
		{
		//	m_pModelCom[m_iModelIndex]->Reset_Anim(1);
			m_pModelCom[m_iModelIndex]->Set_Loop(1);
			m_pModelCom[m_iModelIndex]->Set_LinearTime(1.f, 0.01f);
			m_iCurrentModelIndex = m_iModelIndex;
			m_pModelCom[m_iModelIndex]->Set_CurrentAnimIndex(1);
		}
		m_pModelCom[m_iModelIndex]->Play_Animation(fTimeDelta);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		switch (m_iModelIndex)
		{
		case 0:
			dynamic_cast<CTanjiroWeapon*>(m_pWeapon[0])->Set_Menu(true);
			dynamic_cast<CTanjiroSheath*>(m_pSheath[0])->Set_Menu(true);
			m_pWeapon[0]->Tick(fTimeDelta);
			m_pSheath[0]->Tick(fTimeDelta);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, m_pWeapon[0]);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, m_pSheath[0]);
			break;
		case 1:
			dynamic_cast<CKyoujuroWeaponMenu*>(m_pWeapon[1])->Set_Menu(true);
			dynamic_cast<CKyoujuroSheath*>(m_pSheath[1])->Set_Menu(true);
			m_pWeapon[1]->Tick(fTimeDelta);
			m_pSheath[1]->Tick(fTimeDelta);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, m_pWeapon[1]);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, m_pSheath[1]);
			break;
		case 5:
			dynamic_cast<CShinobuWeaponMenu*>(m_pWeapon[2])->Set_Menu(true);
			dynamic_cast<CShinobuSheath*>(m_pSheath[2])->Set_Menu(true);
			m_pWeapon[2]->Tick(fTimeDelta);
			m_pSheath[2]->Tick(fTimeDelta);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, m_pWeapon[2]);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, m_pSheath[2]);
			break;
		default:
			break;
		}

	}
}

HRESULT CMenuModel::Render()
{
	if (m_iModelIndex != 99)
	{
		if (nullptr == m_pShaderCom)
			return E_FAIL;

		if (FAILED(SetUp_ShaderResources()))
			return E_FAIL;

		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		_uint		iNumMeshes = m_pModelCom[m_iModelIndex]->Get_NumMeshContainers();

		_int iDiffuse = 0;
		_int iMask = 1;
		if (m_bBlack)
		{
			iDiffuse = 4;
			iMask = 3;
		}
		for (_uint i = 0; i < iNumMeshes; ++i)
		{
		
			if (FAILED(m_pModelCom[m_iModelIndex]->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
		
			if ((i == 0 || i == 1) && (m_iModelIndex == 0 || m_iModelIndex == 1))
			{
				if (FAILED(m_pModelCom[m_iModelIndex]->SetUp_Material(m_pShaderCom, "g_MaskTexture", i, aiTextureType_NORMALS)))
					return E_FAIL;

				if (FAILED(m_pModelCom[m_iModelIndex]->Render(m_pShaderCom, i, iMask)))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pModelCom[m_iModelIndex]->Render(m_pShaderCom, i, iDiffuse)))
					return E_FAIL;
			}
		}

		RELEASE_INSTANCE(CGameInstance);

	}
	return S_OK;
}
HRESULT CMenuModel::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom)
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



	_uint		iNumMeshes = m_pModelCom[m_iModelIndex]->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom[m_iModelIndex]->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom[m_iModelIndex]->Render(m_pShaderCom, i, 1)))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}
HRESULT CMenuModel::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxUIModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_TanjiroModel"), LEVEL_STATIC, TEXT("Tanjiro(Menu)"), (CComponent**)&m_pModelCom[0])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_KyoujuroModel"), LEVEL_STATIC, TEXT("Kyoujuro(Menu)"), (CComponent**)&m_pModelCom[1])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_RuiModel"), LEVEL_STATIC, TEXT("Rui(Menu)"), (CComponent**)&m_pModelCom[2])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_AkazaModel"), LEVEL_STATIC, TEXT("Akaza(Menu)"), (CComponent**)&m_pModelCom[3])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_NezukoModel"), LEVEL_STATIC, TEXT("Nezuko(Menu)"), (CComponent**)&m_pModelCom[4])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_ShinobuModel"), LEVEL_STATIC, TEXT("Shinobu(Menu)"), (CComponent**)&m_pModelCom[5])))
		return E_FAIL;



	return S_OK;
}

void CMenuModel::Set_Pos()
{
	_float3 vScale;
	switch (m_i1p)
	{
	case 1:
		m_iModelIndex = 0;
		m_fSizeX = 150.f;
		m_fSizeY = 150.f;
		m_fX = 200.f;
		m_fY = 800.f;
		vScale = { m_fSizeX,m_fSizeY,1.f };
		m_pTransformCom->Set_Scale(XMLoadFloat3(&vScale));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));
		break;
	case 2:
		m_iModelIndex = 3;
		m_fSizeX = 150.f;
		m_fSizeY = 150.f;
		m_fX = 1080.f;
		m_fY = 800.f;
		vScale = { m_fSizeX,m_fSizeY,1.f };
		m_pTransformCom->Set_Scale(XMLoadFloat3(&vScale));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));
		break;
	case 3:
		m_iModelIndex = 99;
		m_fSizeX = 150.f;
		m_fSizeY = 150.f;
		m_fX = 200.f;
		m_fY = 800.f;
		vScale = { m_fSizeX,m_fSizeY,1.f };
		m_pTransformCom->Set_Scale(XMLoadFloat3(&vScale));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));
		break;
	case 4:
		m_iModelIndex = 99;
		m_fSizeX = 150.f;
		m_fSizeY = 150.f;
		m_fX = 1080.f;
		m_fY = 800.f;
		vScale = { m_fSizeX,m_fSizeY,1.f };
		m_pTransformCom->Set_Scale(XMLoadFloat3(&vScale));
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -300.f, 1.f));
		break;
	default:
		break;
	}

}

HRESULT CMenuModel::Ready_TanjiroParts()
{
	CHierarchyNode*		pSocket = m_pModelCom[0]->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CTanjiroWeapon::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom[0]->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeapon[0] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_TanjiroWeapon"), &WeaponDesc);
	if (nullptr == m_pWeapon)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMenuModel::Ready_TanjiroParts2()
{
	CHierarchyNode*		pSocket = m_pModelCom[0]->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CTanjiroSheath::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom[0]->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pSheath[0] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_TanjiroSheath"), &WeaponDesc);
	if (nullptr == m_pSheath)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMenuModel::Ready_KyoujuroParts()
{
	CHierarchyNode*		pSocket = m_pModelCom[1]->Get_BonePtr("L_Weapon_1");
	
	if (nullptr == pSocket)
		return E_FAIL;

	CKyoujuroWeaponMenu::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom[1]->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeapon[1] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_KyoujuroWeaponMenu"), &WeaponDesc);
	if (nullptr == m_pWeapon)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMenuModel::Ready_KyoujuroParts2()
{
	CHierarchyNode*		pSocket = m_pModelCom[1]->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CKyoujuroSheath::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom[1]->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pSheath[1] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_KyoujuroSheath"), &WeaponDesc);
	if (nullptr == m_pSheath)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMenuModel::Ready_ShinobuParts()
{
	CHierarchyNode*		pSocket = m_pModelCom[5]->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CShinobuWeaponMenu::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom[5]->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pWeapon[2] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_ShinobuWeaponMenu"), &WeaponDesc);
	if (nullptr == m_pWeapon)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMenuModel::Ready_ShinobuParts2()
{
	CHierarchyNode*		pSocket = m_pModelCom[5]->Get_BonePtr("L_Weapon_1");
	if (nullptr == pSocket)
		return E_FAIL;

	CShinobuSheath::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom[5]->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pSheath[2] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_ShinobuSheath"), &WeaponDesc);
	if (nullptr == m_pSheath)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



void CMenuModel::Take_Damage(_float _fPow, _bool _bJumpHit)
{
}

void CMenuModel::Get_GuardHit(_int eType)
{
}

void CMenuModel::Player_TakeDown(_float _fPow, _bool _bJump)
{
}

void CMenuModel::Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower)
{
}

HRESULT CMenuModel::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMenuModel * CMenuModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMenuModel*	pInstance = new CMenuModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMenuModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMenuModel::Clone(void * pArg)
{
	CGameObject*	pInstance = new CMenuModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMenuModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMenuModel::Free()
{
	__super::Free();

	for (int i = 0; i < 6; ++i)
	{
		Safe_Release(m_pModelCom[i]);

	}
	for (int i = 0; i < 3; ++i)
	{
		Safe_Release(m_pWeapon[i]);
		Safe_Release(m_pSheath[i]);
	}
}
