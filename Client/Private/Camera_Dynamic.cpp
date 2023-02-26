#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "GameObj.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"
#include "SoundMgr.h" 
#include "Tanjiro.h"
CCamera_Dynamic::CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

void CCamera_Dynamic::Change_CutScene(CUTSCENE eCutScene, vector<_float4> vecPositions, vector<_float4> vecLookAts, vector<_float> vecUseTime, _float2 vMotionBlur)
{
	if (eCutScene >= CUTSCENE_END)
	{
		ERR_MSG(L"Failed to Change : CutScene");
		return;
	}

	m_vecCamEye[eCutScene] = vecPositions;
	m_vecCamAt[eCutScene] = vecLookAts;
	m_vecCamTime[eCutScene] = vecUseTime;
	m_vecMotionBlur[eCutScene] = vMotionBlur;

	ERR_MSG(L"Changed!");
}

void CCamera_Dynamic::Start_CutScene(_bool bTrueisPlay, CUTSCENE eCutScene)
{
	m_bCutScene = bTrueisPlay;
	m_eCutScene = eCutScene;
}

_bool CCamera_Dynamic::Play_CutScene(vector<_float4> vecPositions, vector<_float4> vecLookAts, vector<_float> vecUseTime, _float * pOut, _float fTimeDelta)
{
	_int iSize = (_int)vecPositions.size();

	if (iSize < 4 ||
		(iSize != (_int)vecLookAts.size() || iSize != (_int)vecUseTime.size() + 1))
		return false;

	static _float fCullTime = 0.f;
	_float	fUsedTime = fCullTime;
	_int	iFrame = max(_int(fCullTime), 0) + 1;				//	현재 프레임. 첫 번째는 읽지않음.(None)
	_float	fDecimal = max(fCullTime, 0.f) - (iFrame - 1);

	if (iFrame + 1 >= iSize)		//	끝 Check
	{
		fCullTime = *pOut = 0.f;

		return false;
	}

	*pOut += (fTimeDelta) / (vecUseTime[iFrame]);
	fCullTime = *pOut;


	_vector vCamPos, vCamAt;
	_vector vAt[4];
	_vector vPos[4];

	for (_int j = 0; j < 4; ++j)
	{
		_int	iIndex = max(min(iFrame + j - 1, iSize - 1), 1);		//	최소 = 0, 최대 = Size, 첫번째는 읽지 않음.
		vPos[j] = XMLoadFloat4(&vecPositions[iIndex]);
		vAt[j] = XMLoadFloat4(&vecLookAts[iIndex]);
	}



	_float fRatio = fDecimal;		//	다음 프레임의 할당 비율
	vCamPos = XMVectorCatmullRom(vPos[0], vPos[1], vPos[2], vPos[3], fRatio);
	vCamAt = XMVectorCatmullRom(vAt[0], vAt[1], vAt[2], vAt[3], fRatio);

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vCamPos);
	m_pTransform->LookAt(vCamAt);

	return true;
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
	m_fZoomAngle = 25.f;
	m_fQuestFov = 50.f;
	m_fCamY = 10.f;
	m_fBattleCamY = 5.f;
	m_fBattleCamZ = 30.f;
	m_FovAngle = XMConvertToRadians(60.f);
	//m_bStory = true;
	m_eTurn = CAM_END;


	m_fCurrentCutSceneTime = 0.f;
	m_bCutScene = false;
	if (FAILED(Ready_CutScene("tanjiro_1"))) return E_FAIL;
	if (FAILED(Ready_CutScene("tanjiro_2"))) return E_FAIL;
	if (FAILED(Ready_CutScene("tanjiro_3"))) return E_FAIL;
	if (FAILED(Ready_CutScene("tanjiro_4"))) return E_FAIL;
	if (FAILED(Ready_CutScene("tanjiro_5"))) return E_FAIL;

	if (FAILED(Ready_CutScene("rui_Start"))) return E_FAIL;
	if (FAILED(Ready_CutScene("rui_0"))) return E_FAIL;
	if (FAILED(Ready_CutScene("rui_1"))) return E_FAIL;
	if (FAILED(Ready_CutScene("rui_0"))) return E_FAIL;	//	2
	if (FAILED(Ready_CutScene("rui_0"))) return E_FAIL;	//	3
	if (FAILED(Ready_CutScene("rui_0"))) return E_FAIL;	//	4
	if (FAILED(Ready_CutScene("rui_0"))) return E_FAIL;	//	5
	if (FAILED(Ready_CutScene("rui_0"))) return E_FAIL;	//	6

	if (g_iLevel == LEVEL_BOSSENMU)
	{
		m_bStart = true;
		m_bLerp = true;
	}


	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	CComponent* pOut = pGameInstance->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Renderer");
	m_pRendererCom = (CRenderer*)pOut;
	if (nullptr == m_pRendererCom)
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);


	if (FAILED(Bind_OnPipeLine()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	static _bool	bCamAttach = true;
	if (true == m_bCutScene)
	{
		m_bCutScene = CutScene(m_eCutScene, fTimeDelta);	//	끝나면 m_bCutScene = false. 자동으로 못들어옴.
	}
	else
	{
		CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
		if (pGameInstance->Key_Down(DIK_F10))
			bCamAttach = !bCamAttach;

		if (false == bCamAttach)
		{
			_float fSpeed = 1.f;
			if (pGameInstance->Key_Pressing(DIK_RSHIFT))
				fSpeed *= 10.f;

			//if (pGameInstance->Key_Pressing(DIK_UP))
			//	m_pTransform->Go_Straight(fTimeDelta * fSpeed);
			//if (pGameInstance->Key_Pressing(DIK_DOWN))
			//	m_pTransform->Go_Backward(fTimeDelta * fSpeed);
			//if (pGameInstance->Key_Pressing(DIK_LEFT))
			//	m_pTransform->Go_Left(fTimeDelta * fSpeed);
			//if (pGameInstance->Key_Pressing(DIK_RIGHT))
			//	m_pTransform->Go_Right(fTimeDelta * fSpeed);

			//if (pGameInstance->Key_Pressing(DIK_1))
			//	m_pTransform->Go_Straight(fTimeDelta * fSpeed);
			//if (pGameInstance->Key_Pressing(DIK_2))
			//	m_pTransform->Go_Backward(fTimeDelta * fSpeed);
			//if (pGameInstance->Key_Pressing(DIK_3))
			//	m_pTransform->Go_Left(fTimeDelta * fSpeed);
			//if (pGameInstance->Key_Pressing(DIK_4))
			//	m_pTransform->Go_Right(fTimeDelta * fSpeed);

			if (pGameInstance->Key_Pressing(DIK_NUMPAD8))
				m_pTransform->Go_Straight(fTimeDelta * fSpeed);
			if (pGameInstance->Key_Pressing(DIK_NUMPAD5))
				m_pTransform->Go_Backward(fTimeDelta * fSpeed);
			if (pGameInstance->Key_Pressing(DIK_NUMPAD4))
				m_pTransform->Go_Left(fTimeDelta * fSpeed);
			if (pGameInstance->Key_Pressing(DIK_NUMPAD6))
				m_pTransform->Go_Right(fTimeDelta * fSpeed);

			if (pGameInstance->Key_Pressing(DIK_NUMPAD9))
			{
				_vector vPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
				vPos += XMVectorSet(0.f, fTimeDelta * fSpeed * 3.f, 0.f, 0.f);
				m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}
			if (pGameInstance->Key_Pressing(DIK_NUMPAD3))
			{
				_vector vPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
				vPos -= XMVectorSet(0.f, fTimeDelta * fSpeed * 3.f, 0.f, 0.f);
				m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}

			if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
			{
				_long			MouseMove = 0;

				if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
					m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);

				if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
					m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
			}
		}
		if (pUIManager->Get_RuiDadBattle())
		{
			if (!m_bBattle)
			{
				Set_BattleTarget(fTimeDelta);
				m_bBattle = true;
			}
			if (!m_bStartBattle)
			{
				m_pTarget->Set_BattleStart(true);
				m_bStartBattle = true;
			}

			if (!m_bQuestBattleCam && !m_bLerp)
				QuestBattleCam(fTimeDelta);
			else if (m_bLerp && !m_bQuestBattleCam)
			{
				_vector vPos = XMVectorLerp(XMLoadFloat4(&m_vCamPos), XMVectorSet(32.8311f, 5.5f, 67.4087f, 1.f), m_fLerpTime);
				if (m_fLerpTime > 1.f)
				{
					m_bQuestBattleCam = true;
				}

				m_fLerpTime += fTimeDelta;
				m_pTransform->LookAt(XMLoadFloat4(&m_vLerpLook));
				m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}

			if (true == bCamAttach && m_bQuestBattleCam)
				Set_CamPos();

			if (m_pPlayer->Get_PlayerInfo().bSub)
				m_pPlayer = m_pPlayer->Get_SubChar();
			if (m_pTarget->Get_PlayerInfo().bSub)
				m_pTarget = m_pTarget->Get_SubChar();

			if (true == bCamAttach && m_bQuestBattleCam)
				Move_CamPos(fTimeDelta);
		}
		else if (!m_bStory && !pUIManager->Get_RuiDadBattle())
		{
			m_fStartTime += fTimeDelta;

#ifdef _DEBUG
			if (!m_bLerp && m_fStartTime > 5.9f)
			{
				m_CameraDesc.fFovy = XMConvertToRadians(25.f);
				m_bStart = true;
				_vector vPos;
				if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
					vPos = XMQuaternionSlerp(XMLoadFloat4(&m_vCamPos), XMVectorSet(-14.292f, 21.707f, 231.684f, 1.f), m_fLerpTime);
				else
					vPos = XMQuaternionSlerp(XMLoadFloat4(&m_vCamPos), XMVectorSet(32.8311f, 5.5f, 67.4087f, 1.f), m_fLerpTime);

				if (m_fLerpTime > 1.f)
				{
					m_bLerp = true;
				}

				m_fLerpTime += fTimeDelta;
				m_pTransform->LookAt(XMLoadFloat4(&m_vLerpLook));
				m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}
			if (m_fStartTime > 3.f && g_iLevel != LEVEL_BOSSENMU)
				m_bStart = true;
#else
			if (!m_bLerp && m_fStartTime > 1.5f && g_iLevel != LEVEL_BOSSENMU)
			{
				m_CameraDesc.fFovy = XMConvertToRadians(25.f);
				//m_bStart = true;
				//75.343f, 5.5f, 19.231f
				_vector vPos;
				if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
					vPos = XMQuaternionSlerp(XMLoadFloat4(&m_vCamPos), XMVectorSet(-14.292f, 21.707f, 231.684f, 1.f), m_fLerpTime);
				else
					vPos = XMQuaternionSlerp(XMLoadFloat4(&m_vCamPos), XMVectorSet(32.8311f, 5.5f, 67.4087f, 1.f), m_fLerpTime);

				if (m_fLerpTime > 1.f)
				{
					m_bLerp = true;
				}

				m_fLerpTime += fTimeDelta;
				m_pTransform->LookAt(XMLoadFloat4(&m_vLerpLook));
				m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
			}
			if (m_fStartTime > 1.f && g_iLevel != LEVEL_BOSSENMU)
				m_bStart = true;
#endif
		
			if (!m_bStart)
				Set_StartPos(fTimeDelta);

			if (true == bCamAttach && m_bLerp)
				Set_CamPos();


			if (m_pPlayer->Get_PlayerInfo().bSub)
				m_pPlayer = m_pPlayer->Get_SubChar();
			if (m_pTarget->Get_PlayerInfo().bSub)
				m_pTarget = m_pTarget->Get_SubChar();


			if (true == bCamAttach && m_bLerp && g_iLevel != LEVEL_BATTLEENMU && g_iLevel != LEVEL_BOSSENMU)
				Move_CamPos(fTimeDelta);
			else if (true == bCamAttach && m_bLerp && (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU))
				Move_TrainCam(fTimeDelta);
		}
		else if (m_bStory && bCamAttach)
		{
			if (!m_bQuestCam)
				Key_Input(fTimeDelta);
			else
				QuestCam(fTimeDelta);


			Check_Shake(fTimeDelta);
			//Check_StoryCam();

		}
		RELEASE_INSTANCE(CGameInstance);
		RELEASE_INSTANCE(CUI_Manager);
	}

	if (FAILED(Bind_OnPipeLine()))
		return;
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	CUI_Manager* pUIManager = GET_INSTANCE(CUI_Manager);

#ifdef _DEBUG
	if (!m_bStory && !pUIManager->Get_RuiDadBattle())
	{
		if (!m_bEffect)
		{
			Check_Model();
		}
		if (m_fStartTime > 4.6f && !m_bEffect && m_bStartBattle && ((CModel*)m_pPlayer->Find_Component(TEXT("Com_Model")))->Get_CurrentTime_Index(m_iAnimIndex) > 25.f
			&& g_iLevel != LEVEL_BOSSENMU)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_GAMESTART, m_pPlayer);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		if (m_fStartTime > 4.6f && !m_bBattleSound && m_bStartBattle && ((CModel*)m_pPlayer->Find_Component(TEXT("Com_Model")))->Get_CurrentTime_Index(m_iAnimIndex) > 22.f
			&& g_iLevel != LEVEL_BOSSENMU)
		{
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("BattleStart.wav"), fEFFECT);
			m_bBattleSound = true;
		}
		if (!m_bBattle)
		{
			Set_BattleTarget(fTimeDelta);
			m_bBattle = true;
		}
		if (!m_bStartBattle && m_fStartTime > 4.5f)
		{
			Set_BattleStart(fTimeDelta);
			m_bStartBattle = true;
		}
	}
#else
	if (!m_bStory && !pUIManager->Get_RuiDadBattle())
	{
		if (!m_bEffect)
		{
			Check_Model();
		}
		if (m_fStartTime > 0.2f && !m_bEffect && m_bStartBattle && ((CModel*)m_pPlayer->Find_Component(TEXT("Com_Model")))->Get_CurrentTime_Index(m_iAnimIndex) > 25.f
			&& g_iLevel != LEVEL_BOSSENMU)
		{

			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_GAMESTART, m_pPlayer);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		if (m_fStartTime > 0.2f && !m_bBattleSound && m_bStartBattle && ((CModel*)m_pPlayer->Find_Component(TEXT("Com_Model")))->Get_CurrentTime_Index(m_iAnimIndex) > 22.f
			&& g_iLevel != LEVEL_BOSSENMU)
		{
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("BattleStart.wav"), fEFFECT);
			m_bBattleSound = true;
		}
		if (!m_bBattle)
		{
			Set_BattleTarget(fTimeDelta);
			m_bBattle = true;
		}
		if (!m_bStartBattle && m_fStartTime > 0.1f)
		{
			Set_BattleStart(fTimeDelta);
			m_bStartBattle = true;
		}
	}
#endif
	RELEASE_INSTANCE(CUI_Manager);
}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}


_bool CCamera_Dynamic::Check_Level(_vector vPos)
{
	if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
	{
		if (vPos.m128_f32[1] < 16.6f)
			return true;
	}
	else
	{
		if (vPos.m128_f32[1] < 0.1f)
			return true;
	}
	return false;
}

void CCamera_Dynamic::Move_TrainCam(_float fTimeDelta)
{
	ConvertToViewPort(fTimeDelta);
	if (!m_bZoom)
	{
		if (m_f1pX < 200.f)
		{
			while (true)
			{
				if (m_f1pX >= 200.f)
					break;
				m_fAngle += 0.1f;
				if (m_fAngle >= 360)
					m_fAngle = 0.f;
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
				if (m_fAngle <= -360)
					m_fAngle = 0.f;
				Set_CamPos();

				ConvertToViewPort(fTimeDelta);
			}
		}
	}
	_vector v1PY = m_p1P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_bool	bTrue = m_b1P;
	if (g_iLevel != LEVEL_BOSSENMU)
	{
		if (m_fTurnCol > 0.f)
			m_fTurnCol -= fTimeDelta;
		if (m_fDist < 8.f && m_eTurn == CAM_END && m_p1P->Get_PlayerInfo().iCombo > 1)
		{
			if (m_fTurnCol <= 0.f)
				m_bTurn = true;
		}
		else if (m_fDist < 8.f && m_eTurn == CAM_END && m_p2P->Get_PlayerInfo().iCombo > 1)
		{
			if (m_fTurnCol <= 0.f)
				m_bTargetTurn = true;
		}
		if (m_bTurn)
			Check_Trun(fTimeDelta);
		else if (m_bTargetTurn)
			Check_TargetTrun(fTimeDelta);
	}
	_vector vPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	if (CheckSubChar())
		vPos.m128_f32[1] += v1PY.m128_f32[1] / 2.f;
	if (m_bZoom && g_iLevel == LEVEL_BOSSENMU)
	{
		if (vPos.m128_f32[2] > 230.f)
			vPos.m128_f32[2] = 230.f;
		if (vPos.m128_f32[0] < -30.f)
			vPos.m128_f32[0] = -30.f;
		else if (vPos.m128_f32[0] > 35.f)
			vPos.m128_f32[0] = 35.f;
	}
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pPlayer->Set_CamAngle(m_fAngle - fAngleDot);
	m_pTarget->Set_CamAngle(m_fAngle - fAngleDot);

	Check_Shake(fTimeDelta);

	if (m_bZoom)
		Check_Zoom(fTimeDelta);
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
	m_fDist = fDist;
	//맵의 임시 반지름
	_float fDiameter = 0.f;

	if (g_iLevel == LEVEL_BATTLEENMU)
	{
		fDiameter = 60.f;

		m_fCamDist = fDist / fDiameter;
		if (m_fCamDist > 1.f)
			m_fCamDist = 1.f;
		else if (m_fCamDist < 0.7)
			m_fCamDist = 0.7f;

		vPos += XMVector3Normalize(vLook2) * (fDist * 0.5f);
	}
	else if (g_iLevel == LEVEL_BOSSENMU)
	{
		fDiameter = 60.f;

		m_fCamDist = 1.f - (fDist / fDiameter);
		if (m_fCamDist > 1.f)
			m_fCamDist = 1.f;
		else if (m_fCamDist < 0.33)
			m_fCamDist = 0.33f;

		vPos += XMVector3Normalize(vLook2) * (fDist * 0.5f);
	}
	else
	{
		fDiameter = 85.f;

		m_fCamDist = fDist / fDiameter;
		if (m_fCamDist > 1.f)
			m_fCamDist = 1.f;
		else if (m_fCamDist < 0.33)
			m_fCamDist = 0.33f;

		vPos -= XMVector3Normalize(vLook2) * (fDist * 0.5f);
	}

	if (m_eTurn == CAM_RIGHT || m_eTurn == CAM_TARGETLEFT)
	{
		fAngleDot -= 3.f;
		if (fAngleDot <= -360.f)
			fAngleDot = 0.f;
		fTurnAngle -= 3.f;
		if (fTurnAngle <= 0.f)
		{
			m_eTurn = CAM_END;
			m_fTurnCol = 1.f;
		}
	}
	else if (m_eTurn == CAM_LEFT || m_eTurn == CAM_TARGETRIGHT)
	{
		fAngleDot += 3.f;
		if (fAngleDot >= 360.f)
			fAngleDot = 0.f;
		fTurnAngle -= 3.f;
		if (fTurnAngle <= 0.f)
		{
			m_eTurn = CAM_END;
			m_fTurnCol = 1.f;
		}
	}


	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pTransform->Set_Rotation(_float3(0.f, m_fAngle - fAngleDot, 0.f));
	_vector vLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_float fTime = 1.f;
	vPos -= vLook * (fTime + m_fLookY) * (fDiameter * 0.5f) * m_fCamDist;
	vPos.m128_f32[0] -= 3.f;

	if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
		vPos.m128_f32[1] = 8.f;
	else
		vPos.m128_f32[1] = 0.f;

	vPos.m128_f32[1] += 5.5f;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
}

void CCamera_Dynamic::Move_CamPos(_float fTimeDelta)
{
	ConvertToViewPort(fTimeDelta);
	if (!m_bZoom)
	{
		if (m_f1pX < 200.f)
		{
			while (true)
			{
				if (m_f1pX >= 200.f)
					break;
				m_fAngle += 0.1f;
				if (m_fAngle >= 360)
					m_fAngle = 0.f;
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
				if (m_fAngle <= -360)
					m_fAngle = 0.f;
				Set_CamPos();

				ConvertToViewPort(fTimeDelta);
			}
		}
	}
	_vector v1PY = m_p1P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_bool	bTrue = m_b1P;
	if (CheckSubChar() && !m_bZoom)
	{
		if (!m_p1P->Get_PlayerInfo().bJump && Check_Level(v1PY))
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
					if (bTrue != m_b1P)
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
	if (m_fTurnCol > 0.f)
		m_fTurnCol -= fTimeDelta;
	if (m_fDist < 8.f && m_eTurn == CAM_END && m_p1P->Get_PlayerInfo().iCombo > 1)
	{
		if (m_fTurnCol <= 0.f)
			m_bTurn = true;
	}
	else if (m_fDist < 8.f && m_eTurn == CAM_END && m_p2P->Get_PlayerInfo().iCombo > 1)
	{
		if (m_fTurnCol <= 0.f)
			m_bTargetTurn = true;
	}
	if (m_bTurn)
		Check_Trun(fTimeDelta);
	else if (m_bTargetTurn)
		Check_TargetTrun(fTimeDelta);

	_vector vPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	if (CheckSubChar())
		vPos.m128_f32[1] += v1PY.m128_f32[1] / 2.f;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	m_pPlayer->Set_CamAngle(m_fAngle - fAngleDot);
	m_pTarget->Set_CamAngle(m_fAngle - fAngleDot);

	Check_Shake(fTimeDelta);

	if (m_bZoom)
		Check_Zoom(fTimeDelta);


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
	static _float fAngleY = 180.f;

	_long         MouseMoveX = 0;
	_long         MouseMoveY = 0;

	CTransform* pTransform = m_pPlayer->Get_Transform();

	if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
	{
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
	}
	_matrix matRotX = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(fAngleX));
	_matrix matRotY = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fAngleY));

	_vector vCamDir = XMVector3TransformNormal(XMVectorSet(0.f, 1.f, -1.f, 0.f), matRotX);
	vCamDir = XMVector3TransformNormal(vCamDir, matRotY);
	_vector vCamPos = vCamDir * 5.f;
	_vector vDestPos = pTransform->Get_State(CTransform::STATE_TRANSLATION) + vCamPos;

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vDestPos);

	_vector vLookPos = XMVectorSetY(pTransform->Get_State(CTransform::STATE_TRANSLATION), XMVectorGetY(pTransform->Get_State(CTransform::STATE_TRANSLATION)) + 0.8f);
	vLookPos.m128_f32[1] += 3.f;
	m_pTransform->LookAt(vLookPos);

	RELEASE_INSTANCE(CGameInstance);

}
void CCamera_Dynamic::Blur_High(CRenderer * _pRenderer)
{
	_pRenderer->Set_PointBlur(m_vAtPos, 100.f, 0.5f, 0.6f);
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
	if (g_iLevel == LEVEL_BATTLEENMU)
	{
		vAtPos = vTargetPos;
		vAtPos.m128_f32[1] = 11.f;
	}
	else if (g_iLevel == LEVEL_BOSSENMU)
	{
		vAtPos = vTargetPos;
		vAtPos.m128_f32[1] -= 6.5f;
	}
	else
		vAtPos.m128_f32[1] = 3.f;
	m_pTransform->LookAt(vAtPos);
	XMStoreFloat3(&m_vAtPos, vAtPos);
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
		m_p2P = m_pTarget;
		m_b1P = true;
	}
	else
	{
		if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
		{
			m_f1pX = m_vPlayerPos.x;
			m_f1pY = m_vPlayerPos.y;
			m_f2pX = m_vTargetPos.x;
			m_f2pY = m_vTargetPos.y;
		}
		else
		{
			m_f1pX = m_vTargetPos.x;
			m_f1pY = m_vTargetPos.y;
			m_f2pX = m_vPlayerPos.x;
			m_f2pY = m_vPlayerPos.y;
		}
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
		m_p2P = m_pPlayer;
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
	else if (!bPlayerSub && bTargetSub)
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
	_vector vLook2;

	if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
		vLook2 = vTarget - vPos;
	else
		vLook2 = vPos - vTarget;

	_float fDist = XMVectorGetX(XMVector3Length(vLook2));

	if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
		vPos += XMVector3Normalize(vLook2) * (fDist * 0.5f);
	else
		vPos -= XMVector3Normalize(vLook2) * (fDist * 0.5f);

	if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
		vPos.m128_f32[1] += 2.5f;
	else
		vPos.m128_f32[1] = m_fLookAtY;

	m_pTransform->LookAt(vPos);
	XMStoreFloat4(&m_vLerpLook, vPos);

	if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
		vPos.m128_f32[1] -= 2.5f;
	else
		vPos.m128_f32[1] = 0.f;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPos);
	//	m_pTransform->
	if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
	{
		//if (m_fTurnAngle < 90.f)
		//	m_pTransform->Set_Rotation(_float3(0.f, m_fTurnAngle, 0.f));
		//m_fTurnAngle += 3.f;
		m_pTransform->Set_Rotation(_float3(0.f, 90.f, 0.f));
	}
	_vector vLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));

	vPos -= vLook * 35.f;

	if (g_iLevel != LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
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
	if (m_fFovTime < 1.f && m_fFovTime > 0.5f)
	{
		m_fFov -= 0.12f;
		m_fLookAtY -= 0.02f;
	}
	else if (m_fFovTime < 9.3f && m_fFovTime > 1.f)
	{
		m_fFov += 0.15f;
		//	m_fLookAtY += 0.3f;
	}
#endif
	if (g_iLevel == LEVEL_BATTLEENMU || g_iLevel == LEVEL_BOSSENMU)
	{
		//if (m_fLookAtY < 19.f)
		//	m_fLookAtY = 19.f;
		//else if (m_fLookAtY > 20.f)
		//	m_fLookAtY = 20.f;
	}
	else
	{
		if (m_fLookAtY < 3.f)
			m_fLookAtY = 3.f;
		else if (m_fLookAtY > 4.f)
			m_fLookAtY = 4.f;
	}

	if (m_fFov < 10.f)
		m_fFov = 10.f;
	else if (m_fFov > 25.f)
		m_fFov = 25.f;


	XMStoreFloat4(&m_vCamPos, m_pTransform->Get_State(CTransform::STATE_TRANSLATION));
	m_pTransform->LookAt(XMLoadFloat4(&m_vLerpLook));

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

void CCamera_Dynamic::Check_Model()
{
	if (m_pPlayer->Get_PlayerInfo().strName == TEXT("탄지로"))
		m_iAnimIndex = 99;
	else if (m_pPlayer->Get_PlayerInfo().strName == TEXT("쿄주로"))
		m_iAnimIndex = 82;
	else if (m_pPlayer->Get_PlayerInfo().strName == TEXT("루이"))
		m_iAnimIndex = 83;
	else if (m_pPlayer->Get_PlayerInfo().strName == TEXT("아카자"))
		m_iAnimIndex = 64;
	else if (m_pPlayer->Get_PlayerInfo().strName == TEXT("네즈코"))
		m_iAnimIndex = 10;
	else if (m_pPlayer->Get_PlayerInfo().strName == TEXT("시노부"))
		m_iAnimIndex = 89;
}

void CCamera_Dynamic::Check_Zoom(_float fTimeDelta)
{
	switch (m_ZoomInfo)
	{
	case CCamera_Dynamic::ZOOM::ZOOM_LOW:
		Zoom_Low(fTimeDelta);
		break;
	case CCamera_Dynamic::ZOOM::ZOOM_MIDDLE:
		Zoom_Middle(fTimeDelta);
		break;
	case CCamera_Dynamic::ZOOM::ZOOM_HIGH:
		Zoom_High(fTimeDelta);
		break;
	default:
		break;
	}
}

void CCamera_Dynamic::Zoom_Low(_float fTimeDelta)
{
	if (!m_bZoomIn)
	{
		m_fZoomAngle -= fTimeDelta * 25.f;
		if (m_fZoomAngle < 18.f)
			m_bZoomIn = true;
	}
	else if (m_bZoomIn)
	{
		m_fZoomAngle += fTimeDelta * 15.f;
		if (m_fZoomAngle >= 25.f)
		{
			m_bZoomIn = false;
			m_fZoomAngle = 25.f;
			m_bZoom = false;
		}
	}
	m_CameraDesc.fFovy = XMConvertToRadians(m_fZoomAngle);
}

void CCamera_Dynamic::Zoom_Middle(_float fTimeDelta)
{
	if (!m_bZoomIn)
	{
		m_fZoomAngle -= fTimeDelta * 25.f;
		if (m_fZoomAngle < 17.f)
			m_bZoomIn = true;
	}
	else if (m_bZoomIn)
	{
		m_fZoomAngle += fTimeDelta * 12.5f;
		if (m_fZoomAngle >= 25.f)
		{
			m_bZoomIn = false;
			m_fZoomAngle = 25.f;
			m_bZoom = false;
		}
	}
	m_CameraDesc.fFovy = XMConvertToRadians(m_fZoomAngle);
}

void CCamera_Dynamic::Zoom_High(_float fTimeDelta)
{
	if (!m_bZoomIn)
	{
		m_fZoomAngle -= fTimeDelta * 30.f;
		if (m_fZoomAngle < 15.f)
			m_bZoomIn = true;
	}
	else if (m_bZoomIn)
	{
		m_fZoomAngle += fTimeDelta * 15.f;
		if (m_fZoomAngle >= 25.f)
		{
			m_bZoomIn = false;
			m_fZoomAngle = 25.f;
			m_bZoom = false;
		}
	}
	m_CameraDesc.fFovy = XMConvertToRadians(m_fZoomAngle);
}

void CCamera_Dynamic::Check_Trun(_float fTimeDelta)
{
	if (nullptr == m_p1P ||
		nullptr == m_p2P)
	{
		return;
	}
	_vector vPos = m_p1P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTarget = m_p2P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook2 = vPos - vTarget;

	_float fDist = XMVectorGetX(XMVector3Length(vLook2));

	vPos -= XMVector3Normalize(vLook2) * (fDist * 0.5f);

	_vector vTurnLook = m_p1P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) - vPos;
	_vector vCamTurnLook = m_pTransform->Get_State(CTransform::STATE_LOOK) * -1.f;
	_vector vTurnDot = XMVector3Dot(XMVector3Normalize(vTurnLook), XMVector3Normalize(vCamTurnLook));
	fTurnAngle = XMConvertToDegrees(acosf(XMVectorGetX(vTurnDot)));


	_vector vPlayerPosition = m_p1P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector vUpVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vCamLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector vTargetLook = XMVector3Normalize(vPlayerPosition - vMyPosition);

	_vector vCross = XMVector3Cross(vCamLook, vTargetLook);
	_vector vDot = XMVector3Dot(vCross, vUpVector);


	if (XMVectorGetX(vDot) > 0.f)
	{
		if (fTurnAngle >= 0.f && fTurnAngle < 90.f)
		{
			fTurnAngle = 90.f - fTurnAngle;
			m_bTurn = false;
			m_eTurn = CAM_RIGHT;
		}
		else
			m_bTurn = false;
	}
	else
	{
		if (fTurnAngle >= 0.f && fTurnAngle < 90.f)
		{
			fTurnAngle = 90.f - fTurnAngle;
			m_bTurn = false;
			m_eTurn = CAM_LEFT;
		}
		else
			m_bTurn = false;
	}

}

void CCamera_Dynamic::Check_TargetTrun(_float fTimeDelta)
{
	if (nullptr == m_p1P ||
		nullptr == m_p2P)
	{
		return;
	}
	_vector vPos = m_p1P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTarget = m_p2P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook2 = vPos - vTarget;

	_float fDist = XMVectorGetX(XMVector3Length(vLook2));

	vPos -= XMVector3Normalize(vLook2) * (fDist * 0.5f);

	_vector vTurnLook = m_p1P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) - vPos;
	_vector vCamTurnLook = m_pTransform->Get_State(CTransform::STATE_LOOK) * -1.f;
	_vector vTurnDot = XMVector3Dot(XMVector3Normalize(vTurnLook), XMVector3Normalize(vCamTurnLook));
	fTurnAngle = XMConvertToDegrees(acosf(XMVectorGetX(vTurnDot)));


	_vector vPlayerPosition = m_p2P->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vMyPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector vUpVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vCamLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector vTargetLook = XMVector3Normalize(vPlayerPosition - vMyPosition);

	_vector vCross = XMVector3Cross(vCamLook, vTargetLook);
	_vector vDot = XMVector3Dot(vCross, vUpVector);


	if (XMVectorGetX(vDot) > 0.f)
	{
		if (fTurnAngle >= 0.f && fTurnAngle < 90.f)
		{
			fTurnAngle = 90.f - fTurnAngle;
			m_bTargetTurn = false;
			m_eTurn = CAM_LEFT;
		}
		else
			m_bTargetTurn = false;
	}
	else
	{
		if (fTurnAngle >= 0.f && fTurnAngle < 90.f)
		{
			fTurnAngle = 90.f - fTurnAngle;
			m_bTargetTurn = false;
			m_eTurn = CAM_RIGHT;
		}
		else
			m_bTargetTurn = false;
	}
}

void CCamera_Dynamic::QuestCam(_float fTimeDelta)
{
	_vector vLookAt = m_pNPC->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLookAt.m128_f32[1] += 3.f;


	_vector vLookPos = { 0.f,m_fCamY,15.f,1.f };
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vLookPos);

	_matrix matRotY = XMMatrixRotationY(XMConvertToRadians(m_fQuestAngle));
	_matrix matTarget = m_pNPC->Get_Transform()->Get_WorldMatrix();
	_matrix matWorld = m_pTransform->Get_WorldMatrix() * matRotY * matTarget;

	m_pTransform->Set_WorldMatrix(matWorld);
	m_pTransform->LookAt(vLookAt);

	if (m_fQuestAngle < 120.f)
	{
		m_fQuestAngle += 2.f;
		m_fCamY -= 0.1f;
		if (m_fCamY < 2.f)
			m_fCamY = 2.f;
		m_fQuestFov -= 1.f;
		if (m_fQuestFov < 30.f)
			m_fQuestFov = 30.f;
	}
	else if (m_fQuestAngle >= 120.f && m_fQuestAngle < 400.f)
	{
		m_fQuestAngle += 2.f;
		m_fCamY += 0.1f;
		if (m_fCamY > 7.f)
			m_fCamY = 7.f;
		if (!m_bCamTurn)
			m_fQuestFov += 1.f;
		if (m_fQuestFov > 60.f)
		{
			m_fQuestFov = 60.f;
			m_bCamTurn = true;
		}

	}
	if (m_bCamTurn)
	{
		m_fQuestFov -= 1.f;
		if (m_fQuestFov < 25.f)
			m_fQuestFov = 25.f;
	}

	m_CameraDesc.fFovy = XMConvertToRadians(m_fQuestFov);

}

void CCamera_Dynamic::QuestBattleCam(_float fTimeDelta)
{
	dynamic_cast<CTanjiro*>(m_pPlayer)->Set_Render(true);
	_vector vLookAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLookAt.m128_f32[1] += 2.5f;

	_vector vLookPos = { 0.f,m_fBattleCamY,m_fBattleCamZ,1.f };
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vLookPos);

	_matrix matRotX = XMMatrixRotationX(XMConvertToRadians(m_fBattleAngle));
	_matrix matTarget = m_pTarget->Get_Transform()->Get_WorldMatrix();
	_matrix matWorld = m_pTransform->Get_WorldMatrix() * matRotX * matTarget;

	m_pTransform->Set_WorldMatrix(matWorld);
	m_pTransform->LookAt(vLookAt);

	if (m_fBattleAngle < 20.f)
	{
		m_fBattleAngle += 0.1f;
		m_fBattleCamZ -= 0.3f;
		m_fBattleCamY -= 0.02f;
		if (m_fBattleCamY < 6.5f)
			m_fBattleCamY = 6.5f;

		if (m_fBattleCamZ < 13.f)
			m_fBattleCamZ = 13.f;
	}
	else if (m_fBattleAngle >= 20.f)
	{
		_vector vPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTarget = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vLook2 = vPos - vTarget;

		_float fDist = XMVectorGetX(XMVector3Length(vLook2));

		vPos -= XMVector3Normalize(vLook2) * (fDist * 0.5f);
		vPos.m128_f32[1] = m_fLookAtY;
		XMStoreFloat4(&m_vLerpLook, vPos);
		XMStoreFloat4(&m_vCamPos, m_pTransform->Get_State(CTransform::STATE_TRANSLATION));
		m_bLerp = true;
		dynamic_cast<CTanjiro*>(m_pPlayer)->Set_Render(false);
	}
}

void CCamera_Dynamic::Blur_VeryLow(CRenderer* _pRenderer)
{
	_pRenderer->Set_PointBlur(m_vAtPos, 30.f, 0.3f, 0.3f);
}

void CCamera_Dynamic::Blur_Low(CRenderer* _pRenderer)
{
	_pRenderer->Set_PointBlur(m_vAtPos, 60.f, 1.f, 0.4f);
}

void CCamera_Dynamic::Check_StoryCam()
{
	_vector vPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDist = XMVectorGetX(XMVector3Length(vTargetPos - vPos));

	if (fDist < 40.f)
		m_bStoryBattle = true;
}

_bool CCamera_Dynamic::CutScene(CUTSCENE eCutScene, _float fTimeDelta)
{
	if (CUTSCENE_END <= eCutScene)
		return false;

	if (nullptr != m_pRendererCom)
		m_pRendererCom->MotionBlur((_int)m_vecMotionBlur[eCutScene].x, (_int)m_vecMotionBlur[eCutScene].y);

	switch (m_eCutScene)
	{
	case CUTSCENE_TAN_SPC_1:
	case CUTSCENE_TAN_SPC_2:
	case CUTSCENE_TAN_SPC_3:
	case CUTSCENE_TAN_SPC_4:
		if(false == Play_CutScene(m_vecCamEye[eCutScene], m_vecCamAt[eCutScene], m_vecCamTime[eCutScene], &m_fCurrentCutSceneTime, fTimeDelta))
			Start_CutScene(m_bCutScene, (CUTSCENE)((_int)eCutScene + 1));
		break;
	case CUTSCENE_TAN_SPC_5:	//	끝나면 false
		m_bCutScene = Play_CutScene(m_vecCamEye[eCutScene], m_vecCamAt[eCutScene], m_vecCamTime[eCutScene], &m_fCurrentCutSceneTime, fTimeDelta);
		break;

		//	Rui
	case CUTSCENE_RUI_SPC_START:
	case CUTSCENE_RUI_SPC_0:
	case CUTSCENE_RUI_SPC_1:
	case CUTSCENE_RUI_SPC_2:
	case CUTSCENE_RUI_SPC_3:
	case CUTSCENE_RUI_SPC_4:
	case CUTSCENE_RUI_SPC_5:
		if (false == Play_CutScene(m_vecCamEye[eCutScene], m_vecCamAt[eCutScene], m_vecCamTime[eCutScene], &m_fCurrentCutSceneTime, fTimeDelta))
			Start_CutScene(m_bCutScene, (CUTSCENE)((_int)eCutScene + 1));
		break;
	case CUTSCENE_RUI_SPC_6:
		m_bCutScene = Play_CutScene(m_vecCamEye[eCutScene], m_vecCamAt[eCutScene], m_vecCamTime[eCutScene], &m_fCurrentCutSceneTime, fTimeDelta);
		break;
	}

	if (false == m_bCutScene)
	{
		if (nullptr != m_pRendererCom)
			m_pRendererCom->MotionBlur(0, 0);
	}

	return m_bCutScene;
}

HRESULT CCamera_Dynamic::Ready_CutScene(char * pFileName)
{
	int		iIndex = 0;

	char		szFilePath[MAX_PATH] = "../Bin/Resources/Data/CamActions/";
	strcat_s(szFilePath, pFileName);
	strcat_s(szFilePath, ".cma");

	_tchar		szRealPath[MAX_PATH] = L"";
	MultiByteToWideChar(CP_ACP, 0, szFilePath, (_int)strlen(szFilePath), szRealPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szRealPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Failed to Load : CamAction File");

		return E_FAIL;
	}


	enum CAMTYPE { CAM_EYE, CAM_AT, CAM_END };
	std::vector<_float4>		vecCam[CAM_END];
	std::vector<_float>			vecCamTime;

	DWORD	dwByte = 0;
	_float4*	pCamEye = new _float4;
	_float4*	pCamAt = new _float4;
	_float*		pCamTime = new _float;

	_float2*	pBlur = new _float2;
	ReadFile(hFile, pBlur, sizeof(_float2), &dwByte, nullptr);
	m_vecMotionBlur.push_back(*pBlur);

	while (true)
	{
		ReadFile(hFile, pCamEye, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, pCamAt, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, pCamTime, sizeof(_float), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pCamEye);
			Safe_Delete(pCamAt);
			Safe_Delete(pCamTime);
			Safe_Delete(pBlur);

			vecCamTime.erase(vecCamTime.end() - 1);
			break;
		}
		_float4		vCamEye = *pCamEye;
		_float4		vCamAt = *pCamAt;
		_float		vCamTime = *pCamTime;

		vecCam[CAM_EYE].push_back(vCamEye);
		vecCam[CAM_AT].push_back(vCamAt);
		vecCamTime.push_back(vCamTime);
	}
	CloseHandle(hFile);

	Safe_Delete(pCamEye);
	Safe_Delete(pCamAt);
	Safe_Delete(pCamTime);

	m_vecCamEye.push_back(vecCam[CAM_EYE]);
	m_vecCamAt.push_back(vecCam[CAM_AT]);
	m_vecCamTime.push_back(vecCamTime);

	return S_OK;
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
	Safe_Release(m_pRendererCom);
}
