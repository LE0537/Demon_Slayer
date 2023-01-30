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
	m_tInfo.iCombo = _tinfo.iCombo;
	m_tInfo.fComboTime = _tinfo.fComboTime;
	m_tInfo.bPowerUp = _tinfo.bPowerUp;
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

