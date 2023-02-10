#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "GameObj.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"
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
	m_fStartTime = 0.f;
	m_fFov = 14.f;
	m_fLookAtY = 4.f;
	m_fAngle = 45.f;

	m_FovAngle = XMConvertToRadians(60.f);
	//m_bStory = true;
	return S_OK;
}

void CCamera_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	static _bool	bCamAttach = true;
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Key_Down(DIK_F10))
		bCamAttach = !bCamAttach;

	if (false == bCamAttach)
	{
		_float fSpeed = 1.f;
		if (pGameInstance->Key_Pressing(DIK_RSHIFT))
			fSpeed *= 10.f;

		if (pGameInstance->Key_Pressing(DIK_UP))
			m_pTransform->Go_Straight(fTimeDelta * fSpeed);
		if (pGameInstance->Key_Pressing(DIK_DOWN))
			m_pTransform->Go_Backward(fTimeDelta * fSpeed);
		if (pGameInstance->Key_Pressing(DIK_LEFT))
			m_pTransform->Go_Left(fTimeDelta * fSpeed);
		if (pGameInstance->Key_Pressing(DIK_RIGHT))
			m_pTransform->Go_Right(fTimeDelta * fSpeed);

		if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
		{
			_long			MouseMove = 0;

			if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
				m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);

			if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
				m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
		}
	}
	if(!m_bStory)
	{
		m_fStartTime += fTimeDelta;

	
#ifdef _DEBUG
		if (!m_bLerp && m_fStartTime > 5.9f)
		{
			m_CameraDesc.fFovy = XMConvertToRadians(25.f);
			m_bStart = true;
			_vector vPos = XMQuaternionSlerp(XMLoadFloat4(&m_vCamPos), XMVectorSet(32.8311f, 5.5f, 67.4087f, 1.f), m_fLerpTime);
	
			if (m_fLerpTime > 1.f)
			{
				m_bLerp = true;
			}
	
			m_fLerpTime += fTimeDelta;
			m_pTransform->LookAt(XMLoadFloat4(&m_vLerpLook));
			m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
		}
#else
		if (!m_bLerp && m_fStartTime > 1.4f)
		{
			m_CameraDesc.fFovy = XMConvertToRadians(25.f);
			m_bStart = true;
			//75.343f, 5.5f, 19.231f
			_vector vPos = XMQuaternionSlerp(XMLoadFloat4(&m_vCamPos), XMVectorSet(32.8311f, 5.5f, 67.4087f, 1.f), m_fLerpTime);
			
			if (m_fLerpTime > 1.f)
			{
				m_bLerp = true;
			}
	
			m_fLerpTime += fTimeDelta;
			m_pTransform->LookAt(XMLoadFloat4(&m_vLerpLook));
			m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
		}
#endif

		if (!m_bStart)
			Set_StartPos(fTimeDelta);
	
		if (true == bCamAttach && m_bLerp)
			Set_CamPos();


		if (m_pPlayer->Get_PlayerInfo().bSub)
			m_pPlayer = m_pPlayer->Get_SubChar();
		if (m_pTarget->Get_PlayerInfo().bSub)
			m_pTarget = m_pTarget->Get_SubChar();
	

		if (true == bCamAttach && m_bLerp)
			Move_CamPos(fTimeDelta);
	}
	else if (m_bStory && bCamAttach)
	{
		Key_Input(fTimeDelta);
	}
	RELEASE_INSTANCE(CGameInstance);


	if (FAILED(Bind_OnPipeLine()))
		return;
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (!m_bStory)
	{
		if (!m_bEffect && m_bStartBattle && ((CModel*)m_pPlayer->Find_Component(TEXT("Com_Model")))->Get_CurrentTime() > 25.f)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_GAMESTART, m_pPlayer);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		if (!m_bBattle)
		{
			Set_BattleTarget(fTimeDelta);
			m_bBattle = true;
		}
#ifdef _DEBUG
		if (!m_bStartBattle && m_fStartTime > 4.5f)
		{
			Set_BattleStart(fTimeDelta);
			m_bStartBattle = true;
		}
#else
		if (!m_bStartBattle && m_fStartTime > 1.5f)
		{
			Set_BattleStart(fTimeDelta);
			m_bStartBattle = true;
		}
#endif
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

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransform->Set_Rotation(_float3(0.f, m_fAngle, 0.f)); 

	_vector vLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_float fTime = 1.f;
	vPos -= vLook * (fTime + m_fLookY) * (fDiameter * 0.5f) * m_fCamDist;
	vPos.m128_f32[0] -= 3.f;
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

	Check_Shake(fTimeDelta);
}
void CCamera_Dynamic::Key_Input(_float fTimeDelta)
{
	m_pPlayer->Set_BattleTarget(m_pTarget);
	m_pTarget->Set_BattleTarget(m_pPlayer);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_long MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_WHEEL))
	{
		m_FovAngle += XMConvertToRadians(-MouseMove * fTimeDelta * 0.4f);
		if ((XMConvertToRadians(80.f) < m_CameraDesc.fFovy))
		{
			m_CameraDesc.fFovy = XMConvertToRadians(80.f);
			m_FovAngle = XMConvertToRadians(80.f);
		}
		else if ((XMConvertToRadians(35.f) > m_CameraDesc.fFovy))
		{
			m_CameraDesc.fFovy = XMConvertToRadians(35.f);
			m_FovAngle = XMConvertToRadians(35.f);
		}
	}
	m_CameraDesc.fFovy = m_FovAngle;

	static _float fAngleX = 0.f;
	static _float fAngleY = 0.f;

	_long         MouseMoveX = 0;
	_long         MouseMoveY = 0;

	CTransform* pTransform = m_pPlayer->Get_Transform();


	if (MouseMoveY = pGameInstance->Get_DIMMoveState(DIMM_Y))
	{

		fAngleX += MouseMoveY * fTimeDelta * 8.f;

		if (40.f <= fAngleX)
			fAngleX = 40.f;
		else if (-40.f >= fAngleX)
			fAngleX = -40.f;

	}

	if (MouseMoveX = pGameInstance->Get_DIMMoveState(DIMM_X))
	{

		fAngleY += MouseMoveX * fTimeDelta * 8.f;

		if (360.f <= fAngleY)
			fAngleY = 0.f;
		else if (0.f >= fAngleY)
			fAngleY = 360.f;

	}

	_matrix matRotX = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(fAngleX));
	_matrix matRotY = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fAngleY));

	_vector vCamDir = XMVector3TransformNormal(XMVectorSet(0.f, 1.f, -1.f, 0.f), matRotX);
	vCamDir = XMVector3TransformNormal(vCamDir, matRotY);
	_vector vCamPos = vCamDir * 3.f;
	_vector vDestPos = pTransform->Get_State(CTransform::STATE_TRANSLATION) + vCamPos;

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vDestPos);

	_vector vLookPos = XMVectorSetY(pTransform->Get_State(CTransform::STATE_TRANSLATION), XMVectorGetY(pTransform->Get_State(CTransform::STATE_TRANSLATION)) + 0.8f);
	vLookPos.m128_f32[1] += 1.f;
	m_pTransform->LookAt(vLookPos);

	RELEASE_INSTANCE(CGameInstance);

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



void CCamera_Dynamic::Set_BattleTarget(_float fTimeDelta)
{
	m_pPlayer->Set_BattleTarget(m_pTarget);
	m_pTarget->Set_BattleTarget(m_pPlayer);
	m_pPlayer->Get_Transform()->LookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_pTarget->Get_Transform()->LookAt(m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

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
	return false;
}

void CCamera_Dynamic::Set_StartPos(_float fTimeDelta)
{
	_vector vPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTarget = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook2 = vPos - vTarget;

	_float fDist = XMVectorGetX(XMVector3Length(vLook2));

	vPos -= XMVector3Normalize(vLook2) * (fDist * 0.5f);
	vPos.m128_f32[1] = m_fLookAtY;
	m_pTransform->LookAt(vPos);
	XMStoreFloat4(&m_vLerpLook, vPos);
	vPos.m128_f32[1] = 0.f;
	
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
//	m_pTransform->
//	m_pTransform->Set_Rotation(_float3(0.f, 30.f, 0.f));

	_vector vLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));

	vPos -= vLook * 35.f;
	vPos.m128_f32[1] = 0.f;
	vPos.m128_f32[1] += 4.7f;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);

	m_CameraDesc.fFovy = XMConvertToRadians(m_fFov);
	m_fFovTime += fTimeDelta;
#ifdef _DEBUG
	if (m_fFovTime < 5.5f && m_fFovTime > 5.f)
	{
		m_fFov -= 0.6f;
		m_fLookAtY -= 0.075f;
	}
	else if (m_fFovTime < 6.8f && m_fFovTime > 6.f)
	{
		m_fFov += 0.6f;
		m_fLookAtY += 0.3f;
	}
#else
	if (m_fFovTime < 2.5f && m_fFovTime > 2.f)
	{
		m_fFov -= 0.6f;
		m_fLookAtY -= 0.075f;
	}
	else if (m_fFovTime < 3.8f && m_fFovTime > 3.f)
	{
		m_fFov += 0.6f;
	//	m_fLookAtY += 0.3f;
	}
#endif
	if (m_fLookAtY < 3.f)
		m_fLookAtY = 3.f;
	else if (m_fLookAtY > 4.f)
		m_fLookAtY = 4.f;
	if (m_fFov < 10.f)
		m_fFov = 10.f;
	else if (m_fFov > 25.f)
		m_fFov = 25.f;

	
	XMStoreFloat4(&m_vCamPos, m_pTransform->Get_State(CTransform::STATE_TRANSLATION));
}

void CCamera_Dynamic::Set_BattleStart(_float fTimeDelta)
{
	m_pPlayer->Set_BattleStart(true);
	m_pTarget->Set_BattleStart(true);

}
void CCamera_Dynamic::Check_Shake(_float fTimeDelta)
{
	if (m_ShakeTime > 0.f)
	{
		switch (m_ShakeInfo)
		{
		case CCamera_Dynamic::SHAKE::SHAKE_DOWN:
			Camera_ShakeDown(fTimeDelta);
			break;
		case CCamera_Dynamic::SHAKE::SHAKE_HIT:
			Camera_ShakeHit(fTimeDelta);
			break;
		default:
			break;
		}

		m_ShakeTime -= fTimeDelta;
	}
	
}

void CCamera_Dynamic::Camera_ShakeDown(_float fTimeDelta)
{
	m_fShakeAmount = 0.5f;
	m_fShakeFrequency = 5.f;
	_float fProgress = (fTimeDelta - 0.016667f) / m_fShakeFrequency;
	_float fShake = m_fShakeAmount * (1.f - fProgress) * ((rand() % 200) - 100) * 0.01f;
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	vPos += XMVectorSet(fShake, fShake, fShake, 0.f);

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
}
void CCamera_Dynamic::Camera_ShakeHit(_float fTimeDelta)
{
	m_fShakeAmount = 0.15f;
	m_fShakeFrequency = 20.f;
	_float fProgress = (fTimeDelta - 0.016667f) / m_fShakeFrequency;
	_float fShake = m_fShakeAmount * (1.f - fProgress) * ((rand() % 200) - 100) * 0.01f;
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	vPos += XMVectorSet(fShake, fShake, fShake, 0.f);

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
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
