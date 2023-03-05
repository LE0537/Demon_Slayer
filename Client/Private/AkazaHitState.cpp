#include "stdafx.h"
#include "AkazaHitState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "AkazaUpperHitState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
using namespace Akaza;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
}

CAkazaState * CHitState::HandleInput(CAkaza* pAkaza)
{

	return nullptr;
}

CAkazaState * CHitState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pAkaza->Get_PlayerInfo().iUnicCount < 3 && pAkaza->Get_PlayerInfo().iUnicBar < pAkaza->Get_PlayerInfo().iUnicMaxBar)
		{
			pAkaza->Set_UnicBar(33);
			if (pAkaza->Get_PlayerInfo().iUnicBar >= pAkaza->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pAkaza->Get_PlayerInfo().iUnicCount < 3)
				{
					pAkaza->Reset_UnicBar();
					pAkaza->Set_UnicCount(1);
				}
				else
					pAkaza->Set_UnicBar(pAkaza->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_HIT);
		m_bReset = true;
	}


	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pAkaza->Get_NavigationCom());

	}


	//	_vector vPlayerY = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//
	//	vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
	//	if (vPlayerY.m128_f32[1] < 0)
	//		vPlayerY.m128_f32[1] = 0;

	//	pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	//	if (fHitTime >= 35.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//	{
	//		if (!m_bTrun)
	//		{
	//			_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//			_vector vLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	//			vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
	//			if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPos))
	//				pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pAkaza->Get_NavigationCom());
	//			else
	//			{
	//				m_bTrun = true;
	//			}
	//		}
	//		if (m_bTrun)
	//		{
	//			pAkaza->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pAkaza->Get_NavigationCom());
	//		}
	//	}
	//	if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_HIT))
	//	{
	//		pAkaza->Get_Model()->Set_End(CAkaza::ANIM_HIT);
	//		//pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);

	//		pAkaza->Get_Model()->Set_CurrentAnimIndex(58);
	//		pAkaza->Get_Model()->Set_Loop(58);
	//		pAkaza->Get_Model()->Set_LinearTime(58, 0.01f);
	//		pAkaza->Set_bGuard(true);
	//	}

	//	if (pAkaza->Get_Model()->Get_End(58))
	//	{
	//		pAkaza->Get_Model()->Reset_Anim(58);
	//		pAkaza->Get_Model()->Set_End(58);
	//		pAkaza->Set_bGuard(false);
	//		return new CIdleState();
	//	}
	//}
	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}
	if (pAkaza->Get_AnimIndex() == CAkaza::ANIM_DEAD)
	{
		if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_DEAD))
		{
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_RETURN_1);
			pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIMID::ANIM_HIT_DMG_RETURN_1, 0.01f);
			pAkaza->Get_Model()->Set_End(CAkaza::ANIM_DEAD);
		}
	}
	else if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_HIT_DMG_RETURN_1))
	{
		pAkaza->Get_Model()->Set_End(CAkaza::ANIM_HIT_DMG_RETURN_1);

		return new CIdleState(STATE_HIT);
	}

	else
	{
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState(STATE_HIT);
		}
	}




	return nullptr;
}

CAkazaState * CHitState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	//m_fJumpTime += 0.035f;
	//if (m_bJumpHit && !m_bJump)
	//{
	//	Jump(pAkaza, m_fJumpTime);
	//}

	if (pAkaza->Get_BattleTarget()->Get_SplSkl() == true)
		fTimeDelta *= 0.3f;

	if (pAkaza->Get_AnimIndex() == CAkaza::ANIMID::ANIM_HIT_DMG2_G)
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta);





	return nullptr;
}

void CHitState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_HIT;

	if (pAkaza->Get_PlayerInfo().iHp <= 0)
	{
		pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIMID::ANIM_DEAD);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_DEAD);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_DEAD);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.05f);
		pAkaza->Set_GodMode(true);
		CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pAkaza->Get_Renderer());
		RELEASE_INSTANCE(CGameInstance);
		g_bDeathTime = true;
	}
	else
	{
		if (pAkaza->Get_NavigationHeight().y + 0.1f < XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
		{
			m_bHitPlayerJump = true;
			Set_JumpHitState(pAkaza);
		}
		else
		{
			m_bHitPlayerJump = false;
			Set_HitState(pAkaza);
		}
	}



	_uint iRand = rand() % 3;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_Hit_1.wav"), g_fVoice);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_Hit_2.wav"), g_fVoice);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_Hit_3.wav"), g_fVoice);
}




CAkazaState * CHitState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pAkaza->Get_NavigationHeight().y;
	pAkaza->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pAkaza->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Set_HitState(CAkaza* pAkaza)
{
	_int iHit = pAkaza->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 3);
	int iRandom = RandomPattern(gen);

	if (iHit >= 12)
	{

		switch (iRandom)
		{
		case 1:
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_F);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_F);
			pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
			pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.05f);
			break;
		case 2:
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_L);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_L);
			pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
			pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.05f);
			break;
		case 3:
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_R);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_R);
			pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
			pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.05f);
			break;
		}
	}

	else
	{

		switch (iHit)
		{
		case 3: // atk 1
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_F);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_F);
			pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
			pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
			break;
		case 4: // atk 2
			if (pAkaza->Get_Atk2() == false)
			{
				pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_L);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_L);
				pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
				pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
				pAkaza->Set_Atk2(true);
			}
			else if (pAkaza->Get_Atk2() == true)
			{
				pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_R);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_R);
				pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
				pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
				pAkaza->Set_Atk2(false);
			}
			break;
		case 5: // atk 3
			if (pAkaza->Get_Atk2() == true)
			{
				pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_F);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_F);
				pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
				pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
				pAkaza->Set_Atk2(false);
				break;
			}
			else
			{
				pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG2_G);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG2_G);
				pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
				pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
				pAkaza->Set_Atk2(false);
			}
			break;
		case 6: // atk 4
			if (pAkaza->Get_Atk2() == true)
			{
				std::random_device RandomDevice;
				std::mt19937 gen(RandomDevice());
				std::uniform_int_distribution<int> RandomPattern(1, 3);
				int iRandom = RandomPattern(gen);
				switch (iRandom)
				{
				case 1:
					pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_F);
					pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_F);
					pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
					pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.05f);
					break;
				case 2:
					pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_L);
					pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_L);
					pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
					pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.05f);
					break;
				case 3:
					pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_R);
					pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_R);
					pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
					pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.05f);
					break;
				}
			}
			else
			{
				pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG2_F);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG2_F);
				pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
				pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
			}
			break;
		default:
			break;
		}

	}


	if (pAkaza->Get_BattleTarget()->Get_PlayerInfo().bOni == false)
	{
		if (iHit < 12 && pAkaza->Get_BattleTarget()->Get_SubChar()->Get_Change() == false)
		{
			std::random_device RandomDevice;
			std::mt19937 gen(RandomDevice());
			std::uniform_int_distribution<int> RandomPattern(1, 3);
			int iRandom = RandomPattern(gen);

			switch (iRandom)
			{
			case 1:
				pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_F);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_F);
				pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
				pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
				break;
			case 2:
				pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_L);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_L);
				pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
				pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
				break;
			case 3:
				pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_R);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_R);
				pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
				pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
				break;
			}
		}
	}
}
void CHitState::Set_JumpHitState(CAkaza* pAkaza)
{

	_int iHit = pAkaza->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);


	switch (iRandom)
	{
	case 1:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_AF);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_AF);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	case 2:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_AL);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_AL);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	case 3:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_AR);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_AR);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	case 4:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_AU);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_AU);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	}
}
void CHitState::Exit(CAkaza* pAkaza)
{
	pAkaza->Set_HitTime(0.3f);
	pAkaza->Get_Model()->Reset_Anim(pAkaza->Get_AnimIndex());
}



