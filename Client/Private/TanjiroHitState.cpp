#include "stdafx.h"
#include "TanjiroHitState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroUpperHitState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "UI_Manager.h"

using namespace Tanjiro;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
}

CTanjiroState * CHitState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CHitState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pTanjiro->Get_PlayerInfo().iUnicCount < 3 && pTanjiro->Get_PlayerInfo().iUnicBar < pTanjiro->Get_PlayerInfo().iUnicMaxBar)
		{
			pTanjiro->Set_UnicBar(33);
			if (pTanjiro->Get_PlayerInfo().iUnicBar >= pTanjiro->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pTanjiro->Get_PlayerInfo().iUnicCount < 3)
				{
					pTanjiro->Reset_UnicBar();
					pTanjiro->Set_UnicCount(1);
				}
				else
					pTanjiro->Set_UnicBar(pTanjiro->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_HIT);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_HIT);
		m_bReset = true;
	}


	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pTanjiro->Get_NavigationCom());

	}


	//	_vector vPlayerY = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//
	//	vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
	//	if (vPlayerY.m128_f32[1] < 0)
	//		vPlayerY.m128_f32[1] = 0;

	//	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	//	if (fHitTime >= 35.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//	{
	//		if (!m_bTrun)
	//		{
	//			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//			_vector vLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	//			vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
	//			if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos))
	//				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pTanjiro->Get_NavigationCom());
	//			else
	//			{
	//				m_bTrun = true;
	//			}
	//		}
	//		if (m_bTrun)
	//		{
	//			pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pTanjiro->Get_NavigationCom());
	//		}
	//	}
	//	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_HIT))
	//	{
	//		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_HIT);
	//		//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_HIT);

	//		pTanjiro->Get_Model()->Set_CurrentAnimIndex(58);
	//		pTanjiro->Get_Model()->Set_Loop(58);
	//		pTanjiro->Get_Model()->Set_LinearTime(58, 0.01f);
	//		pTanjiro->Set_bGuard(true);
	//	}

	//	if (pTanjiro->Get_Model()->Get_End(58))
	//	{
	//		pTanjiro->Get_Model()->Reset_Anim(58);
	//		pTanjiro->Get_Model()->Set_End(58);
	//		pTanjiro->Set_bGuard(false);
	//		return new CIdleState();
	//	}
	//}


	//m_fCurrentDuration += (1.f / 60.f);
	//if (m_fCurrentDuration >= 2.f)
	//	g_bDeathTime = false;

	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}


	if (pTanjiro->Get_AnimIndex() == CTanjiro::ANIM_DEAD)
	{
		if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_DEAD))
		{
			//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_DEAD);
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_RETURN_1);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN_1, 0.01f);
			pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_DEAD);
		}
	}
	else if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_HIT_DMG_RETURN_1))
	{
		//pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_HIT_DMG_RETURN_1); 
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_HIT_DMG_RETURN_1);
		return new CIdleState(STATE_HIT);
	}

	else
	{
		if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState(STATE_HIT);
		}
	}





	return nullptr;
}

CTanjiroState * CHitState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	//m_fJumpTime += 0.035f;
	//if (m_bJumpHit && !m_bJump)
	//{
	//	Jump(pTanjiro, m_fJumpTime);
	//}


	if (pTanjiro->Get_BattleTarget()->Get_SplSkl() == true)
		fTimeDelta *= 0.3f;

	if (pTanjiro->Get_AnimIndex() == CTanjiro::ANIMID::ANIM_HIT_DMG2_G)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);





	return nullptr;
}

void CHitState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_HIT;

	if (pTanjiro->Get_PlayerInfo().iHp <= 0)
	{
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIMID::ANIM_DEAD);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_DEAD);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_DEAD);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.05f);
		pTanjiro->Set_GodMode(true);
		CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pTanjiro->Get_Renderer());
		RELEASE_INSTANCE(CGameInstance);
		g_bDeathTime = true;
	}

	else
	{
		if (pTanjiro->Get_NavigationHeight().y < XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
		{
			m_bHitPlayerJump = true;
			Set_JumpHitState(pTanjiro);
		}
		else
		{
			m_bHitPlayerJump = false;
			Set_HitState(pTanjiro);
		}
	}

	_uint iRand = rand() % 3;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Hit1_1.wav"), fVOICE);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Hit1_2.wav"), fVOICE);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Hit1_3.wav"), fVOICE);
}




CTanjiroState * CHitState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pTanjiro->Get_NavigationHeight().y;
	pTanjiro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pTanjiro->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Set_HitState(CTanjiro * pTanjiro)
{
	_int iHit = pTanjiro->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 3);
	int iRandom = RandomPattern(gen);

	if (iHit >= 12)
	{

		switch (iRandom)
		{
		case 1:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_F);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_F);
			pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
			pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
			break;
		case 2:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_L);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_L);
			pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
			pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
			break;
		case 3:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_R);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_R);
			pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
			pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
			break;
		}
	}

	else
	{

		switch (iHit)
		{
		case 3: // atk 1
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_F);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_F);
			pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
			pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
			break;
		case 4: // atk 2
			if (pTanjiro->Get_Atk2() == false)
			{
				pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_L);
				pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_L);
				pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
				pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
				pTanjiro->Set_Atk2(true);
			}
			else if (pTanjiro->Get_Atk2() == true)
			{
				pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_R);
				pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_R);
				pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
				pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
				pTanjiro->Set_Atk2(false);
			}
			break;
		case 5: // atk 3
			if (pTanjiro->Get_Atk2() == true)
			{
				pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_F);
				pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_F);
				pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
				pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
				pTanjiro->Set_Atk2(false);
				break;
			}
			else
			{
				pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG2_G);
				pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG2_G);
				pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
				pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
				pTanjiro->Set_Atk2(false);
			}
			break;
		case 6: // atk 4
			if (pTanjiro->Get_Atk2() == true)
			{
				std::random_device RandomDevice;
				std::mt19937 gen(RandomDevice());
				std::uniform_int_distribution<int> RandomPattern(1, 3);
				int iRandom = RandomPattern(gen);
				switch (iRandom)
				{
				case 1:
					pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_F);
					pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_F);
					pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
					pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
					break;
				case 2:
					pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_L);
					pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_L);
					pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
					pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
					break;
				case 3:
					pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_R);
					pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_R);
					pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
					pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
					break;
				}
			}
			else
			{
				pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG2_F);
				pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG2_F);
				pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
				pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
			}
			break;
		default:
			break;
		}

	}


}
void CHitState::Set_JumpHitState(CTanjiro * pTanjiro)
{

	_int iHit = pTanjiro->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);


	switch (iRandom)
	{
	case 1:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_AF);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_AF);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
		break;
	case 2:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_AL);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_AL);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
		break;
	case 3:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_AR);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_AR);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
		break;
	case 4:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_HIT_DMG_AU);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_HIT_DMG_AU);
		pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());
		pTanjiro->Get_Model()->Set_LinearTime(pTanjiro->Get_AnimIndex(), 0.2f);
		break;
	}
}

void CHitState::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Set_HitTime(0.3f);
	//pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
}



