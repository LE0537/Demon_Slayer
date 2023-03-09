#include "stdafx.h"
#include "..\Public\Characters.h"
#include "Navigation.h"
#include "GameInstance.h"

CCharacters::CCharacters(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CCharacters::CCharacters(const CCharacters & rhs)
	: CGameObj(rhs)
{
}

HRESULT CCharacters::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacters::Initialize(void * pArg)
{
	ZeroMemory(&m_tInfo, sizeof(PLAYERINFO));

	return S_OK;
}

void CCharacters::Tick(_float fTimeDelta)
{
}

void CCharacters::Late_Tick(_float fTimeDelta)
{
}

HRESULT CCharacters::Render()
{
	return S_OK;
}

HRESULT CCharacters::Render_ShadowDepth()
{
	return S_OK;
}

_float4 CCharacters::Get_NavigationHeight()
{
	return m_pNavigationCom->Get_NavigationHeight();
}

void CCharacters::Set_NavigationHeight(_fvector vPosition)
{
	m_pNavigationCom->Navigation_Height(vPosition);
}

void CCharacters::Set_PlayerOriginPosY(_float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	fVelocity += fGravity * fTimeDelta;

	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	vecPos = XMVectorSetX(vecPos, vPosition.x);
	vecPos = XMVectorSetY(vecPos, vPosition.y);
	vecPos = XMVectorSetZ(vecPos, vPosition.z);
	m_pNavigationCom->Navigation_Height(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	if (vPosition.y <= m_pNavigationCom->Get_NavigationHeight().y)// m_fCurrentPosY)
	{
		vPosition.y = m_pNavigationCom->Get_NavigationHeight().y;
		fVelocity = m_pNavigationCom->Get_NavigationHeight().y;
		
		_vector vecPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

		vecPos = XMVectorSetY(vecPos, vPosition.y);
		if (m_pNavigationCom->Cheak_Cell(vecPos))
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		
	}
	else
	{
		if (m_pNavigationCom->Cheak_Cell(vecPos))
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}


}

//CCharacters * CCharacters::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
//{
//	CCharacters*	pInstance = new CCharacters(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		ERR_MSG(TEXT("Failed to Created : CCharacters"));
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}


void CCharacters::Set_Change(_bool _bChange, _vector _vPos)
{
	m_bChange = _bChange;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, _vPos);
}

void CCharacters::Change_Info(PLAYERINFO _tinfo)
{
	m_tInfo.iHp = _tinfo.iHp;
	m_tInfo.iSkBar = _tinfo.iSkBar;
	m_tInfo.iUnicBar = _tinfo.iUnicBar;
	m_tInfo.iUnicCount = _tinfo.iUnicCount;
	m_tInfo.iCombo = _tinfo.iCombo;
	m_tInfo.fComboTime = _tinfo.fComboTime;
	m_tInfo.fPowerUp = _tinfo.fPowerUp;
	m_tInfo.fPowerUpTime = _tinfo.fPowerUpTime;
	m_tInfo.iFriendBar = _tinfo.iFriendBar;
}

CGameObject * CCharacters::Clone(void * pArg)
{
	//CGameObject*	pInstance = new CCharacters(*this);

	//if (FAILED(pInstance->Initialize(pArg)))
	//{
	//	ERR_MSG(TEXT("Failed to Cloned : CCharacters"));
	//	Safe_Release(pInstance);
	//}

	//return pInstance;
	return nullptr;
}

void CCharacters::Free()
{
	__super::Free();


}

HRESULT CCharacters::Add_EffectLight(LIGHTDESC::TYPE eType, _float4 vPosition, _float4 vDiffuse, _float4 vAmbient, _float fRange, _float fLifeTime)
{
	if (eType < LIGHTDESC::TYPE_EFF1 ||
		eType > LIGHTDESC::TYPE_EFF3)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LIGHTDESC tLightDesc;
	ZeroMemory(&tLightDesc, sizeof tLightDesc);
	tLightDesc.eType = eType;
	tLightDesc.vPosition = vPosition;
	tLightDesc.vDiffuse = vDiffuse;
	tLightDesc.vAmbient = vAmbient;
	tLightDesc.fRange = fRange;
	tLightDesc.fLifeTime = fLifeTime;
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, tLightDesc)))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CCharacters::Add_Light(_float4 vPosition, _float4 vDiffuse, _float4 vAmbient, _float fRange)
{
	if (3 <= m_vecCreateLights.size())
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LIGHTDESC tLightDesc;
	ZeroMemory(&tLightDesc, sizeof tLightDesc);
	tLightDesc.eType = LIGHTDESC::TYPE((_int)LIGHTDESC::TYPE_EFFPOINT1 + (_int)m_vecCreateLights.size());
	tLightDesc.vPosition = vPosition;
	tLightDesc.vDiffuse = vDiffuse;
	tLightDesc.vAmbient = vAmbient;
	tLightDesc.fRange = fRange;
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, tLightDesc)))
		return E_FAIL;
	m_vecCreateLights.push_back(tLightDesc.eType);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CCharacters::Delete_MyLights(LIGHTDESC::TYPE eType)
{
	if (eType != LIGHTDESC::TYPE_EFFPOINT1 &&
		eType != LIGHTDESC::TYPE_EFFPOINT2 &&
		eType != LIGHTDESC::TYPE_EFFPOINT3 &&
		eType != LIGHTDESC::TYPE_END)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (eType == LIGHTDESC::TYPE_END)
	{
		for (auto& iter = m_vecCreateLights.begin();
			iter != m_vecCreateLights.end();)
		{
			pGameInstance->Delete_Light(*iter);
			iter = m_vecCreateLights.erase(iter);			
		}
	}
	else
		pGameInstance->Delete_Light(eType);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
