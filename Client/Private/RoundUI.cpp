#include "stdafx.h"
#include "RoundUI.h"
#include "GameInstance.h"
#include  "UI_Manager.h"
#include "NumTimer.h"
#include "UI_Manager.h"
#include "RoundIcon.h"
#include "Layer.h"

CRoundUI::CRoundUI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CRoundUI::CRoundUI(const CRoundUI & rhs)
	: CUI(rhs)
{
}

HRESULT CRoundUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRoundUI::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));
	
	m_ThrowUIinfo.vScale.x = m_ThrowUIinfo.vScale.x * 0.7f;
	m_ThrowUIinfo.vScale.y =m_ThrowUIinfo.vScale.y * 0.7f; 
	m_fSizeX = m_ThrowUIinfo.vScale.x * 0.7f;
	m_fSizeY = m_ThrowUIinfo.vScale.y * 0.7f;
	//m_fSizeX = 800.f;
	//m_fSizeY = 450.f;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	pUI_Manager->Set_RoundUI(this);
	RELEASE_INSTANCE(CUI_Manager);


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));

	return S_OK;
}

void CRoundUI::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (pGameInstance->Key_Down(DIK_M))
	{
		_int iHp = pUI_Manager->Get_1P()->Get_PlayerInfo().iHp;

		pUI_Manager->Get_1P()->Set_Hp(-iHp);
	}
	
	Battle_End(fTimeDelta);
	ZeroTiemr_RoundUI(fTimeDelta);
	ZeroHp_RoundUI(fTimeDelta);

	UI_Function(fTimeDelta);

	if (m_bResultOn)
	{
		if (m_b1PRoundEnd)
		{
			m_ResultInfo.iFirstTarget = pUI_Manager->Get_Sel1P();
			m_ResultInfo.iSecondTarget = pUI_Manager->Get_Sel1P_2();
			m_ResultInfo.bPlayerWin = false;
		}
		else if (m_b2PRoundEnd)
		{
			m_ResultInfo.iFirstTarget = pUI_Manager->Get_Sel1P();
			m_ResultInfo.iSecondTarget = pUI_Manager->Get_Sel1P_2();
			m_ResultInfo.bPlayerWin = true;
		}
		pUI_Manager->Set_Result_Info(m_ResultInfo);
		pUI_Manager->Set_LevelResultOn(true);
		m_bResultOn = false;
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CUI_Manager);
}

void CRoundUI::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CRoundUI::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	if(m_bRoundOnOff)
		m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CRoundUI::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundUI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundJudgmentUI"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;	

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundFightUI"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_RoundWinUI"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRoundUI::SetUp_ShaderResources()
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

	if (m_bRoundCheck)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
			return E_FAIL;
	}
	else if (m_bJudgmentCheck)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_iJudgmentImgNum))))
			return E_FAIL;
	}
	else if (m_bWinCheck)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(m_iWinImgNum))))
			return E_FAIL;
	}
	else if (m_bStartCheck)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_iStartEndImgNum))))
			return E_FAIL;
	}

	return S_OK;
}

void CRoundUI::ZoomIn(_float fTimeDelta)
{

	m_fZoomTime += fTimeDelta * 1.5f;

	if (!m_bScaleCheck)
	{
		if (m_iStartEndImgNum == 0)
		{
			m_fSizeX = m_ThrowUIinfo.vScale.x * 0.7f;
			m_fSizeY = m_ThrowUIinfo.vScale.y * 0.9f;
		}
		else
		{
			m_fSizeX = m_ThrowUIinfo.vScale.x * 0.7f;
			m_fSizeY = m_ThrowUIinfo.vScale.y * 0.7f;
		}

		m_bScaleCheck = true;
	}
	
	if (m_fZoomTime >= 0.001f)
	{
		m_fSizeX += m_fSizeX * 0.01f;
		m_fSizeY += m_fSizeY * 0.01f;
		Fade_In(fTimeDelta);
		m_fZoomTime = 0.f;
	}
	if (m_fSizeX >= m_ThrowUIinfo.vScale.x && m_fSizeY >= m_ThrowUIinfo.vScale.y)
	{
		m_bZoomCheck = true;
		m_fZoomTime = 0.f;
	}
	
}

void CRoundUI::ZoomOut(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	m_fStopTime += fTimeDelta;

	if (m_fStopTime >= 1.f)
	{

		m_fZoomTime += fTimeDelta * 1.5f;
		if (m_fZoomTime >= 0.001f)
		{
			m_fSizeX -= m_fSizeX * 0.01f;
			m_fSizeY -= m_fSizeY * 0.01f;
			Fade_Out(fTimeDelta);
			m_fZoomTime = 0.f;
		}
		if (m_fSizeX <= m_ThrowUIinfo.vScale.x * 0.7f && m_fSizeY <= m_ThrowUIinfo.vScale.y * 0.7f)
		{
			m_bRoundOnOff = false;
			m_bZoomCheck = false;
			m_fStopTime = 0.f;	
			m_fZoomTime = 0.f;
			Case_ReturnEnd();
		}
	}

	RELEASE_INSTANCE(CUI_Manager);
}

void CRoundUI::Fade_In(_float fTimeDelta)
{
	m_fFadeTime += fTimeDelta * 1.8f;
	if (m_fFadeTime >= 1.f)
		m_fFadeTime = 1.f;
}

void CRoundUI::Fade_Out(_float fTimeDelta)
{
	m_fFadeTime -= fTimeDelta * 1.8f;
	if (m_fFadeTime <= 0.f)
		m_fFadeTime = 0.f;
}

void CRoundUI::Case_ReturnEnd()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bRoundCheck)
	{
		m_bRoundCheck = false;
		m_bRoundOnOff = true;
		m_bStartCheck = true;
	}
	else if (m_bJudgmentCheck)
	{
		_int i1PMaxHp = pUI_Manager->Get_1P()->Get_PlayerInfo().iMaxHp;
		_int i1PHp = pUI_Manager->Get_1P()->Get_PlayerInfo().iHp;

		_int i2PMaxHp = pUI_Manager->Get_2P()->Get_PlayerInfo().iMaxHp;
		_int i2PHp = pUI_Manager->Get_2P()->Get_PlayerInfo().iHp;

		auto pTarget = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_RoundIconUI"));
		if (i1PHp > i2PHp)
		{
			++m_i1PRoundWin;
			for (auto iter : pTarget->Get_ObjectList())
			{
				_bool bCheck = dynamic_cast<CRoundIcon*>(iter)->Get_1PRoundCheck();
				if (!bCheck)
				{
					dynamic_cast<CRoundIcon*>(iter)->Set_1PRoundCheck(true);
					break;
				}
			}
		}
		else if (i1PHp < i2PHp)
		{
			++m_i2PRoundWin;
			for (auto iter : pTarget->Get_ObjectList())
			{
				_bool bCheck = dynamic_cast<CRoundIcon*>(iter)->Get_2PRoundCheck();
				if (!bCheck)
				{
					dynamic_cast<CRoundIcon*>(iter)->Set_2PRoundCheck(true);
					break;
				}
			}
		}
		else if (i1PHp == i2PHp)
		{
			for (auto iter : pTarget->Get_ObjectList())
			{
				_bool bCheck = dynamic_cast<CRoundIcon*>(iter)->Get_2PRoundCheck();
				if (!bCheck)
				{
					dynamic_cast<CRoundIcon*>(iter)->Set_2PRoundCheck(true);
					break;
				}
			}

			for (auto iter : pTarget->Get_ObjectList())
			{
				_bool bCheck = dynamic_cast<CRoundIcon*>(iter)->Get_1PRoundCheck();
				if (!bCheck)
				{
					dynamic_cast<CRoundIcon*>(iter)->Set_1PRoundCheck(true);
					break;
				}
			}
		}

		if (!m_b1PRoundEnd && !m_b2PRoundEnd)
			++m_iImgNum;

		if (m_i1PRoundWin >= 3)
		{
			m_iImgNum = 0;
			m_iStartEndImgNum = 1;
			m_bStartCheck = true;
		}
		else if (m_i2PRoundWin >= 3)
		{
			m_iImgNum = 0;
			m_iStartEndImgNum = 1;
			m_bStartCheck = true;
		}
		else 
			m_bRoundCheck = true;


		pUI_Manager->Get_1P()->Set_Hp(i1PMaxHp - i1PHp);
		pUI_Manager->Get_2P()->Set_Hp(i2PMaxHp - i2PHp);

		m_bJudgmentCheck = false;
		m_bRoundOnOff = true;
	}
	else if (m_bStartCheck)
	{
		m_bStartCheck = false;

		if (m_iStartEndImgNum == 1)
		{
			m_bRoundOnOff = true;
			m_bWinCheck = true;
			if (m_b1PRoundEnd)
				m_iWinImgNum = 0;
			else if (m_b2PRoundEnd)
				m_iWinImgNum = 1;

			m_i1PRoundWin = 0;
			m_i2PRoundWin = 0;
		}
	
	}
	else if (m_bWinCheck)
	{
		m_bWinCheck = false;
		m_bResultOn = true;
	}
		
	m_bScaleCheck = false;
	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CRoundUI::UI_Function(_float fTimeDelta)
{
	if (m_bRoundOnOff)
	{
		m_fDelayTime += fTimeDelta;
		if (m_fDelayTime >= 5.f)
		{
			if (!m_bZoomCheck)
				ZoomIn(fTimeDelta);
			else if (m_bZoomCheck)
				ZoomOut(fTimeDelta);
		}
	}
	
}

void CRoundUI::ZeroTiemr_RoundUI(_float fTImeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	_float fTimer = dynamic_cast<CNumTimer*>(pUI_Manager->Get_TimerUI())->Get_Time();

	if (fTimer <= 0)
	{
		m_iJudgmentImgNum = 1;
		m_bJudgmentCheck = true;
		m_bRoundOnOff = true;
	}

	RELEASE_INSTANCE(CUI_Manager);
}

void CRoundUI::ZeroHp_RoundUI(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (pUI_Manager->Get_1P()->Get_PlayerInfo().iHp <= 0.f)
	{
		m_iJudgmentImgNum = 0;
		m_bJudgmentCheck = true;
		m_bRoundOnOff = true;
	}
	else if (pUI_Manager->Get_2P()->Get_PlayerInfo().iHp <= 0.f)
	{
		m_iJudgmentImgNum = 0;
		m_bJudgmentCheck = true;
		m_bRoundOnOff = true;
	}

	RELEASE_INSTANCE(CUI_Manager);
}

void CRoundUI::Battle_End(_float fTimeDelta)
{
	if (m_b1PRoundEnd)
	{
		m_iJudgmentImgNum = 0;
		m_bRoundOnOff = true;
		if (!m_bJudgmentCheck && m_iStartEndImgNum == 0)
			m_bJudgmentCheck = true;
	}
}

CRoundUI * CRoundUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRoundUI*	pInstance = new CRoundUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRoundUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CRoundUI::Clone(void * pArg)
{
	CRoundUI*	pInstance = new CRoundUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRoundUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRoundUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	Safe_Release(m_pTextureCom4);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
