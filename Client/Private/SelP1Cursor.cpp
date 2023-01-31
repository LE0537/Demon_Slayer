#include "stdafx.h"
#include "SelP1Cursor.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CSelP1Cursor::CSelP1Cursor(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CSelP1Cursor::CSelP1Cursor(const CSelP1Cursor & rhs)
	: CUI(rhs)
{
}

HRESULT CSelP1Cursor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSelP1Cursor::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	memcpy(&m_ThrowUIinfo, pArg, sizeof(THROWUIINFO));

	m_fSizeX = m_ThrowUIinfo.vScale.x;
	m_fSizeY = m_ThrowUIinfo.vScale.y;
	m_fX = m_ThrowUIinfo.vPos.x;
	m_fY = m_ThrowUIinfo.vPos.y;

	if (m_ThrowUIinfo.iLayerNum == 0)
		m_iImgNum = 0;

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
	pUI_Manager->Set_1PCursor(this);
	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

void CSelP1Cursor::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	Cursor_ImgSel();
	
	if (!m_bSelComplete)
		Move_Cursor();

	if (m_iSelCount < 2)
	{
		if (pGameInstance->Key_Down(DIK_E))
		{
			if (m_iSelCount == 0)
				m_bFirstSelCheck = true;
			else if (m_iSelCount == 1)
			{
				m_bSelectSecond = true;
				m_bSecondSelCheck = true;
			}
			
			++m_iSelCount;

		}
	}
	if (m_iSelCount > 0)
	{
		if (pGameInstance->Key_Down(DIK_Q))
		{
			if (m_iSelCount == 1)
				m_bFirstSelCheck = false;
			else if (m_iSelCount == 2)
			{
				m_bSelectFirst = true;
				m_bSecondSelCheck = false;
			}

			--m_iSelCount;
		}
	}

	if (m_iSelCount == 0)
		m_bSelectFirst = true;
	else if (m_iSelCount == 1)
	{
		m_bSelectFirst = false;
		m_bSelectSecond = true;
	}
	else if (m_iSelCount == 2)
		m_bSelectSecond = false;

	Cursor_To_SelFrame();

	if (m_iSelCount >= 2)
		m_bSelComplete = true;
	else
		m_bSelComplete = false;
		

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (_float)g_iWinSizeX * 0.5f, -m_fY + (_float)g_iWinSizeY * 0.5f, 0.f, 1.f));

	
	RELEASE_INSTANCE(CGameInstance);
}

void CSelP1Cursor::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIPOKE, this);
}

HRESULT CSelP1Cursor::Render()
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

	if (!m_bSelComplete)
		m_pVIBufferCom->Render();

	return S_OK;
}

void CSelP1Cursor::Cursor_To_SelFrame()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	
	if (m_iSelCount == 1 && m_bFirstSelCheck)
	{
		m_SelectInfo = pUI_Manager->Get_SelectFrame(m_iFrameLayerNum)->Get_SelectUIInfo();

		if (m_SelectInfo.bOni)
			m_iSelCount = 3;
	}
	else if (m_iSelCount == 2 && m_bSecondSelCheck)
		m_SelectInfo_2 = pUI_Manager->Get_SelectFrame(m_iFrameLayerNum)->Get_SelectUIInfo();

	RELEASE_INSTANCE(CUI_Manager);
}

void CSelP1Cursor::Move_Cursor()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (pGameInstance->Key_Down(DIK_D))
	{
		if (m_iFrameLayerNum < 5)
			++m_iFrameLayerNum;
		else if (m_iFrameLayerNum == 5)
			m_iFrameLayerNum = 0;
	}
	else if (pGameInstance->Key_Down(DIK_A))
	{
		if (m_iFrameLayerNum > 0)
			--m_iFrameLayerNum;
		else if (m_iFrameLayerNum == 0)
			m_iFrameLayerNum = 5;
	}
	else if (pGameInstance->Key_Down(DIK_W))
	{
		if (m_iFrameLayerNum > 3)
			m_iFrameLayerNum -= 4;
		else if (m_iFrameLayerNum < 2)
			m_iFrameLayerNum += 4;
		else if (m_iFrameLayerNum >= 2 && m_iFrameLayerNum < 4)
			m_iFrameLayerNum = 5;
	}
	else if (pGameInstance->Key_Down(DIK_S))
	{
		if (m_iFrameLayerNum < 2)
			m_iFrameLayerNum += 4;
		else if (m_iFrameLayerNum > 3)
			m_iFrameLayerNum -= 4;
		else if (m_iFrameLayerNum >= 2 && m_iFrameLayerNum < 4)
			m_iFrameLayerNum = 5;
	}

	_float2 vPos = pUI_Manager->Get_SelectFrame(m_iFrameLayerNum)->Get_ThrowInfo().vPos;

	m_fX = vPos.x;
	m_fY = vPos.y - 2.f;

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CSelP1Cursor::Select_Order()
{
}

void CSelP1Cursor::Cursor_ImgSel()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	_float fP2CursorX = pUI_Manager->Get_2PCursor()->Get_fX();
	_float fP2CursorY = pUI_Manager->Get_2PCursor()->Get_fY();

	if (fP2CursorX == m_fX && fP2CursorY == m_fY)
		m_iImgNum = 2;
	else
		m_iImgNum = 0;
	RELEASE_INSTANCE(CUI_Manager);
}

HRESULT CSelP1Cursor::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SelCursor"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSelP1Cursor::SetUp_ShaderResources()
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

CSelP1Cursor * CSelP1Cursor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSelP1Cursor*	pInstance = new CSelP1Cursor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSelP1Cursor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CSelP1Cursor::Clone(void * pArg)
{
	CSelP1Cursor*	pInstance = new CSelP1Cursor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSelP1Cursor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSelP1Cursor::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
