#include "..\Public\Transform.h"
#include "Navigation.h"
#include "PipeLine.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{

}

void CTransform::Set_Scale(_fvector vScaleInfo)
{
	Set_State(CTransform::STATE_RIGHT,
		XMVector3Normalize(Get_State(CTransform::STATE_RIGHT)) * XMVectorGetX(vScaleInfo));
	Set_State(CTransform::STATE_UP,
		XMVector3Normalize(Get_State(CTransform::STATE_UP)) * XMVectorGetY(vScaleInfo));
	Set_State(CTransform::STATE_LOOK,
		XMVector3Normalize(Get_State(CTransform::STATE_LOOK)) * XMVectorGetZ(vScaleInfo));
}

void CTransform::Set_Scale(STATE eState, _fvector vScaleInfo)
{
	Set_State(eState, XMVector3Normalize(Get_State(eState)) * XMVectorGetX(vScaleInfo));
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

void CTransform::Go_StraightNoNavi(_float fTimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vPrePosition = vPosition;

	_float3		vSliding = _float3(0.f, 0.f, 0.f);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
		return;
	}

	_bool	bMoveCheck = pNavigation->isMove(vPosition, vLook, &vSliding);

	if (true == bMoveCheck)
	{
		_float fHeight = 0.f;
		_vector vecTemp = Get_State(CTransform::STATE_TRANSLATION);

		if (m_bJump == false)
		{
			pNavigation->Navigation_Height(vPosition);
			fHeight = pNavigation->Get_NavigationHeight().y;
			vecTemp = XMVectorSetY(vPosition, fHeight);
			Set_State(CTransform::STATE_TRANSLATION, vecTemp);
		}
		else
			Set_State(CTransform::STATE_TRANSLATION, vPosition);
		
	}
	else
	{
		
		vPrePosition += XMLoadFloat3(&vSliding) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
		if (true == pNavigation->isMove(vPrePosition, vLook, &vSliding))
		{
			_float fHeight = 0.f;
			_vector vecTemp = Get_State(CTransform::STATE_TRANSLATION);

			if (m_bJump == false)
			{
				pNavigation->Navigation_Height(vPosition);
				fHeight = pNavigation->Get_NavigationHeight().y;
				vecTemp = XMVectorSetY(vPosition, fHeight);
				Set_State(CTransform::STATE_TRANSLATION, vecTemp);
			}
			else
				Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
	}
}

void CTransform::Go_Backward(_float fTimeDelta, class CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vPrePosition = vPosition;

	_float3		vSliding = _float3(0.f, 0.f, 0.f);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
		return;
	}

	_bool	bMoveCheck = pNavigation->isMove(vPosition, vLook, &vSliding);

	if (true == bMoveCheck)
	{
		_float fHeight = 0.f;
		_vector vecTemp = Get_State(CTransform::STATE_TRANSLATION);

		if (m_bJump == false)
		{
			pNavigation->Navigation_Height(vPosition);
			fHeight = pNavigation->Get_NavigationHeight().y;
			vecTemp = XMVectorSetY(vPosition, fHeight);
			Set_State(CTransform::STATE_TRANSLATION, vecTemp);
		}
		else
			Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
	{
		vPrePosition += XMLoadFloat3(&vSliding) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
		if (true == pNavigation->isMove(vPrePosition, vLook, &vSliding))
		{
			_float fHeight = 0.f;
			_vector vecTemp = Get_State(CTransform::STATE_TRANSLATION);

			if (m_bJump == false)
			{
				pNavigation->Navigation_Height(vPosition);
				fHeight = pNavigation->Get_NavigationHeight().y;
				vecTemp = XMVectorSetY(vPosition, fHeight);
				Set_State(CTransform::STATE_TRANSLATION, vecTemp);
			}
			else
				Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
	}
}

void CTransform::Go_Left(_float fTimeDelta, class CNavigation* pNavigation)
{

	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vPrePosition = vPosition;

	_float3		vSliding = _float3(0.f, 0.f, 0.f);

	vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
		return;
	}

	_bool	bMoveCheck = pNavigation->isMove(vPosition, vRight, &vSliding);

	if (true == bMoveCheck)
	{
		_float fHeight = 0.f;
		_vector vecTemp = Get_State(CTransform::STATE_TRANSLATION);

		if (m_bJump == false)
		{
			pNavigation->Navigation_Height(vPosition);
			fHeight = pNavigation->Get_NavigationHeight().y;
			vecTemp = XMVectorSetY(vPosition, fHeight);
			Set_State(CTransform::STATE_TRANSLATION, vecTemp);
		}
		else
			Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
	{
		vPrePosition += XMLoadFloat3(&vSliding) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
		if (true == pNavigation->isMove(vPrePosition, vRight, &vSliding))
		{
			_float fHeight = 0.f;
			_vector vecTemp = Get_State(CTransform::STATE_TRANSLATION);

			if (m_bJump == false)
			{
				pNavigation->Navigation_Height(vPosition);
				fHeight = pNavigation->Get_NavigationHeight().y;
				vecTemp = XMVectorSetY(vPosition, fHeight);
				Set_State(CTransform::STATE_TRANSLATION, vecTemp);
			}
			else
				Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
	}
}

void CTransform::Go_Right(_float fTimeDelta, class CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vPrePosition = vPosition;

	_float3		vSliding = _float3(0.f, 0.f, 0.f);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
		return;
	}

	_bool	bMoveCheck = pNavigation->isMove(vPosition, vRight, &vSliding);

	if (true == bMoveCheck)
	{
		_float fHeight = 0.f;
		_vector vecTemp = Get_State(CTransform::STATE_TRANSLATION);

		if (m_bJump == false)
		{
			pNavigation->Navigation_Height(vPosition);
			fHeight = pNavigation->Get_NavigationHeight().y;
			vecTemp = XMVectorSetY(vPosition, fHeight);
			Set_State(CTransform::STATE_TRANSLATION, vecTemp);
		}
		else
			Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
	{
		vPrePosition += XMLoadFloat3(&vSliding) * m_TransformDesc.fSpeedPerSec * fTimeDelta;
		if (true == pNavigation->isMove(vPrePosition, vRight, &vSliding))
		{
			_float fHeight = 0.f;
			_vector vecTemp = Get_State(CTransform::STATE_TRANSLATION);

			if (m_bJump == false)
			{
				pNavigation->Navigation_Height(vPosition);
				fHeight = pNavigation->Get_NavigationHeight().y;
				vecTemp = XMVectorSetY(vPosition, fHeight);
				Set_State(CTransform::STATE_TRANSLATION, vecTemp);
			}
			else
				Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
	}
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(Get_State(CTransform::STATE_RIGHT), RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(Get_State(CTransform::STATE_UP), RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(Get_State(CTransform::STATE_LOOK), RotationMatrix));
}

void CTransform::Turn2(_fvector vAxis, _float fAngle)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fAngle);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(Get_State(CTransform::STATE_RIGHT), RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(Get_State(CTransform::STATE_UP), RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(Get_State(CTransform::STATE_LOOK), RotationMatrix));
}

void CTransform::LookAt(_fvector vAt)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = vAt - vPosition;
	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector		vRight = XMVector3Cross(vAxisY, vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scale();

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::RotationAll(_float3 vAxis)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), vAxis.x) *
		XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), vAxis.y) *
		XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), vAxis.z);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scale().x, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scale().y, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scale().z, RotationMatrix));
}

void CTransform::Set_Rotation(_float3 fAngle)
{
	// Get current RotationMatrix from the WorldMatrix by decomposition.
	_vector vScale, vRotationQuat, vTranslation;
	XMMatrixDecompose(&vScale, &vRotationQuat, &vTranslation, Get_WorldMatrix());
	_matrix RotationMatrix = XMMatrixRotationQuaternion(vRotationQuat);

	// Multiply the WorldMatrix by the Inverse of current RotationMatrix (to get a WorldMatrix without any Rotation)
	_matrix InverseRotationMatrix = XMMatrixInverse(nullptr, RotationMatrix);
	_matrix WorldMatrixWithoutRotation = XMMatrixMultiply(Get_WorldMatrix(), InverseRotationMatrix);

	// Make a NewRotationMatrix with new angle values
	_matrix NewRotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fAngle.x), XMConvertToRadians(fAngle.y), XMConvertToRadians(fAngle.z));
	m_fCurrentRotationX = fAngle.x;
	m_fCurrentRotationY = fAngle.y;
	m_fCurrentRotationZ = fAngle.z;

	// Set NewRotationMatrix to WorldMatrixWithoutRotation
	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(WorldMatrixWithoutRotation.r[0], NewRotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(WorldMatrixWithoutRotation.r[1], NewRotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(WorldMatrixWithoutRotation.r[2], NewRotationMatrix));
}

void CTransform::Set_RotationY(_float fAngleY)
{
	Set_Rotation(_float3(m_fCurrentRotationX, fAngleY, m_fCurrentRotationZ));
}

void CTransform::Set_PlayerLookAt(_fvector vAt)
{
	//
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = vAt - vPosition;

	vLook = XMVectorSetY(vLook, 0.f);

	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector		vRight = XMVector3Cross(vAxisY, vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	_float3		vScale = Get_Scale();

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
