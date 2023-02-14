#include "stdafx.h"
#include "ComboNum.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CComboNum::CComboNum(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CComboNum::CComboNum(const CComboNum & rhs)
	: CUI(rhs)
{
}

HRESULT CComboNum::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComboNum::Initialize(void * pArg)
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
		m_pTransformCom->Turn2(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(m_ThrowUIinfo.vRot));

	_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	if (!m_ThrowUIinfo.bReversal)
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	else
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * -1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (m_ThrowUIinfo.iLayerNum == 0 && !m_ThrowUIinfo.bPlyCheck)
		pUI_Manager->Set_ComboUI(this, 0);
	else if (m_ThrowUIinfo.iLayerNum == 0 && m_ThrowUIinfo.bPlyCheck)
		pUI_Manager->Set_ComboUI(this, 1);

	RELEASE_INSTANCE(CUI_Manager);
	return S_OK;
}

void CComboNum::Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (!m_ThrowUIinfo.bPlyCheck)
	{
		_float fTime = pUI_Manager->Get_1P()->Get_PlayerInfo().fComboTime + fTimeDelta;

		pUI_Manager->Get_1P()->Set_ComboTime(fTime);

		_float fComboTime = pUI_Manager->Get_1P()->Get_PlayerInfo().fComboTime;

		m_iComboNum = pUI_Manager->Get_1P()->Get_PlayerInfo().iCombo;

		if(pUI_Manager->Get_MaximumCombo(0) < m_iComboNum)
			pUI_Manager->Set_MaximumCombo(m_iComboNum, 0);

		if (m_ThrowUIinfo.iLayerNum == 0)
			m_iFirstNum = (_uint)m_iComboNum / 10;
		else if (m_ThrowUIinfo.iLayerNum == 1)
			m_iSecondNum = (_uint)m_iComboNum % 10;

		if (fComboTime > 2.f)
		{
			pUI_Manager->Get_1P()->Set_ComboTime(0.f);
			pUI_Manager->Get_1P()->Reset_Combo();
			m_iComboNum = 0;
			m_bRenderOnOff = true;
		}
		else if(fComboTime <= 1.f)
			m_bRenderOnOff = false;
	}
	else
	{
		_float fTime = pUI_Manager->Get_2P()->Get_PlayerInfo().fComboTime + fTimeDelta;

		pUI_Manager->Get_2P()->Set_ComboTime(fTime);

		_float fComboTime = pUI_Manager->Get_2P()->Get_PlayerInfo().fComboTime;

		m_iComboNum = pUI_Manager->Get_2P()->Get_PlayerInfo().iCombo;

		if (pUI_Manager->Get_MaximumCombo(1) < m_iComboNum)
			pUI_Manager->Set_MaximumCombo(m_iComboNum, 1);

		if (m_ThrowUIinfo.iLayerNum == 0)
			m_iFirstNum = (_uint)m_iComboNum / 10;
		else if (m_ThrowUIinfo.iLayerNum == 1)
			m_iSecondNum = (_uint)m_iComboNum % 10;

		if (fComboTime >= 2.f)
		{
			pUI_Manager->Get_2P()->Set_ComboTime(0.f);
			pUI_Manager->Get_2P()->Reset_Combo();
			m_iComboNum = 0;
			m_bRenderOnOff = true;
		}
		else if (fComboTime <= 1.f)
			m_bRenderOnOff = false;
	}
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	RELEASE_INSTANCE(CUI_Manager);
}

void CComboNum::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CComboNum::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	if (m_bRenderOnOff == false)
	{
		if (m_ThrowUIinfo.iLayerNum != 0 && m_iComboNum > 0) // 뒷자리이고 콤보가 0 이상일때 랜더
			m_pVIBufferCom->Render();
		else if (m_ThrowUIinfo.iLayerNum == 0 && m_iImgNum > 0)// 앞자리이고 앞자리가 0 이상일때
			m_pVIBufferCom->Render();
	}
	
	return S_OK;
}

HRESULT CComboNum::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ComboNum"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CComboNum::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (m_ThrowUIinfo.iLayerNum == 0)
		m_iImgNum = m_iFirstNum;
	else if (m_ThrowUIinfo.iLayerNum == 1)
		m_iImgNum = m_iSecondNum;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iImgNum))))
		return E_FAIL;

	return S_OK;
}

CComboNum * CComboNum::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CComboNum*	pInstance = new CComboNum(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CComboNum"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CComboNum::Clone(void * pArg)
{
	CComboNum*	pInstance = new CComboNum(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CComboNum"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CComboNum::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
