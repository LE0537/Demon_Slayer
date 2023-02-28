#include "stdafx.h"
#include "MapNameBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SoundMgr.h"

CMapNameBar::CMapNameBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CMapNameBar::CMapNameBar(const CMapNameBar & rhs)
	: CUI(rhs)
{
}

HRESULT CMapNameBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapNameBar::Initialize(void * pArg)
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

void CMapNameBar::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (!pUI_Manager->Get_SaveStory())
	{
		if (m_fFadeTime <= 0.f)
			m_bFadeCheck = true;
		else if (m_fFadeTime >= 1.f)
		{
			m_fStopTime += fTimeDelta;
			if (m_fStopTime >= 1.f)
				m_bFadeCheck = false;
		}

		if (m_bFadeCheck && !m_bOnCheck)
		{
			m_iMoveCount += 1;
			m_fFadeTime += 0.01f;
			if (m_iMoveCount >= 80)
			{
				m_iMoveCount = 80;
				m_fFadeTime = 1.f;
			}
		}
		else
		{
			m_iMoveCount -= 1;
			m_fFadeTime -= 0.01f;
			if (m_iMoveCount <= 0)
			{
				m_iMoveCount = 0;
				m_fFadeTime = 0.f;
				m_bOnCheck = true;
				if (!m_bMsgOnCheck)
				{
					if (m_ThrowUIinfo.iLevelIndex == LEVEL_ADVRUI)
					{
						pUI_Manager->Set_MsgOn();
						pUI_Manager->Set_MsgName(TEXT("카마도 탄지로"));
						pUI_Manager->Set_Msg(TEXT("(킁킁.. 이..냄새는... 뭐지..?)"));
						CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Dialog_04.wav"), fEFFECT);
						pUI_Manager->Set_QuestStartCheck(true);
						pUI_Manager->Set_MainQuestOn();
						m_bMsgOnCheck = true;
					
					}
					else if(m_ThrowUIinfo.iLevelIndex == LEVEL_ADVAKAZA)
					{
						switch (pUI_Manager->Get_MsgCount())
						{
						case 0:
							pUI_Manager->Set_MsgOn();
							pUI_Manager->Set_MsgName(TEXT("카마도 탄지로"));
							pUI_Manager->Set_Msg(TEXT("큭.. 냄새가 지독해, 무거워...! 이 바람 속에서 혈귀 냄새가 이렇게까지...!!"));
							break;
						case 1:
							pUI_Manager->Set_MsgOn();
							pUI_Manager->Set_MsgName(TEXT("카마도 탄지로"));
							pUI_Manager->Set_Msg(TEXT("혈귀는 바람이 불어오는 쪽... 선두 차량인가? 앞으로 가보자"));
							pUI_Manager->Set_QuestStartCheck(true);
							pUI_Manager->Set_MainQuestOn();
							pUI_Manager->Reset_MsgCount();
							m_bMsgOnCheck = true;
							break;
						default:
							break;
						}
					}

					
				}
			}
		}
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CMapNameBar::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CMapNameBar::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();

	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if(m_ThrowUIinfo.iLevelIndex == LEVEL_ADVRUI)
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("나타구모 산 초입"), XMVectorSet(m_fX - 115.f, m_fY - 23.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, m_fFadeTime), XMVectorSet(0.9f, 0.9f, 0.f, 1.f));	
	else if(m_ThrowUIinfo.iLevelIndex == LEVEL_ADVAKAZA)
		pGameInstance->Render_Font(TEXT("Font_Nexon"), TEXT("무한열차 후문"), XMVectorSet(m_fX - 100.f, m_fY - 23.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, m_fFadeTime), XMVectorSet(0.9f, 0.9f, 0.f, 1.f));
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMapNameBar::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MapNameBar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapNameBar::SetUp_ShaderResources()
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

CMapNameBar * CMapNameBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMapNameBar*	pInstance = new CMapNameBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMapNameBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMapNameBar::Clone(void * pArg)
{
	CMapNameBar*	pInstance = new CMapNameBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMapNameBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapNameBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
