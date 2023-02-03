#include "stdafx.h"
#include "CharNameUI.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SelP1Cursor.h"
#include "SelP2Cursor.h"

CCharNameUI::CCharNameUI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CCharNameUI::CCharNameUI(const CCharNameUI & rhs)
	: CUI(rhs)
{
}

HRESULT CCharNameUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharNameUI::Initialize(void * pArg)
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

	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	if (m_ThrowUIinfo.iLayerNum == 0)		
		pUI_Manager->Set_1PChar(this);
	else if(m_ThrowUIinfo.iLayerNum == 1)
		pUI_Manager->Set_2PChar(this);
	else if (m_ThrowUIinfo.iLayerNum == 2)
		pUI_Manager->Set_1P_2Char(this);
	else if (m_ThrowUIinfo.iLayerNum == 3)
		pUI_Manager->Set_2P_2Char(this);

	RELEASE_INSTANCE(CUI_Manager);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (m_ThrowUIinfo.iLayerNum == 0)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuModel"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &m_pModel)))
			return E_FAIL;
		m_pModel->Set_1P(1);
	}
	else if (m_ThrowUIinfo.iLayerNum == 1)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuModel"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &m_pModel)))
			return E_FAIL;
		m_pModel->Set_1P(2);
	}
	else if (m_ThrowUIinfo.iLayerNum == 2)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuModel"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &m_pModel)))
			return E_FAIL;
		m_pModel->Set_1P(3);
	}
	else if (m_ThrowUIinfo.iLayerNum == 3)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MenuModel"), LEVEL_SELECTCHAR, TEXT("Layer_UI"), &m_pModel)))
			return E_FAIL;
		m_pModel->Set_1P(4);
	}
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CCharNameUI::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	if (m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY)
	{
		if (!m_ThrowUIinfo.bPlyCheck)
			Name_Selected(pUI_Manager->Get_1P()->Get_PlayerInfo().strName);
		else if (m_ThrowUIinfo.bPlyCheck)
			Name_Selected(pUI_Manager->Get_2P()->Get_PlayerInfo().strName);
	}
	else if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTCHAR)
	{
		
		if (m_ThrowUIinfo.iLayerNum == 0)
		{
			_float fX = pUI_Manager->Get_Sel1PMain(0)->Get_fX();
			_float fY = pUI_Manager->Get_Sel1PMain(0)->Get_fY();

			m_fX = fX;
			m_fY = fY;
		}
		else if (m_ThrowUIinfo.iLayerNum == 1)
		{
			_float fX = pUI_Manager->Get_Sel2PMain(0)->Get_fX();
			_float fY = pUI_Manager->Get_Sel2PMain(0)->Get_fY();

			m_fX = fX;
			m_fY = fY;
		}
		else if (m_ThrowUIinfo.iLayerNum == 2)
		{
			_float fX = pUI_Manager->Get_Sel1PMain(1)->Get_fX();
			_float fY = pUI_Manager->Get_Sel1PMain(1)->Get_fY();

			m_fX = fX;
			m_fY = fY;
		}
		else if (m_ThrowUIinfo.iLayerNum == 3)
		{
			_float fX = pUI_Manager->Get_Sel2PMain(1)->Get_fX();
			_float fY = pUI_Manager->Get_Sel2PMain(1)->Get_fY();

			m_fX = fX;
			m_fY = fY;
		}

		Set_Name_SelLevel();
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
	RELEASE_INSTANCE(CUI_Manager);
}

void CCharNameUI::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCharNameUI::Render()
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

	if(m_ThrowUIinfo.iLevelIndex == LEVEL_GAMEPLAY)
		m_pVIBufferCom->Render();
	else if (m_ThrowUIinfo.iLevelIndex == LEVEL_SELECTCHAR)
	{
		CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

		CSelP1Cursor* pSelP1Cursor = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor());
		CSelP2Cursor* pSelP2Cursor = dynamic_cast<CSelP2Cursor*>(pUI_Manager->Get_2PCursor());

		if(m_ThrowUIinfo.iLayerNum == 0)
			m_pVIBufferCom->Render();
		else if(m_ThrowUIinfo.iLayerNum == 1)
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 2)
		{
			if (pSelP1Cursor != nullptr)
			{
				if (pSelP1Cursor->Get_FirstSelCheck() && !pSelP1Cursor->Get_SecondSelCheck() && !pSelP1Cursor->Get_SelectUIInfo().bOni)
					m_pVIBufferCom->Render();
				if (pSelP1Cursor->Get_SecondSelCheck() && !pSelP1Cursor->Get_SelectUIInfo().bOni)
					m_pVIBufferCom->Render();
			}
		}
		else if (m_ThrowUIinfo.iLayerNum == 3)
		{
			if (pSelP2Cursor != nullptr)
			{
				if (pSelP2Cursor->Get_FirstSelCheck() && !pSelP2Cursor->Get_SecondSelCheck() && !pSelP2Cursor->Get_SelectUIInfo().bOni)
					m_pVIBufferCom->Render();
				if (pSelP2Cursor->Get_SecondSelCheck() && !pSelP2Cursor->Get_SelectUIInfo().bOni)
					m_pVIBufferCom->Render();
			}
		}

		RELEASE_INSTANCE(CUI_Manager);
	}
	return S_OK;
}

void CCharNameUI::Name_Selected(wstring strName)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (strName == TEXT("쿄주로"))
	{
		m_iImgNum = 1;
		m_fSizeX = m_ThrowUIinfo.vScale.x * 2.f;
		if (!m_ThrowUIinfo.bPlyCheck)//1p	
			m_fX = 215.f;
		else if (m_ThrowUIinfo.bPlyCheck)//2p
			m_fX = 1065.f;	
	}
	else if (strName == TEXT("탄지로"))
	{
		m_iImgNum = 0;
		m_fSizeX = m_ThrowUIinfo.vScale.x * 2.f;
		if (!m_ThrowUIinfo.bPlyCheck)
			m_fX = 220;
		else if (m_ThrowUIinfo.bPlyCheck)
			m_fX = 1065.f;
	}
	else if (strName == TEXT("루이"))
	{
		m_iImgNum = 2;
		m_fSizeX = m_ThrowUIinfo.vScale.x * 0.65f;
	}
	else if (strName == TEXT("아카자"))
	{
		m_iImgNum = 3;
		m_fSizeX = m_ThrowUIinfo.vScale.x;
	/*	if (!m_ThrowUIinfo.bPlyCheck)
			m_fX = 185;
		else if (m_ThrowUIinfo.bPlyCheck)
			m_fX = 1195.f;*/
	}
	else if (strName == TEXT("네즈코"))
	{
		m_iImgNum = 4;
		m_fSizeX = m_ThrowUIinfo.vScale.x * 2.f;
		if (!m_ThrowUIinfo.bPlyCheck)//1p
			m_fX = 220;
		else if (m_ThrowUIinfo.bPlyCheck)//2p
			m_fX = 1065.f;
		
	}
	else if (strName == TEXT("시노부"))
	{
		m_iImgNum = 5;
		m_fSizeX = m_ThrowUIinfo.vScale.x * 2.f;
		if (!m_ThrowUIinfo.bPlyCheck)//1p	
			m_fX = 215.f;
		else if (m_ThrowUIinfo.bPlyCheck)//2p
			m_fX = 1065.f;
	}
	RELEASE_INSTANCE(CUI_Manager);
}

void CCharNameUI::Set_Name_SelLevel()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	CSelP1Cursor* pSelP1Cursor = dynamic_cast<CSelP1Cursor*>(pUI_Manager->Get_1PCursor());
	CSelP2Cursor* pSelP2Cursor = dynamic_cast<CSelP2Cursor*>(pUI_Manager->Get_2PCursor());

	_uint iSelNum1PCursor = pSelP1Cursor->Get_FrameLayerNum();
	_uint iSelNum2PCursor = pSelP2Cursor->Get_FrameLayerNum();

	if (m_ThrowUIinfo.iLayerNum == 0)
	{
		if (!pSelP1Cursor->Get_FirstSelCheck())
		{
			m_iImgNum = iSelNum1PCursor;
			m_pModel->Set_ModelIndex(m_iImgNum);
			m_pModel->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(250.f - g_iWinSizeX * 0.5f, -800.f + g_iWinSizeY * 0.5f, -330.f, 1.f));
		}
		else if(!(m_iImgNum == 2 || m_iImgNum == 3))
		{
			m_pModel->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(120.f - g_iWinSizeX * 0.5f, -800.f + g_iWinSizeY * 0.5f, -200.f, 1.f));
		}

	}
	else if (m_ThrowUIinfo.iLayerNum == 1)
	{
		if (!pSelP2Cursor->Get_FirstSelCheck())
		{
			m_iImgNum = iSelNum2PCursor;
			m_pModel->Set_ModelIndex(m_iImgNum);
			m_pModel->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1030.f - g_iWinSizeX * 0.5f, -800.f + g_iWinSizeY * 0.5f, -330.f, 1.f));
		}
		else if(!(m_iImgNum == 2 || m_iImgNum == 3))
		{
			m_pModel->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1160.f - g_iWinSizeX * 0.5f, -800.f + g_iWinSizeY * 0.5f, -200.f, 1.f));
		}
	}
	else if (m_ThrowUIinfo.iLayerNum == 2)
	{
		if (!pSelP1Cursor->Get_SecondSelCheck())
		{
			m_iImgNum = iSelNum1PCursor;
			m_pModel->Set_ModelIndex(m_iImgNum);
			m_pModel->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(250.f - g_iWinSizeX * 0.5f, -800.f + g_iWinSizeY * 0.5f, -330.f, 1.f));
		}
		else
		{
			m_pModel->Set_ModelIndex(99);
		}
	
	}
	else if (m_ThrowUIinfo.iLayerNum == 3)
	{
		if (!pSelP2Cursor->Get_SecondSelCheck())
		{
			m_iImgNum = iSelNum2PCursor;
			m_pModel->Set_ModelIndex(m_iImgNum);
			m_pModel->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1030.f - g_iWinSizeX * 0.5f, -800.f + g_iWinSizeY * 0.5f, -330.f, 1.f));
		}
		else
		{
			m_pModel->Set_ModelIndex(99);
		}
	}
	
	Select_NameReSize();

	RELEASE_INSTANCE(CUI_Manager);
}

_float CCharNameUI::Select_NameReSize()
{
	if (m_iImgNum == 0)
		return m_fSizeX = m_ThrowUIinfo.vScale.x * 1.9f;
	else if (m_iImgNum == 1)
		return m_fSizeX = m_ThrowUIinfo.vScale.x * 1.9f;
	else if (m_iImgNum == 2)
		return m_fSizeX = m_ThrowUIinfo.vScale.x * 0.7f;
	else if (m_iImgNum == 3)
		return m_fSizeX = m_ThrowUIinfo.vScale.x;
	else if (m_iImgNum == 4)
		return m_fSizeX = m_ThrowUIinfo.vScale.x * 1.9f;
	else if (m_iImgNum == 5)
		return m_fSizeX = m_ThrowUIinfo.vScale.x * 1.9f;
	else
		return 0.f;
}

HRESULT CCharNameUI::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CharNameUI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharNameUI::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;

	return S_OK;
}

CCharNameUI * CCharNameUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCharNameUI*	pInstance = new CCharNameUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCharNameUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCharNameUI::Clone(void * pArg)
{
	CCharNameUI*	pInstance = new CCharNameUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCharNameUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharNameUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
