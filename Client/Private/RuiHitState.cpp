#include "stdafx.h"
#include "RuiHitState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "RuiUpperHitState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"


using namespace Rui;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
}

CRuiState * CHitState::HandleInput(CRui* pRui)
{

	return nullptr;
}

CRuiState * CHitState::Tick(CRui* pRui, _float fTimeDelta)
{


	if (!m_bReset)
	{
		if (pRui->Get_PlayerInfo().iUnicCount < 3 && pRui->Get_PlayerInfo().iUnicBar < pRui->Get_PlayerInfo().iUnicMaxBar)
		{
			pRui->Set_UnicBar(33);
			if (pRui->Get_PlayerInfo().iUnicBar >= pRui->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pRui->Get_PlayerInfo().iUnicCount < 3)
				{
					pRui->Reset_UnicBar();
					pRui->Set_UnicCount(1);
				}
				else
					pRui->Set_UnicBar(pRui->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		pRui->Get_Model()->Reset_Anim(CRui::ANIM_HIT);
		pRui->Get_Model()->Set_Loop(CRui::ANIM_HIT);
		m_bReset = true;
	}


	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pRui->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pRui->Get_NavigationCom());

	}


	//	_vector vPlayerY = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//
	//	vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
	//	if (vPlayerY.m128_f32[1] < 0)
	//		vPlayerY.m128_f32[1] = 0;

	//	pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	//	if (fHitTime >= 35.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//	{
	//		if (!m_bTrun)
	//		{
	//			_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//			_vector vLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	//			vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
	//			if (pRui->Get_NavigationCom()->Cheak_Cell(vPos))
	//				pRui->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pRui->Get_NavigationCom());
	//			else
	//			{
	//				m_bTrun = true;
	//			}
	//		}
	//		if (m_bTrun)
	//		{
	//			pRui->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pRui->Get_NavigationCom());
	//		}
	//	}
	//	if (pRui->Get_Model()->Get_End(CRui::ANIM_HIT))
	//	{
	//		pRui->Get_Model()->Set_End(CRui::ANIM_HIT);
	//		//pRui->Get_Model()->Reset_Anim(CRui::ANIM_HIT);

	//		pRui->Get_Model()->Set_CurrentAnimIndex(58);
	//		pRui->Get_Model()->Set_Loop(58);
	//		pRui->Get_Model()->Set_LinearTime(58, 0.01f);
	//		pRui->Set_bGuard(true);
	//	}

	//	if (pRui->Get_Model()->Get_End(58))
	//	{
	//		pRui->Get_Model()->Reset_Anim(58);
	//		pRui->Get_Model()->Set_End(58);
	//		pRui->Set_bGuard(false);
	//		return new CIdleState();
	//	}
	//}

	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}

	if (pRui->Get_AnimIndex() == CRui::ANIM_DEAD)
	{
		if (pRui->Get_Model()->Get_End(CRui::ANIM_DEAD))
		{
			pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_RETURN_1);
			pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_HIT_DMG_RETURN_1, 0.01f);
			pRui->Get_Model()->Set_End(CRui::ANIM_DEAD);
		}
	}
	else if (pRui->Get_Model()->Get_End(CRui::ANIM_HIT_DMG_RETURN_1))
	{
		pRui->Get_Model()->Set_End(CRui::ANIM_HIT_DMG_RETURN_1);
		return new CIdleState(STATE_HIT);
	}

	else
	{
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState(STATE_HIT);
		}

	}



	return nullptr;
}

CRuiState * CHitState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	//m_fJumpTime += 0.035f;
	//if (m_bJumpHit && !m_bJump)
	//{
	//	Jump(pRui, m_fJumpTime);
	//}

	if (pRui->Get_BattleTarget()->Get_SplSkl() == true)
		fTimeDelta *= 0.3f;

	if (pRui->Get_AnimIndex() == CRui::ANIMID::ANIM_HIT_DMG2_G)
		pRui->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pRui->Get_Model()->Play_Animation(fTimeDelta);





	return nullptr;
}

void CHitState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_HIT;


	if (pRui->Get_PlayerInfo().iHp <= 0)
	{
		pRui->Get_Model()->Reset_Anim(CRui::ANIMID::ANIM_DEAD);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_DEAD);
		pRui->Set_AnimIndex(CRui::ANIM_DEAD);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.05f);
		pRui->Set_GodMode(true);
		CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(g_iLevel, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pRui->Get_Renderer());
		RELEASE_INSTANCE(CGameInstance);
		g_bDeathTime = true;
	}
	else
	{
		if (pRui->Get_NavigationHeight().y + 0.1f < XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
		{
			m_bHitPlayerJump = true;
			Set_JumpHitState(pRui);
		}
		else
		{
			m_bHitPlayerJump = false;
			Set_HitState(pRui);
		}
	}

	pRui->Set_RuiHit(true);


	_uint iRand = rand() % 3;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Hit_1.wav"), g_fVoice);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Hit_2.wav"), g_fVoice);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Rui_Hit_3.wav"), g_fVoice);
}




CRuiState * CHitState::Jump(CRui* pRui, _float fTimeDelta)
{
	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pRui->Get_NavigationHeight().y;
	pRui->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pRui->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Set_HitState(CRui* pRui)
{
	_int iHit = pRui->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 3);
	int iRandom = RandomPattern(gen);

	if (iHit >= 12)
	{

		switch (iRandom)
		{
		case 1:
			pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_F);
			pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_F);
			pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
			pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
			break;
		case 2:
			pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_L);
			pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_L);
			pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
			pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
			break;
		case 3:
			pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_R);
			pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_R);
			pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
			pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
			break;
		}
	}

	else
	{

		switch (iHit)
		{
		case 3: // atk 1
			pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_F);
			pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_F);
			pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
			pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
			break;
		case 4: // atk 2
			if (pRui->Get_Atk2() == false)
			{
				pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_L);
				pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_L);
				pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
				pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
				pRui->Set_Atk2(true);
			}
			else if (pRui->Get_Atk2() == true)
			{
				pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_R);
				pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_R);
				pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
				pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
				pRui->Set_Atk2(false);
			}
			break;
		case 5: // atk 3
			if (pRui->Get_Atk2() == true)
			{
				pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_F);
				pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_F);
				pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
				pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
				pRui->Set_Atk2(false);
				break;
			}
			else
			{
				pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG2_G);
				pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG2_G);
				pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
				pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
				pRui->Set_Atk2(false);
			}
			break;
		case 6: // atk 4
			if (pRui->Get_Atk2() == true)
			{
				std::random_device RandomDevice;
				std::mt19937 gen(RandomDevice());
				std::uniform_int_distribution<int> RandomPattern(1, 3);
				int iRandom = RandomPattern(gen);

				switch (iRandom)
				{
				case 1:
					pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_F);
					pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_F);
					pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
					pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
					break;
				case 2:
					pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_L);
					pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_L);
					pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
					pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
					break;
				case 3:
					pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_R);
					pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_R);
					pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
					pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
					break;
				}
			}
			else
			{
				pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG2_F);
				pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG2_F);
				pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
				pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
			}
			break;
		default:
			break;
		}

	}

	if (pRui->Get_BattleTarget()->Get_PlayerInfo().bOni == false)
	{
		if (iHit < 12 && pRui->Get_BattleTarget()->Get_SubChar()->Get_Change() == false)
		{
			std::random_device RandomDevice;
			std::mt19937 gen(RandomDevice());
			std::uniform_int_distribution<int> RandomPattern(1, 3);
			int iRandom = RandomPattern(gen);

			switch (iRandom)
			{
			case 1:
				pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_F);
				pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_F);
				pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
				pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
				break;
			case 2:
				pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_L);
				pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_L);
				pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
				pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
				break;
			case 3:
				pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_R);
				pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_R);
				pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
				pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
				break;
			}
		}
	}
}
void CHitState::Set_JumpHitState(CRui* pRui)
{

	_int iHit = pRui->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);


	switch (iRandom)
	{
	case 1:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_AF);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_AF);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	case 2:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_AL);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_AL);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	case 3:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_AR);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_AR);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	case 4:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_HIT_DMG_AU);
		pRui->Set_AnimIndex(CRui::ANIM_HIT_DMG_AU);
		pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());
		pRui->Get_Model()->Set_LinearTime(pRui->Get_AnimIndex(), 0.2f);
		break;
	}
}
void CHitState::Exit(CRui* pRui)
{
	pRui->Set_HitTime(0.3f);
	//pRui->Get_Model()->Reset_Anim(pRui->Get_AnimIndex());
}



