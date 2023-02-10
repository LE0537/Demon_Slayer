#include "stdafx.h"
#include "AkazaHitState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"

using namespace Akaza;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump) 
{
}

CAkazaState * CHitState::HandleInput(CAkaza* pAkaza)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Down(DIK_F3))
		return new CHitState(0.f);

	return nullptr;
}

CAkazaState * CHitState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (!m_bReset)
	{
		if (pAkaza->Get_PlayerInfo().iUnicCount < 3 && pAkaza->Get_PlayerInfo().iUnicBar < pAkaza->Get_PlayerInfo().iUnicMaxBar)
		{
			pAkaza->Set_UnicBar(67);
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
		m_bReset = true;
	}

	fHitTime += fTimeDelta * 60.f;

	//if (!m_bJumpHit)
	//{
	//	if (fHitTime <= 20.f)
	//		pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

	//	if (fHitTime >= 38.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//		pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

	//	if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_HIT))
	//	{
	//		pAkaza->Get_Model()->Set_End(CAkaza::ANIM_HIT);
	//		pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
	//		return new CIdleState();
	//	}
	//}


	return nullptr;
}

CAkazaState * CHitState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pAkaza, m_fJumpTime);
	}

	_vector vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	if (m_bJumpHit == false)
	{
		if (pAkaza->Get_Model()->Get_CurrentFrame() <= 20)
		{
			pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.5f, false);
			pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pAkaza->Get_NavigationCom());

			_vector vPlayerY = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
			if (vPlayerY.m128_f32[1] < 0)
				vPlayerY.m128_f32[1] = 0;

			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);

			if (pAkaza->Get_Model()->Get_CurrentFrame() == 19)
				return new CIdleState();
		}
	}
	else
	{
		if (pAkaza->Get_Model()->Get_CurrentFrame() <= 60  && pAkaza->Get_AnimIndex() == CAkaza::ANIM_HIT)
		{
			if (!m_bTrun)
			{
				_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK);
				vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
				if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPos))
					pAkaza->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pAkaza->Get_NavigationCom());
				else
				{
					m_bTrun = true;
				}
			}
			if (m_bTrun)
			{
				pAkaza->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pAkaza->Get_NavigationCom());
			}
		}

		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.3f, false);


		if (pAkaza->Get_Model()->Get_End(65))
		{
			pAkaza->Get_Model()->Set_End(65);
				pAkaza->Get_Model()->Set_CurrentAnimIndex(37);
				pAkaza->Get_Model()->Set_Loop(37);
				pAkaza->Get_Model()->Set_LinearTime(37, 0.01f);
				pAkaza->Set_AnimIndex(CAkaza::ANIM_IDLE);
				pAkaza->Set_bGuard(true);
			//return new CIdleState();
		}

		if (pAkaza->Get_Model()->Get_End(37))
		{
			pAkaza->Get_Model()->Reset_Anim(37);
			pAkaza->Get_Model()->Set_End(37);
			pAkaza->Set_bGuard(false);
			return new CIdleState();
		}

	}






	return nullptr;
}
	


void CHitState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_HIT;
	pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT);

	//pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_HIT);
	//pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_HIT, 0.01f);

	if (m_bJumpHit == false)
	{
		pAkaza->Get_Model()->Set_FrameNum(pAkaza->Get_AnimIndex(), 100);
		//pAkaza->Get_Model()->Set_FrameTime(pAkaza->Get_AnimIndex(), 0, 20, 1.f);
		pAkaza->Get_Model()->Set_UsingFrame(CAkaza::ANIM_HIT, 8, 20);
	}
	else
	{
		pAkaza->Get_Model()->Set_FrameNum(pAkaza->Get_AnimIndex(), 100);
		//pAkaza->Get_Model()->Set_FrameTime(pAkaza->Get_AnimIndex(), 0, 20, 1.f);
		pAkaza->Get_Model()->Set_UsingFrame(CAkaza::ANIM_HIT, 25, 100);
	}

	_uint iRand = rand() % 4;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_Hit_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_Hit_2.wav"), fEFFECT);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_Hit_3.wav"), fEFFECT);
	else if (iRand == 3)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_Hit_4.wav"), fEFFECT);

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff2.wav"), fEFFECT);
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
void CHitState::Exit(CAkaza* pAkaza)
{
	pAkaza->Get_Model()->Set_UsingFrame(CAkaza::ANIM_HIT, 0, 100);
	pAkaza->Set_HitTime(0.5f);
	pAkaza->Get_Model()->Clear_Frame(CAkaza::ANIM_HIT);
}



