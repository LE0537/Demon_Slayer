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
	//if (!m_bReset)
	//{
	//	pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
	//	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_HIT);
	//	m_bReset = true;
	//}

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
		if (pAkaza->Get_Model()->Get_CurrentFrame() <= 60)
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

		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.5f, false);


		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
		}

	}






	return nullptr;
}
	


void CHitState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT);
	pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIM_HIT);
	//pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	//pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.0f);

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


}
CAkazaState * CHitState::Jump(CAkaza* pAkaza, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 7.f;
	static _float fGravity = 14.f;


	_vector      vPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
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
	pAkaza->Set_HitTime(0.2f);
}



