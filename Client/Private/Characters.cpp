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

