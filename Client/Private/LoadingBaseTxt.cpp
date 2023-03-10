#include "stdafx.h"
#include "LoadingBaseTxt.h"
#include "GameInstance.h"
#include "UI_Manager.h"
CLoadingBaseTxt::CLoadingBaseTxt(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CLoadingBaseTxt::CLoadingBaseTxt(const CLoadingBaseTxt & rhs)
	: CUI(rhs)
{
}

HRESULT CLoadingBaseTxt::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingBaseTxt::Initialize(void * pArg)
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

	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
	pUIManager->Get_LoadingList()->push_back(this);
	pUIManager->Set_TipMsgCount(1);

	if (pUIManager->Get_TipMsgCount() >= 4)
		pUIManager->Set_TipMsgCountZero();

	Tip();
	RELEASE_INSTANCE(CUI_Manager);

	return S_OK;
}

void CLoadingBaseTxt::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));
}

void CLoadingBaseTxt::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CLoadingBaseTxt::Render()
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

	pGameInstance->Render_Font(TEXT("Font_Nexon"), m_szTitle.c_str(), XMVectorSet(m_fX, m_fY, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.8f, 0.8f, 0.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLoadingBaseTxt::Tip()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	switch (pUI_Manager->Get_TipMsgCount())
	{
	case 0: m_szTitle = TEXT("조작 캐릭터의 이름 밑에는 2개의 게이지가\n있어. 녹색 바가 체력 게이지, 파란색 바가\n 기술게이지야. 상대의 체력 게이지를 0으로\n 만들면 승리하고, 반대로 조작 캐릭터의\n체력 게이지가 0이 되면 패배해.\n\n기술 게이지는 '기술'을 사용할 때 소비되지만\n시간이 지나면 회복될 거야.");
		break;
	case 1: m_szTitle = TEXT("화면 하단에 있는 것은 오의 개방 게이지야\n\n상대를 공격하거나 반대로 상대의 공격을\n받거나 하면 쌓여. 게이지가 쌓이면 최대\n3개까지 저장돼.\n\n오의 개방 게이지를 소비하면, '오의',\n'개방'을 발동할 수 있어.");
		break;
	case 2:	m_szTitle = TEXT("Tab키를 누르면 퀘스트창을 확인할 수 있어.\nY키를 누르면 혈귀 냄새를 맡을수도 있지.\n\n스토리 모드에서 확인해 봐!!");
		break;
	case 3:	m_szTitle = TEXT("청해, 맥도날드, 윤쉐프, 강촌식당, 량, 장터\n파파이스, 브라운 테이블, 미분당, 보승회관,\n구로돈가, 스시메이진, 은보카츠, 타마,\n한사리, 더진국, 란궁, 소용궁, 롯데리아,\n밥도둑, 동남집, KFC, 명륜진사갈비");
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CUI_Manager);
}

HRESULT CLoadingBaseTxt::Ready_Components()
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingTxt"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingBaseTxt::SetUp_ShaderResources()
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

CLoadingBaseTxt * CLoadingBaseTxt::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLoadingBaseTxt*	pInstance = new CLoadingBaseTxt(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CLoadingBaseTxt"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CLoadingBaseTxt::Clone(void * pArg)
{
	CLoadingBaseTxt*	pInstance = new CLoadingBaseTxt(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CLoadingBaseTxt"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingBaseTxt::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
