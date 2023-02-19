#include "stdafx.h"
#include "ShinobuHitState.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "ShinobuUpperHitState.h"
#include "Camera_Dynamic.h"
#include "Layer.h"

using namespace Shinobu;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump)
{
}

CShinobuState * CHitState::HandleInput(CShinobu* pShinobu)
{

	return nullptr;
}

CShinobuState * CHitState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pShinobu->Get_PlayerInfo().iUnicCount < 3 && pShinobu->Get_PlayerInfo().iUnicBar < pShinobu->Get_PlayerInfo().iUnicMaxBar)
		{
			pShinobu->Set_UnicBar(33);
			if (pShinobu->Get_PlayerInfo().iUnicBar >= pShinobu->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pShinobu->Get_PlayerInfo().iUnicCount < 3)
				{
					pShinobu->Reset_UnicBar();
					pShinobu->Set_UnicCount(1);
				}
				else
					pShinobu->Set_UnicBar(pShinobu->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_HIT);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_HIT);
		m_bReset = true;
	}


	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pShinobu->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pShinobu->Get_NavigationCom());

	}

	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}
	
	if (pShinobu->Get_AnimIndex() == CShinobu::ANIM_DEAD)
	{
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_RETURN_1);
			pShinobu->Get_Model()->Set_Loop(CShinobu::ANIMID::ANIM_HIT_DMG_RETURN_1);
			pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIMID::ANIM_HIT_DMG_RETURN_1, 0.01f);
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
		}
	}
	else if (pShinobu->Get_Model()->Get_End(CShinobu::ANIM_HIT_DMG_RETURN_1))
	{
		pShinobu->Get_Model()->Set_End(CShinobu::ANIM_HIT_DMG_RETURN_1);
		return new CIdleState(STATE_HIT);
	}

	else
	{

		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState(STATE_HIT);
		}
	}




	return nullptr;
}

CShinobuState * CHitState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	//m_fJumpTime += 0.035f;
	//if (m_bJumpHit && !m_bJump)
	//{
	//	Jump(pShinobu, m_fJumpTime);
	//}

	if (pShinobu->Get_AnimIndex() == CShinobu::ANIMID::ANIM_HIT_DMG2_G)
		pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);





	return nullptr;
}

void CHitState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_HIT;

	if (pShinobu->Get_PlayerInfo().iHp <= 0)
	{
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIMID::ANIM_DEAD);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_DEAD);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_DEAD);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.05f);
		pShinobu->Set_GodMode(true);
		CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
		dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pShinobu->Get_Renderer());
		RELEASE_INSTANCE(CGameInstance);
		g_bDeathTime = true;
	}
	else
	{

		if (pShinobu->Get_NavigationHeight().y < XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))
		{
			m_bHitPlayerJump = true;
			Set_JumpHitState(pShinobu);
		}
		else
		{
			m_bHitPlayerJump = false;
			Set_HitState(pShinobu);
		}

	}


	_uint iRand = rand() % 4;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_2.wav"), fEFFECT);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_3.wav"), fEFFECT);
	else if (iRand == 3)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_4.wav"), fEFFECT);

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff2.wav"), fEFFECT);
}




CShinobuState * CHitState::Jump(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pShinobu->Get_NavigationHeight().y;
	pShinobu->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 40.f;


	_vector      vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pShinobu->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Set_HitState(CShinobu* pShinobu)
{
	_int iHit = pShinobu->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 3);
	int iRandom = RandomPattern(gen);

	if (iHit >= 12)
	{

		switch (iRandom)
		{
		case 1:
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_F);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_F);
			pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
			pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
			break;
		case 2:
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_L);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_L);
			pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
			pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
			break;
		case 3:
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_R);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_R);
			pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
			pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
			break;
		}
	}

	else
	{

		switch (iHit)
		{
		case 3: // atk 1
			pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_F);
			pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_F);
			pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
			pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
			break;
		case 4: // atk 2
			if (pShinobu->Get_Atk2() == false)
			{
				pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_L);
				pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_L);
				pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
				pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
				pShinobu->Set_Atk2(true);
			}
			else if (pShinobu->Get_Atk2() == true)
			{
				pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_R);
				pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_R);
				pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
				pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
				pShinobu->Set_Atk2(false);
			}
			break;
		case 5: // atk 3
			if (pShinobu->Get_Atk2() == true)
			{
				pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_F);
				pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_F);
				pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
				pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
				pShinobu->Set_Atk2(false);
				break;
			}
			else
			{
				pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG2_G);
				pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG2_G);
				pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
				pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
				pShinobu->Set_Atk2(false);
			}
			break;
		case 6: // atk 4
			if (pShinobu->Get_Atk2() == true)
			{
				std::random_device RandomDevice;
				std::mt19937 gen(RandomDevice());
				std::uniform_int_distribution<int> RandomPattern(1, 3);
				int iRandom = RandomPattern(gen);
				switch (iRandom)
				{
				case 1:
					pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_F);
					pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_F);
					pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
					pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
					break;
				case 2:
					pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_L);
					pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_L);
					pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
					pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
					break;
				case 3:
					pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_R);
					pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_R);
					pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
					pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
					break;
				}
			}
			else
			{
				pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG2_F);
				pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG2_F);
				pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
				pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
			}
			break;
		default:
			break;
		}

	}


}
void CHitState::Set_JumpHitState(CShinobu* pShinobu)
{

	_int iHit = pShinobu->Get_BattleTarget()->Get_TargetState();

	std::random_device RandomDevice;
	std::mt19937 gen(RandomDevice());
	std::uniform_int_distribution<int> RandomPattern(1, 4);
	int iRandom = RandomPattern(gen);


	switch (iRandom)
	{
	case 1:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_AF);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_AF);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		break;
	case 2:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_AL);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_AL);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		break;
	case 3:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_AR);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_AR);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		break;
	case 4:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT_DMG_AU);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT_DMG_AU);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());
		pShinobu->Get_Model()->Set_LinearTime(pShinobu->Get_AnimIndex(), 0.2f);
		break;
	}
}
void CHitState::Exit(CShinobu* pShinobu)
{
	pShinobu->Set_HitTime(0.3f);
	//pShinobu->Get_Model()->Reset_Anim(pShinobu->Get_AnimIndex());
}



