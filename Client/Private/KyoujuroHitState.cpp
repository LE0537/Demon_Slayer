#include "stdafx.h"
#include "KyoujuroHitState.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroUpperHitState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"

using namespace Kyoujuro;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
}

CKyoujuroState * CHitState::HandleInput(CKyoujuro* pKyoujuro)
{

	return nullptr;
}

CKyoujuroState * CHitState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3 && pKyoujuro->Get_PlayerInfo().iUnicBar < pKyoujuro->Get_PlayerInfo().iUnicMaxBar)
		{
			pKyoujuro->Set_UnicBar(33);
			if (pKyoujuro->Get_PlayerInfo().iUnicBar >= pKyoujuro->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pKyoujuro->Get_PlayerInfo().iUnicCount < 3)
				{
					pKyoujuro->Reset_UnicBar();
					pKyoujuro->Set_UnicCount(1);
				}
				else
					pKyoujuro->Set_UnicBar(pKyoujuro->Get_PlayerInfo().iUnicMaxBar);
			}
			pKyoujuro->Get_BattleTarget()->Set_UnicBar(16);
			if (pKyoujuro->Get_BattleTarget()->Get_PlayerInfo().iUnicBar >= pKyoujuro->Get_BattleTarget()->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pKyoujuro->Get_BattleTarget()->Get_PlayerInfo().iUnicCount < 3)
				{
					pKyoujuro->Get_BattleTarget()->Reset_UnicBar();
					pKyoujuro->Get_BattleTarget()->Set_UnicCount(1);
				}
				else
					pKyoujuro->Get_BattleTarget()->Set_UnicBar(pKyoujuro->Get_BattleTarget()->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_HIT);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_HIT);
		m_bReset = true;
	}


	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pKyoujuro->Get_NavigationCom());

	}


	//	_vector vPlayerY = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//
	//	vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
	//	if (vPlayerY.m128_f32[1] < 0)
	//		vPlayerY.m128_f32[1] = 0;

	//	pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	//	if (fHitTime >= 35.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//	{
	//		if (!m_bTrun)
	//		{
	//			_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//			_vector vLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	//			vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
	//			if (pKyoujuro->Get_NavigationCom()->Cheak_Cell(vPos))
	//				pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pKyoujuro->Get_NavigationCom());
	//			else
	//			{
	//				m_bTrun = true;
	//			}
	//		}
	//		if (m_bTrun)
	//		{
	//			pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pKyoujuro->Get_NavigationCom());
	//		}
	//	}
	//	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_HIT))
	//	{
	//		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_HIT);
	//		//pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_HIT);

	//		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(58);
	//		pKyoujuro->Get_Model()->Set_Loop(58);
	//		pKyoujuro->Get_Model()->Set_LinearTime(58, 0.01f);
	//		pKyoujuro->Set_bGuard(true);
	//	}

	//	if (pKyoujuro->Get_Model()->Get_End(58))
	//	{
	//		pKyoujuro->Get_Model()->Reset_Anim(58);
	//		pKyoujuro->Get_Model()->Set_End(58);
	//		pKyoujuro->Set_bGuard(false);
	//		return new CIdleState();
	//	}
	//}

	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}


	if (pKyoujuro->Get_AnimIndex() == CKyoujuro::ANIM_DEAD)
	{
		if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_DEAD))
		{
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_RETURN_1);
			pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_HIT_DMG_RETURN_1, 0.01f);
			pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_DEAD);
		}
	}
	else if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_HIT_DMG_RETURN_1))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_HIT_DMG_RETURN_1);
		return new CIdleState(STATE_HIT);
	}

	else
	{
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState(STATE_HIT);
		}

	}



	return nullptr;
}

CKyoujuroState * CHitState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	//m_fJumpTime += 0.035f;
	//if (m_bJumpHit && !m_bJump)
	//{
	//	Jump(pKyoujuro, m_fJumpTime);
	//}

	if (pKyoujuro->Get_BattleTarget()->Get_SplSkl() == true)
		fTimeDelta *= 0.3f;

	if (pKyoujuro->Get_AnimIndex() == CKyoujuro::ANIMID::ANIM_HIT_DMG2_G)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);





	return nullptr;
}

void CHitState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_HIT;

	if (pKyoujuro->Get_PlayerInfo().iHp <= 0)
	{
		pKyoujuro->Get_Model()->Reset_Anim(CKyoujuro::ANIMID::ANIM_DEAD);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_DEAD);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_DEAD);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.05f);
		pKyoujuro->Set_GodMode(true);
		CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pKyoujuro->Get_Renderer());
		RELEASE_INSTANCE(CGameInstance);
		g_bDeathTime = true;
	}
	else
	{
		if (pKyoujuro->Get_NavigationHeight().y + 0.1f < XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
		{
			m_bHitPlayerJump = true;
			Set_JumpHitState(pKyoujuro);
		}
		else
		{
			m_bHitPlayerJump = false;
			Set_HitState(pKyoujuro);
		}
	}

	_uint iRand = rand() % 2;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_Hit_1.wav"), g_fVoice);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_Hit_2.wav"), g_fVoice);

}




CKyoujuroState * CHitState::Jump(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pKyoujuro->Get_NavigationHeight().y;
	pKyoujuro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pKyoujuro->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Set_HitState(CKyoujuro* pKyoujuro)
{
	_int iHit = pKyoujuro->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 3);
	int iRandom = RandomPattern(gen);

	if (iHit >= 12)
	{
		switch (iRandom)
		{
		case 1:
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_F);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_F);
			pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
			pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
			break;
		case 2:
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_L);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_L);
			pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
			pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
			break;
		case 3:
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_R);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_R);
			pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
			pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
			break;
		}
	}

	else
	{

		switch (iHit)
		{
		case 3: // atk 1
			pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_F);
			pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_F);
			pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
			pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
			break;
		case 4: // atk 2
			if (pKyoujuro->Get_Atk2() == false)
			{
				pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_L);
				pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_L);
				pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
				pKyoujuro->Set_Atk2(true);
			}
			else if (pKyoujuro->Get_Atk2() == true)
			{
				pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_R);
				pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_R);
				pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
				pKyoujuro->Set_Atk2(false);
			}
			break;
		case 5: // atk 3
			if (pKyoujuro->Get_Atk2() == true)
			{
				pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_F);
				pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_F);
				pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
				pKyoujuro->Set_Atk2(false);
				break;
			}
			else
			{
				pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG2_G);
				pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG2_G);
				pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
				pKyoujuro->Set_Atk2(false);
			}
			break;
		case 6: // atk 4
			if (pKyoujuro->Get_Atk2() == true)
			{
				std::random_device RandomDevice;
				std::mt19937 gen(RandomDevice());
				std::uniform_int_distribution<int> RandomPattern(1, 3);
				int iRandom = RandomPattern(gen);
				switch (iRandom)
				{
				case 1:
					pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_F);
					pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_F);
					pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
					pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
					break;
				case 2:
					pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_L);
					pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_L);
					pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
					pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
					break;
				case 3:
					pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_R);
					pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_R);
					pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
					pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
					break;
				}
			}
			else
			{
				pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG2_F);
				pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG2_F);
				pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
			}
			break;
		default:
			break;
		}

	}

	if (pKyoujuro->Get_BattleTarget()->Get_PlayerInfo().bOni == false)
	{
		if (iHit < 12 && pKyoujuro->Get_BattleTarget()->Get_SubChar()->Get_Change() == false)
		{
			std::random_device RandomDevice;
			std::mt19937 gen(RandomDevice());
			std::uniform_int_distribution<int> RandomPattern(1, 3);
			int iRandom = RandomPattern(gen);

			switch (iRandom)
			{
			case 1:
				pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_F);
				pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_F);
				pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
				break;
			case 2:
				pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_L);
				pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_L);
				pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
				break;
			case 3:
				pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_R);
				pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_R);
				pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
				pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
				break;
			}
		}
	}
}
void CHitState::Set_JumpHitState(CKyoujuro* pKyoujuro)
{

	_int iHit = pKyoujuro->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);


	switch (iRandom)
	{
	case 1:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_AF);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_AF);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		break;
	case 2:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_AL);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_AL);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		break;
	case 3:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_AR);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_AR);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		break;
	case 4:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_HIT_DMG_AU);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_HIT_DMG_AU);
		pKyoujuro->Get_Model()->Set_Loop(pKyoujuro->Get_AnimIndex());
		pKyoujuro->Get_Model()->Set_LinearTime(pKyoujuro->Get_AnimIndex(), 0.2f);
		break;
	}
}
void CHitState::Exit(CKyoujuro* pKyoujuro)
{
	pKyoujuro->Set_HitTime(0.3f);
	//pKyoujuro->Get_Model()->Reset_Anim(pKyoujuro->Get_AnimIndex());
}



