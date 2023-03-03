#include "stdafx.h"
#include "NezukoHitState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "NezukoUpperHitState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"

using namespace Nezuko;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
}

CNezukoState * CHitState::HandleInput(CNezuko* pNezuko)
{

	return nullptr;
}

CNezukoState * CHitState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pNezuko->Get_PlayerInfo().iUnicCount < 3 && pNezuko->Get_PlayerInfo().iUnicBar < pNezuko->Get_PlayerInfo().iUnicMaxBar)
		{
			pNezuko->Set_UnicBar(33);
			if (pNezuko->Get_PlayerInfo().iUnicBar >= pNezuko->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pNezuko->Get_PlayerInfo().iUnicCount < 3)
				{
					pNezuko->Reset_UnicBar();
					pNezuko->Set_UnicCount(1);
				}
				else
					pNezuko->Set_UnicBar(pNezuko->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIM_HIT);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_HIT);
		m_bReset = true;
	}


	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pNezuko->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pNezuko->Get_NavigationCom());

	}


	//	_vector vPlayerY = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//
	//	vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
	//	if (vPlayerY.m128_f32[1] < 0)
	//		vPlayerY.m128_f32[1] = 0;

	//	pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	//	if (fHitTime >= 35.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//	{
	//		if (!m_bTrun)
	//		{
	//			_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//			_vector vLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	//			vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
	//			if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPos))
	//				pNezuko->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pNezuko->Get_NavigationCom());
	//			else
	//			{
	//				m_bTrun = true;
	//			}
	//		}
	//		if (m_bTrun)
	//		{
	//			pNezuko->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pNezuko->Get_NavigationCom());
	//		}
	//	}
	//	if (pNezuko->Get_Model()->Get_End(CNezuko::ANIM_HIT))
	//	{
	//		pNezuko->Get_Model()->Set_End(CNezuko::ANIM_HIT);
	//		//pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIM_HIT);

	//		pNezuko->Get_Model()->Set_CurrentAnimIndex(58);
	//		pNezuko->Get_Model()->Set_Loop(58);
	//		pNezuko->Get_Model()->Set_LinearTime(58, 0.01f);
	//		pNezuko->Set_bGuard(true);
	//	}

	//	if (pNezuko->Get_Model()->Get_End(58))
	//	{
	//		pNezuko->Get_Model()->Reset_Anim(58);
	//		pNezuko->Get_Model()->Set_End(58);
	//		pNezuko->Set_bGuard(false);
	//		return new CIdleState();
	//	}
	//}
	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}
	if (pNezuko->Get_AnimIndex() == CNezuko::ANIM_DEAD)
	{
		if (pNezuko->Get_Model()->Get_End(CNezuko::ANIM_DEAD))
		{
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_RETURN_1);
			pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIMID::ANIM_HIT_DMG_RETURN_1, 0.01f);
			pNezuko->Get_Model()->Set_End(CNezuko::ANIM_DEAD);
		}
	}
	else if (pNezuko->Get_Model()->Get_End(CNezuko::ANIM_HIT_DMG_RETURN_1))
	{
		pNezuko->Get_Model()->Set_End(CNezuko::ANIM_HIT_DMG_RETURN_1);
		return new CIdleState(STATE_HIT);
	}

	else
	{
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState(STATE_HIT);
		}

	}



	return nullptr;
}

CNezukoState * CHitState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	//m_fJumpTime += 0.035f;
	//if (m_bJumpHit && !m_bJump)
	//{
	//	Jump(pNezuko, m_fJumpTime);
	//}

	if (pNezuko->Get_BattleTarget()->Get_SplSkl() == true)
		fTimeDelta *= 0.3f;

	if (pNezuko->Get_AnimIndex() == CNezuko::ANIMID::ANIM_HIT_DMG2_G)
		pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pNezuko->Get_Model()->Play_Animation(fTimeDelta);





	return nullptr;
}

void CHitState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_HIT;


	if (pNezuko->Get_PlayerInfo().iHp <= 0)
	{
		pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIMID::ANIM_DEAD);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_DEAD);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_DEAD);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.05f);
		pNezuko->Set_GodMode(true);
		CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pNezuko->Get_Renderer());
		RELEASE_INSTANCE(CGameInstance);
		g_bDeathTime = true;
	}
	else
	{

		if (pNezuko->Get_NavigationHeight().y < XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
		{
			m_bHitPlayerJump = true;
			Set_JumpHitState(pNezuko);
		}
		else
		{
			m_bHitPlayerJump = false;
			Set_HitState(pNezuko);
		}

	}

	_uint iRand = rand() % 2;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Nezuko_Hit_1.wav"), g_fVoice);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Nezuko_Hit_2.wav"), g_fVoice);

}

CNezukoState * CHitState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pNezuko->Get_NavigationHeight().y;
	pNezuko->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pNezuko->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Set_HitState(CNezuko* pNezuko)
{
	_int iHit = pNezuko->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 3);
	int iRandom = RandomPattern(gen);

	if (iHit >= 12)
	{

		switch (iRandom)
		{
		case 1:
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_F);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_F);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			break;
		case 2:
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_L);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_L);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			break;
		case 3:
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_R);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_R);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			break;
		}
	}

	else
	{

		switch (iHit)
		{
		case 3: // atk 1
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_F);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_F);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			break;
		case 4: // atk 2
			if (pNezuko->Get_Atk2() == false)
			{
				pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_L);
				pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_L);
				pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
				pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
				pNezuko->Set_Atk2(true);
			}
			else if (pNezuko->Get_Atk2() == true)
			{
				pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_R);
				pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_R);
				pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
				pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
				pNezuko->Set_Atk2(false);
			}
			break;
		case 5: // atk 3
			if (pNezuko->Get_Atk2() == true)
			{
				pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_F);
				pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_F);
				pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
				pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
				pNezuko->Set_Atk2(false);
				break;
			}
			else
			{
				pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG2_G);
				pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG2_G);
				pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
				pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
				pNezuko->Set_Atk2(false);
			}
			break;
		case 6: // atk 4
			if (pNezuko->Get_Atk2() == true)
			{
				std::random_device RandomDevice;
				std::mt19937 gen(RandomDevice());
				std::uniform_int_distribution<int> RandomPattern(1, 3);
				int iRandom = RandomPattern(gen);
				switch (iRandom)
				{
				case 1:
					pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_F);
					pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_F);
					pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
					pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
					break;
				case 2:
					pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_L);
					pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_L);
					pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
					pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
					break;
				case 3:
					pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_R);
					pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_R);
					pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
					pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
					break;
				}
			}
			else
			{
				pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG2_F);
				pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG2_F);
				pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
				pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			}
			break;
		default:
			break;
		}

	}


	if (iHit < 12 && pNezuko->Get_BattleTarget()->Get_SubChar()->Get_Change() == false)
	{
		std::random_device RandomDevice;
		std::mt19937 gen(RandomDevice());
		std::uniform_int_distribution<int> RandomPattern(1, 3);
		int iRandom = RandomPattern(gen);

		switch (iRandom)
		{
		case 1:
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_F);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_F);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			break;
		case 2:
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_L);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_L);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			break;
		case 3:
			pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_R);
			pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_R);
			pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
			pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
			break;
		}
	}
}
void CHitState::Set_JumpHitState(CNezuko* pNezuko)
{

	_int iHit = pNezuko->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);


	switch (iRandom)
	{
	case 1:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_AF);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_AF);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	case 2:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_AL);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_AL);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	case 3:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_AR);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_AR);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	case 4:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT_DMG_AU);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT_DMG_AU);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.2f);
		break;
	}
}
void CHitState::Exit(CNezuko* pNezuko)
{
	pNezuko->Set_HitTime(0.3f);
	//pNezuko->Get_Model()->Reset_Anim(pNezuko->Get_AnimIndex());
}



