#include "stdafx.h"
#include "QuiestMainBase.h"
#include "GameInstance.h"
#include "UI_Manager.h"


CQuiestMainBase::CQuiestMainBase(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CQuiestMainBase::CQuiestMainBase(const CQuiestMainBase & rhs)
	: CUI(rhs)
{
}

HRESULT CQuiestMainBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQuiestMainBase::Initialize(void * pArg)
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

void CQuiestMainBase::Tick(_float fTimeDelta)
{

	if (g_iLevel == LEVEL_BATTLEENMU)
		m_bDead = true;

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);


	if (pUI_Manager->Get_MsgOnOff())
		m_bQuestOn = true;

	if(pUI_Manager->Get_MainQuestOnOff())
		m_bQuestOn = true;
	else
		m_bQuestOn = false;

	if (m_bQuestOn)
	{
		if (pUI_Manager->Get_QuestStartCheck())
		{
			m_fFadeTime += 0.1f;
			if (m_fFadeTime >= 1.f)
				m_fFadeTime = 1.0f;
		}
		else
		{
			m_fFadeTime -= 0.1f;
			if (m_fFadeTime <= 0.f)
				m_fFadeTime = 0.0f;
		}
	}


	m_iBeforeCount = pUI_Manager->Get_QuestCount();

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_ADVRUI)
	{
		switch (pUI_Manager->Get_QuestCount())
		{
		case 0: m_szMainQuest = TEXT("냄새를 따라가보자");
			break;
		case 1: m_szMainQuest = TEXT("귀살대 대원들을 구출하자");
			break;
		case 2: m_szMainQuest = TEXT("하현의 혈귀를 찾아보자");
			break;
		case 3: m_szMainQuest = TEXT("");
			break;
		case 4: m_szMainQuest = TEXT("");
			break;
		default:
			break;
		}
	}
	else if (m_ThrowUIinfo.iLevelIndex == LEVEL_ADVAKAZA)
	{
		switch (pUI_Manager->Get_QuestCount())
		{
		case 0: m_szMainQuest = TEXT("열차 안을 조사하자");
			break;
		case 1: m_szMainQuest = TEXT("'주'급 귀살 대원을 찾자");
			break;
		case 2: m_szMainQuest = TEXT("열차 앞 혈귀를 처치하라");
			break;
		case 3: m_szMainQuest = TEXT("");
			break;
		case 4: m_szMainQuest = TEXT("");
			break;
		default:
			break;
		}
	}
	

	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CQuiestMainBase::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CQuiestMainBase::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Render_Font(TEXT("Font_Nexon"), m_szMainQuest.c_str(), XMVectorSet(m_fX - 110.f, m_fY - 16.f, 0.f, 1.f), XMVectorSet(m_fFadeTime, m_fFadeTime, m_fFadeTime, m_fFadeTime), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CQuiestMainBase::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestMainBase"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuiestMainBase::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fFadeTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CQuiestMainBase * CQuiestMainBase::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CQuiestMainBase*	pInstance = new CQuiestMainBase(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CQuiestMainBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CQuiestMainBase::Clone(void * pArg)
{
	CQuiestMainBase*	pInstance = new CQuiestMainBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CQuiestMainBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuiestMainBase::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
