#include "stdafx.h"
#include "CharSelMsg.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SelP1Cursor.h"
#include "SelP2Cursor.h"

CCharSelMsg::CCharSelMsg(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CCharSelMsg::CCharSelMsg(const CCharSelMsg & rhs)
	: CUI(rhs)
{
}

HRESULT CCharSelMsg::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharSelMsg::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));

	if (m_ThrowUIinfo.vRot >= 0 && m_ThrowUIinfo.vRot <= 360)
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, m_ThrowUIinfo.vRot));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));


	return S_OK;
}

void CCharSelMsg::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
}

void CCharSelMsg::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCharSelMsg::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	SELUIINFO Select1PInfo = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor())->Get_SelectUIInfo();
	SELUIINFO Select1PInfo_2 = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor())->Get_SelectUIInfoSecond();
	SELUIINFO Select2PInfo = dynamic_cast<CSelP2Cursor*>(pUI_Manager->Get_2PCursor())->Get_SelectUIInfo();
	SELUIINFO Select2PInfo_2 = dynamic_cast<CSelP2Cursor*>(pUI_Manager->Get_2PCursor())->Get_SelectUIInfoSecond();
	CSelP1Cursor* pP1Cursor = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor());
	CSelP2Cursor* pP2Cursor = dynamic_cast<CSelP2Cursor*>(pUI_Manager->Get_2PCursor());

	if(!Select1PInfo.bOni && Select1PInfo_2.bOni && pP1Cursor->Get_FirstSelCheck())
	{
		if (m_ThrowUIinfo.iLayerNum == 0)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 1)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 2)
		{
			m_pVIBufferCom->Render();
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("이 캐릭터와는 협력할 수 없습니다."), XMVectorSet(m_fX - 140.f, m_fY - 10.f, 0.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 1.f), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		}
	}
	else if (Select1PInfo.strName == Select1PInfo_2.strName && pP1Cursor->Get_FirstSelCheck())
	{
		if (m_ThrowUIinfo.iLayerNum == 0)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 1)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 2)
		{
			m_pVIBufferCom->Render();
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("같은 캐릭터는 선택할 수 없습니다."), XMVectorSet(m_fX - 145.f, m_fY - 10.f, 0.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 1.f), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		}
	}

	if (!Select2PInfo.bOni && Select2PInfo_2.bOni && pP2Cursor->Get_FirstSelCheck())
	{
		if (m_ThrowUIinfo.iLayerNum == 3)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 4)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 5)
		{
			m_pVIBufferCom->Render();
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("이 캐릭터와는 협력할 수 없습니다."), XMVectorSet(m_fX - 140.f, m_fY - 10.f, 0.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 1.f), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		}		
	}
	else if (Select2PInfo.strName == Select2PInfo_2.strName && pP2Cursor->Get_FirstSelCheck())
	{
		if (m_ThrowUIinfo.iLayerNum == 3)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 4)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 5)
		{
			m_pVIBufferCom->Render();
			pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("같은 캐릭터는 선택할 수 없습니다."), XMVectorSet(m_fX - 145.f, m_fY - 10.f, 0.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 1.f), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
		}
	}

	if (m_ThrowUIinfo.iLayerNum >= 6)
	{
		m_pVIBufferCom->Render();
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("같은 캐릭터는 선택할 수 없습니다."), XMVectorSet(m_fX - 145.f, m_fY - 10.f, 0.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 1.f), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));
	}

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CCharSelMsg::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UIVtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharSelMsg"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharSelMsg::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CCharSelMsg * CCharSelMsg::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCharSelMsg*	pInstance = new CCharSelMsg(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRankEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCharSelMsg::Clone(void * pArg)
{
	CCharSelMsg*	pInstance = new CCharSelMsg(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRankEff"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharSelMsg::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
