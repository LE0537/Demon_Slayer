#include "stdafx.h"
#include "EnmuHitState.h"
#include "EnmuIdleState.h"
#include "GameInstance.h"
#include "EnmuUpperHitState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"

using namespace Enmu;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
}

CEnmuState * CHitState::HandleInput(CEnmu* pEnmu)
{

	return nullptr;
}

CEnmuState * CHitState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pEnmu->Get_PlayerInfo().iUnicCount < 3 && pEnmu->Get_PlayerInfo().iUnicBar < pEnmu->Get_PlayerInfo().iUnicMaxBar)
		{
			pEnmu->Set_UnicBar(33);
			if (pEnmu->Get_PlayerInfo().iUnicBar >= pEnmu->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pEnmu->Get_PlayerInfo().iUnicCount < 3)
				{
					pEnmu->Reset_UnicBar();
					pEnmu->Set_UnicCount(1);
				}
				else
					pEnmu->Set_UnicBar(pEnmu->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		m_bReset = true;
	}


	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pEnmu->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pEnmu->Get_NavigationCom());

	}


	//	_vector vPlayerY = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//
	//	vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
	//	if (vPlayerY.m128_f32[1] < 0)
	//		vPlayerY.m128_f32[1] = 0;

	//	pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
	//	if (fHitTime >= 35.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//	{
	//		if (!m_bTrun)
	//		{
	//			_vector vPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//			_vector vLook = pEnmu->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	//			vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
	//			if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPos))
	//				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pEnmu->Get_NavigationCom());
	//			else
	//			{
	//				m_bTrun = true;
	//			}
	//		}
	//		if (m_bTrun)
	//		{
	//			pEnmu->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pEnmu->Get_NavigationCom());
	//		}
	//	}
	//	if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_HIT))
	//	{
	//		pEnmu->Get_Model()->Set_End(CEnmu::ANIM_HIT);
	//		//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIM_HIT);

	//		pEnmu->Get_Model()->Set_CurrentAnimIndex(58);
	//		pEnmu->Get_Model()->Set_Loop(58);
	//		pEnmu->Get_Model()->Set_LinearTime(58, 0.01f);
	//		pEnmu->Set_bGuard(true);
	//	}

	//	if (pEnmu->Get_Model()->Get_End(58))
	//	{
	//		pEnmu->Get_Model()->Reset_Anim(58);
	//		pEnmu->Get_Model()->Set_End(58);
	//		pEnmu->Set_bGuard(false);
	//		return new CIdleState();
	//	}
	//}

	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}


	if (pEnmu->Get_AnimIndex() == CEnmu::ANIM_DEAD)
	{
		if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_DEAD))
		{
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_RETURN_1);
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_HIT_DMG_RETURN_1, 0.01f);
			pEnmu->Get_Model()->Set_End(CEnmu::ANIM_DEAD);
		}
	}
	else if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_HIT_DMG_RETURN_1))
	{
		pEnmu->Get_Model()->Set_End(CEnmu::ANIM_HIT_DMG_RETURN_1);
		return new CIdleState(STATE_HIT);
	}

	else
	{
		if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
		{
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CIdleState(STATE_HIT);
		}

	}



	return nullptr;
}

CEnmuState * CHitState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	//m_fJumpTime += 0.035f;
	//if (m_bJumpHit && !m_bJump)
	//{
	//	Jump(pEnmu, m_fJumpTime);
	//}

	if (pEnmu->Get_BattleTarget()->Get_SplSkl() == true)
		fTimeDelta *= 0.3f;

	if (pEnmu->Get_AnimIndex() == CEnmu::ANIMID::ANIM_HIT_DMG2_G)
		pEnmu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pEnmu->Get_Model()->Play_Animation(fTimeDelta);





	return nullptr;
}

void CHitState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_HIT;

	if (pEnmu->Get_PlayerInfo().iHp <= 0)
	{
		pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_DEAD);
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DEAD);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_DEAD);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.05f);
		pEnmu->Set_GodMode(true);
		CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_BATTLEENMU, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pEnmu->Get_Renderer());
		RELEASE_INSTANCE(CGameInstance);
		g_bDeathTime = true;
	}
	else
	{
		if (pEnmu->Get_NavigationHeight().y < XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
		{
			m_bHitPlayerJump = true;
			Set_JumpHitState(pEnmu);
		}
		else
		{
			m_bHitPlayerJump = false;
			Set_HitState(pEnmu);
		}
	}

	_uint iRand = rand() % 2;

	//if (iRand == 0)
	//	CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_Hit_1.wav"), fVOICE);
	//else if (iRand == 1)
	//	CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_Hit_2.wav"), fVOICE);

}




CEnmuState * CHitState::Jump(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Set_NavigationHeight(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pEnmu->Get_NavigationHeight().y;
	pEnmu->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pEnmu->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Set_HitState(CEnmu* pEnmu)
{
	_int iHit = pEnmu->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 3);
	int iRandom = RandomPattern(gen);

	if (iHit >= 12)
	{
		switch (iRandom)
		{
		case 1:
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_F);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_F);
			pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
			pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
			break;
		case 2:
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_L);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_L);
			pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
			pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
			break;
		case 3:
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_R);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_R);
			pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
			pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
			break;
		}
	}

	else
	{

		switch (iHit)
		{
		case 3: // atk 1
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_F);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_F);
			pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
			pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
			break;
		case 4: // atk 2
			if (pEnmu->Get_Atk2() == false)
			{
				pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_L);
				pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_L);
				pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
				pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
				pEnmu->Set_Atk2(true);
			}
			else if (pEnmu->Get_Atk2() == true)
			{
				pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_R);
				pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_R);
				pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
				pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
				pEnmu->Set_Atk2(false);
			}
			break;
		case 5: // atk 3
			if (pEnmu->Get_Atk2() == true)
			{
				pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_F);
				pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_F);
				pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
				pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
				pEnmu->Set_Atk2(false);
				break;
			}
			else
			{
				pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG2_G);
				pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG2_G);
				pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
				pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
				pEnmu->Set_Atk2(false);
			}
			break;
		case 6: // atk 4
			if (pEnmu->Get_Atk2() == true)
			{
				std::random_device RandomDevice;
				std::mt19937 gen(RandomDevice());
				std::uniform_int_distribution<int> RandomPattern(1, 3);
				int iRandom = RandomPattern(gen);
				switch (iRandom)
				{
				case 1:
					pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_F);
					pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_F);
					pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
					pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
					break;
				case 2:
					pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_L);
					pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_L);
					pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
					pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
					break;
				case 3:
					pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_R);
					pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_R);
					pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
					pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
					break;
				}
			}
			else
			{
				pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG2_F);
				pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG2_F);
				pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
				pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
			}
			break;
		default:
			break;
		}

	}


}
void CHitState::Set_JumpHitState(CEnmu* pEnmu)
{

	_int iHit = pEnmu->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);


	switch (iRandom)
	{
	case 1:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_AF);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_AF);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	case 2:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_AL);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_AL);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	case 3:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_AR);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_AR);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	case 4:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_AU);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_AU);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	}
}
void CHitState::Exit(CEnmu* pEnmu)
{
	pEnmu->Set_HitTime(0.3f);
	//pEnmu->Get_Model()->Reset_Anim(pEnmu->Get_AnimIndex());
}



