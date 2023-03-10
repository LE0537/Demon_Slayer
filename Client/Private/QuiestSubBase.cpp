#include "stdafx.h"
#include "QuiestSubBase.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SoundMgr.h"

CQuiestSubBase::CQuiestSubBase(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CQuiestSubBase::CQuiestSubBase(const CQuiestSubBase & rhs)
	: CUI(rhs)
{
}

HRESULT CQuiestSubBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CQuiestSubBase::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x - 20.f;
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

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	pUI_Manager->Set_SubBase(this, m_ThrowUIinfo.iLayerNum);

	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

void CQuiestSubBase::Tick(_float fTimeDelta)
{

	if (g_iLevel == LEVEL_BATTLEENMU)
		m_bDead = true;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (pGameInstance->Key_Down(DIK_TAB))
	{
		if (!m_bBaseOn)
		{
			m_bBaseOn = true;
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("UI_QuestOn.wav"), g_fEffect);
		}
		else
		{
			m_bBaseOn = false;
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("UI_QuestOff.wav"), g_fEffect);
		}
	}

	if (m_bBaseOn)
	{
		m_fX += 4.f;
		m_fFadeTime += 0.2f;
		if (m_fX >= m_ThrowUIinfo.vPos.x && m_fFadeTime >= 1.f)
		{
			m_fX = m_ThrowUIinfo.vPos.x;
			m_fFadeTime = 1.f;
		}
	}
	else
	{
		m_fX -= 4.f;
		m_fFadeTime -= 0.2f;
		if (m_fX <= m_ThrowUIinfo.vPos.x - 10.f && m_fFadeTime <= 0.f)
		{
			m_fX = m_ThrowUIinfo.vPos.x - 10.f;
			m_fFadeTime = 0.f;
		}
	}


	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);
}

void CQuiestSubBase::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CQuiestSubBase::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_ThrowUIinfo.iLevelIndex == LEVEL_ADVRUI)
	{
		if (m_ThrowUIinfo.iLayerNum == 0)
			m_szQuest = TEXT("Ç÷±Í ³¿»õ µû¶ó°¡±â");
		else if (m_ThrowUIinfo.iLayerNum == 1)
			wsprintf(m_szQuest2, TEXT("±Í»ì´ë¿ø ±¸Ãâ(%d/2)"), pUI_Manager->Get_RescueCount());
		else if (m_ThrowUIinfo.iLayerNum == 2)
			m_szQuest = TEXT("ÇÏÇö Ç÷±Í Ã³Ä¡");
		else if (m_ThrowUIinfo.iLayerNum == 3)
			m_szQuest = TEXT("");
		else if (m_ThrowUIinfo.iLayerNum == 4)
			m_szQuest = TEXT("");
	}
	else
	{
		if (m_ThrowUIinfo.iLayerNum == 0)
			m_szQuest = TEXT("¿­Â÷ ¾ÈÀ» Á¶»çÇÏ±â");
		else if (m_ThrowUIinfo.iLayerNum == 1)
			m_szQuest = TEXT("'¿°ÁÖ' ±Í»ì´ë¿ø Ã£±â");
		else if (m_ThrowUIinfo.iLayerNum == 2)
			m_szQuest = TEXT("ÇÏÇö1 Ç÷±Í Ã³Ä¡");
		else if (m_ThrowUIinfo.iLayerNum == 3)
			m_szQuest = TEXT("");
		else if (m_ThrowUIinfo.iLayerNum == 4)
			m_szQuest = TEXT("");
	}
	


	if (pUI_Manager->Get_QuestCount() >= m_ThrowUIinfo.iLayerNum)
	{
		m_pVIBufferCom->Render();
		if(m_ThrowUIinfo.iLayerNum != 1)
			pGameInstance->Render_Font(TEXT("Font_Nexon"), m_szQuest.c_str(), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFadeTime, m_fFadeTime, m_fFadeTime, m_fFadeTime), XMVectorSet(0.9f, 0.9f, 0.f, 1.f));
		else
			pGameInstance->Render_Font(TEXT("Font_Nexon"), m_szQuest2, XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(m_fFadeTime, m_fFadeTime, m_fFadeTime, m_fFadeTime), XMVectorSet(0.9f, 0.9f, 0.f, 1.f));
	}

	

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

HRESULT CQuiestSubBase::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuiestSubBase"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuiestSubBase::SetUp_ShaderResources()
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

CQuiestSubBase * CQuiestSubBase::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CQuiestSubBase*	pInstance = new CQuiestSubBase(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CQuiestSubBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CQuiestSubBase::Clone(void * pArg)
{
	CQuiestSubBase*	pInstance = new CQuiestSubBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CQuiestSubBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuiestSubBase::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
