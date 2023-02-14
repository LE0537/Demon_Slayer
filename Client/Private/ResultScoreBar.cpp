#include "stdafx.h"
#include "ResultScoreBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "RankIcon.h"

CResultScoreBar::CResultScoreBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CResultScoreBar::CResultScoreBar(const CResultScoreBar & rhs)
	: CUI(rhs)
{
}

HRESULT CResultScoreBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CResultScoreBar::Initialize(void * pArg)
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
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, -500.f, 100.f)));


	return S_OK;
}

void CResultScoreBar::Tick(_float fTimeDelta)
{
	m_fScoreTime += fTimeDelta;
	if (m_ThrowUIinfo.iLayerNum == 0 && m_fScoreTime >= 2.f)
		m_bScoreSelCheck = true;
	if (m_ThrowUIinfo.iLayerNum == 1 && m_fScoreTime >= 2.3f)
		m_bScoreSelCheck = true;
	if (m_ThrowUIinfo.iLayerNum == 2 && m_fScoreTime >= 2.6f)
		m_bScoreSelCheck = true;
	if (m_ThrowUIinfo.iLayerNum == 3 && m_fScoreTime >= 2.9f)
		m_bScoreSelCheck = true;
	if (m_ThrowUIinfo.iLayerNum == 4 && m_fScoreTime >= 3.2f)
		m_bScoreSelCheck = true;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, -50.f, 1.f));
}

void CResultScoreBar::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CResultScoreBar::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (!m_ThrowUIinfo.bReversal)
		m_pShaderCom->Begin();
	else
		m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	_uint iRand = rand() % 9999;

	if (m_ThrowUIinfo.iLayerNum == 0)
	{
		if(!m_bScoreSelCheck)
			wsprintf(m_szScore, TEXT("최대 콤보       : %d"), iRand);
		else
		{
			if(!m_ThrowUIinfo.bPlyCheck)
				wsprintf(m_szScore, TEXT("최대 콤보       : %d"), pUI_Manager->Get_MaximumCombo(0));
			else
				wsprintf(m_szScore, TEXT("최대 콤보       : %d"), pUI_Manager->Get_MaximumCombo(1));
		}
	}
	if (m_ThrowUIinfo.iLayerNum == 1)
	{
		if (!m_bScoreSelCheck)
			wsprintf(m_szScore, TEXT("남은 시간       : %d"), iRand);
		else
		{
			if (!m_ThrowUIinfo.bPlyCheck)
				wsprintf(m_szScore, TEXT("남은 시간       : %d"), pUI_Manager->Get_RemnantTime(0));
			else
				wsprintf(m_szScore, TEXT("남은 시간       : %d"), pUI_Manager->Get_RemnantTime(1));
		}
	}
	if (m_ThrowUIinfo.iLayerNum == 2)
	{
		if (!m_bScoreSelCheck)
			wsprintf(m_szScore, TEXT("스킬사용횟수: %d"), iRand);
		else
		{
			if (!m_ThrowUIinfo.bPlyCheck)
				wsprintf(m_szScore, TEXT("스킬사용횟수: %d"), pUI_Manager->Get_UseSkillCount(0));
			else
				wsprintf(m_szScore, TEXT("스킬사용횟수: %d"), pUI_Manager->Get_UseSkillCount(1));
		}
	}
	if (m_ThrowUIinfo.iLayerNum == 3)
	{
		if (!m_bScoreSelCheck)
			wsprintf(m_szScore, TEXT("개방사용횟수: %d"), iRand);
		else
		{
			if (!m_ThrowUIinfo.bPlyCheck)
				wsprintf(m_szScore, TEXT("개방사용횟수: %d"), pUI_Manager->Get_UltUseCount(0));
			else
				wsprintf(m_szScore, TEXT("개방사용횟수: %d"), pUI_Manager->Get_UltUseCount(1));
		}
	}
	if (m_ThrowUIinfo.iLayerNum == 4)
	{
		if (!m_bScoreSelCheck)
			wsprintf(m_szScore, TEXT("친구스킬횟수: %d"), iRand);
		else
		{
			if (!m_ThrowUIinfo.bPlyCheck)
				wsprintf(m_szScore, TEXT("친구스킬횟수: %d"), pUI_Manager->Get_FriendUseCount(0));
			else
				wsprintf(m_szScore, TEXT("친구스킬횟수: %d"), pUI_Manager->Get_FriendUseCount(1));
		}
	}

	pGameInstance->Render_Font(TEXT("Font_Nexon"), m_szScore, XMVectorSet(m_fX - 120.f, m_fY - 20.f, 0.f, 1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

HRESULT CResultScoreBar::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ResultScoreBar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CResultScoreBar::SetUp_ShaderResources()
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

CResultScoreBar * CResultScoreBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CResultScoreBar*	pInstance = new CResultScoreBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CResultScoreBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CResultScoreBar::Clone(void * pArg)
{
	CResultScoreBar*	pInstance = new CResultScoreBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CResultScoreBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CResultScoreBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
