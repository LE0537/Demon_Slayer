#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "GameObj.h"

CCamera_Dynamic::CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(&((CAMERADESC_DERIVED*)pArg)->CameraDesc)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc = m_pTransform->Get_TransformDesc();
	if (FAILED(__super::Add_Components(TEXT("Com_SubTransform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pSubTransform, &TransformDesc)))
		return E_FAIL;

	m_pSubTransform->Set_WorldMatrix(m_pTransform->Get_WorldMatrix());

	return S_OK;
}

void CCamera_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Set_CamPos();

	if (m_pPlayer->Get_PlayerInfo().bSub)
		m_pPlayer = m_pPlayer->Get_SubChar();
	if (m_pTarget->Get_PlayerInfo().bSub)
		m_pTarget = m_pTarget->Get_SubChar();

	Move_CamPos(fTimeDelta);
	//
	if (FAILED(Bind_OnPipeLine()))
		return;
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{

	__super::Late_Tick(fTimeDelta);

	if (!m_bBattle)
	{
		Set_BattleTarget();
		m_bBattle = true;
	}

}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Dynamic::Set_CamPos()
{
	if (nullptr == m_pPlayer ||
		nullptr == m_pTarget)
	{
		return;
	}
	_vector vPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTarget = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook2 = vPos - vTarget;

	_float fDist = XMVectorGetX(XMVector3Length(vLook2));

	//맵의 임시 반지름
	_float fDiameter = 85.f;
	m_fCamDist = fDist / fDiameter;
	if (m_fCamDist > 1.f)
		m_fCamDist = 1.f;
	else if (m_fCamDist < 0.33f)
		m_fCamDist = 0.33f;
	vPos -= XMVector3Normalize(vLook2) * (fDist * 0.5f);

	_vector vRight = XMVector3Normalize(vPos - vTarget);

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransform->Set_Rotation(_float3(0.f, m_fAngle, 0.f));

	_vector vLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_float fTime = 1.f;
	vPos -= vLook * (fTime + m_fLookY) * (fDiameter * 0.5f) * m_fCamDist;
	vPos.m128_f32[1] = 0.f;
	vPos.m128_f32[1] += 5.5f;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	
}

void CCamera_Dynamic::Move_CamPos(_float fTimeDelta)
{
	ConvertToViewPort(fTimeDelta);

	if (m_f1pX < 200.f)
	{
		while (true)
		{
			if (m_f1pX >= 200.f)
				break;
			m_fAngle += 0.1f;
			Set_CamPos();
		
			ConvertToViewPort(fTimeDelta);
		}
	}
	else if (m_f1pX > 1100.f)
	{
		while (true)
		{
			if (m_f1pX <= 1100.f)
				break;
			m_fAngle -= 0.1f;
			Set_CamPos();
		
			ConvertToViewPort(fTimeDelta);
		}
	}
	_vector v1PY = m_p1P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_bool	bTrue = m_b1P;
	if (CheckSubChar())
	{
		if (!m_p1P->Get_PlayerInfo().bJump && v1PY.m128_f32[1] < 0.1f)
		{
			if (m_f1pY > 630.f)
			{
				while (true)
				{
					if (m_f1pY <= 630.f)
						break;
					if (bTrue != m_b1P)
						break;
					m_fLookY += fTimeDelta / 7.f;
					if (m_fLookY > 85.f)
						m_fLookY = 85.f;
					Set_CamPos();
				
					ConvertToViewPort(fTimeDelta);
				}
			}
			else if (m_f1pY < 580.f)
			{
				while (true)
				{
					if (m_f1pY >= 580.f)
						break;
					if(bTrue != m_b1P)
						break;
					m_fLookY -= fTimeDelta / 7.f;
					if (m_fLookY < 0.f)
						m_fLookY = 0.f;
					Set_CamPos();
					
					ConvertToViewPort(fTimeDelta);
				}
			}
		}
	}
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	if (CheckSubChar())
		vPos.m128_f32[1] += v1PY.m128_f32[1] / 2.f;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pPlayer->Set_CamAngle(m_fAngle);
	m_pTarget->Set_CamAngle(m_fAngle);
}

void CCamera_Dynamic::Lerp_SubCam(_float fTimeDelta)
{
	_float	fWeight = 1.f;
	_matrix matWorld = XMMatrixIdentity();

	_vector vLerp = XMVector3Normalize(XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_LOOK), m_pSubTransform->Get_State(CTransform::STATE_LOOK), fTimeDelta * fWeight));
	matWorld.r[CTransform::STATE_LOOK] = vLerp;

	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pTransform->Get_State(CTransform::STATE_LOOK)));
	_vector vUp = XMVector3Cross(m_pTransform->Get_State(CTransform::STATE_LOOK), m_pTransform->Get_State(CTransform::STATE_RIGHT));
	matWorld.r[CTransform::STATE_RIGHT] = vRight;
	matWorld.r[CTransform::STATE_UP] = vUp;

	_vector vLerp_Pos = XMVector3Normalize(XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_pSubTransform->Get_State(CTransform::STATE_TRANSLATION), fTimeDelta * fWeight));
	matWorld.r[CTransform::STATE_TRANSLATION] = vLerp_Pos;

	m_pTransform->Set_WorldMatrix(matWorld);
}


void CCamera_Dynamic::ConvertToViewPort(_float fTimeDelta)
{
	_vector vPlayerPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPlayerLook = vPos - vPlayerPos;
	_vector vTargetLook = vPos - vTargetPos;

	

	_vector vAtPos = XMVectorLerp(vPlayerPos, vTargetPos, 0.5f);
	vAtPos.m128_f32[1] = 3.f;
	m_pTransform->LookAt(vAtPos);

	_float fPlayerDist = XMVectorGetX(XMVector3Length(vPlayerLook));
	_float fTargetDist = XMVectorGetX(XMVector3Length(vTargetLook));
	/////////////////////////////////////////////////////////////////////////////위에가 소트
	_matrix matPlayerWorld = m_pPlayer->Get_Transform()->Get_WorldMatrix();
	_matrix matTargetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();
	_matrix matView = XMMatrixInverse(nullptr, m_pTransform->Get_WorldMatrix());
	_matrix matProj = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	_matrix matWVP = matPlayerWorld * matView * matProj;
	_matrix matWVP2 = matTargetWorld * matView * matProj;
	_float  m_fScalingbyDepth = XMVectorGetW(matWVP.r[3]);
	matWVP.r[3] /= m_fScalingbyDepth;
	_float  m_fScalingbyDepth2 = XMVectorGetW(matWVP2.r[3]);
	matWVP2.r[3] /= m_fScalingbyDepth2;


	XMStoreFloat4(&m_vPlayerPos, matWVP.r[3]);
	
	m_vPlayerPos.x += 1.f;
	m_vPlayerPos.x *= g_iWinSizeX / 2.f;

	m_vPlayerPos.y += 1.f;
	m_vPlayerPos.y *= g_iWinSizeY / 2.f;
	m_vPlayerPos.y = 720.f - m_vPlayerPos.y;


	XMStoreFloat4(&m_vTargetPos, matWVP2.r[3]);

	m_vTargetPos.x += 1.f;
	m_vTargetPos.x *= g_iWinSizeX / 2.f;

	m_vTargetPos.y += 1.f;
	m_vTargetPos.y *= g_iWinSizeY / 2.f;
	m_vTargetPos.y = 720.f - m_vTargetPos.y;


	if (fTargetDist >= fPlayerDist)
	{
		m_f1pX = m_vPlayerPos.x;
		m_f1pY = m_vPlayerPos.y;
		m_f2pX = m_vTargetPos.x;
		m_f2pY = m_vTargetPos.y;

		switch (m_pPlayer->Get_i1P())
		{
		case 1:
			m_pPlayer->Set_iTargetIndex(1);
			m_pTarget->Set_iTargetIndex(2);
			break;
		case 2:
			m_pTarget->Set_iTargetIndex(1);
			m_pPlayer->Set_iTargetIndex(2);
			break;
		default:
			break;
		}
		m_p1P = m_pPlayer;
		m_b1P = true;
	}
	else
	{
		m_f1pX = m_vTargetPos.x;
		m_f1pY = m_vTargetPos.y;
		m_f2pX = m_vPlayerPos.x;
		m_f2pY = m_vPlayerPos.y;

		switch (m_pPlayer->Get_i1P())
		{
		case 1:
			m_pTarget->Set_iTargetIndex(1);
			m_pPlayer->Set_iTargetIndex(2);
			break;
		case 2:
			m_pPlayer->Set_iTargetIndex(1);
			m_pTarget->Set_iTargetIndex(2);
			break;
		default:
			break;
		}
		m_p1P = m_pTarget;
		m_b1P = false;
	}

}



void CCamera_Dynamic::Set_BattleTarget()
{
	m_pPlayer->Set_BattleTarget(m_pTarget);
	m_pTarget->Set_BattleTarget(m_pPlayer);
}

_bool CCamera_Dynamic::CheckSubChar()
{
	_bool bPlayerSub = false;
	_bool bTargetSub = false;

	if (m_pPlayer->Get_SubChar() == nullptr)
		bPlayerSub = true;

	if (m_pTarget->Get_SubChar() == nullptr)
		bTargetSub = true;

	if (bPlayerSub && !bTargetSub)
	{
		if (!m_pPlayer->Get_PlayerInfo().bChange && !m_pTarget->Get_PlayerInfo().bChange &&
			!m_pTarget->Get_SubChar()->Get_PlayerInfo().bChange)
			return true;
	}
	else if(!bPlayerSub && bTargetSub)
	{
		if (!m_pPlayer->Get_PlayerInfo().bChange && !m_pTarget->Get_PlayerInfo().bChange &&
			!m_pPlayer->Get_SubChar()->Get_PlayerInfo().bChange)
			return true;
	}
	else if (bPlayerSub && bTargetSub)
	{
		if (!m_pPlayer->Get_PlayerInfo().bChange && !m_pTarget->Get_PlayerInfo().bChange)
			return true;
	}
	else if (!bPlayerSub && !bTargetSub)
	{
		if (!m_pPlayer->Get_PlayerInfo().bChange && !m_pTarget->Get_PlayerInfo().bChange &&
			!m_pPlayer->Get_SubChar()->Get_PlayerInfo().bChange &&
			!m_pTarget->Get_SubChar()->Get_PlayerInfo().bChange)
			return true;
	}
	return true;
}


CCamera_Dynamic * CCamera_Dynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();

	Safe_Release(m_pSubTransform);
}
