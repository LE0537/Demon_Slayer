#include "stdafx.h"
#include "NezukoHitState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"

using namespace Nezuko;

CHitState::CHitState(_float _fPow, _bool _bJump)
	:m_fPow(_fPow), m_bJumpHit(_bJump) 
{
}

CNezukoState * CHitState::HandleInput(CNezuko* pNezuko)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Down(DIK_F3))
		return new CHitState(0.f);

	return nullptr;
}

CNezukoState * CHitState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	//if (!m_bReset)
	//{
	//	pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIM_HIT);
	//	pNezuko->Get_Model()->Set_Loop(CNezuko::ANIM_HIT);
	//	m_bReset = true;
	//}

	fHitTime += fTimeDelta * 60.f;

	//if (!m_bJumpHit)
	//{
	//	if (fHitTime <= 20.f)
	//		pNezuko->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

	//	if (fHitTime >= 38.f)
	//		return new CIdleState();
	//}
	//else if (m_bJumpHit)
	//{
	//	if (fHitTime <= 35.f)
	//		pNezuko->Get_Transform()->Go_Backward(fTimeDelta * m_fPow);

	//	if (pNezuko->Get_Model()->Get_End(CNezuko::ANIM_HIT))
	//	{
	//		pNezuko->Get_Model()->Set_End(CNezuko::ANIM_HIT);
	//		pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIM_HIT);
	//		return new CIdleState();
	//	}
	//}


	return nullptr;
}

CNezukoState * CHitState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pNezuko, m_fJumpTime);
	}

	_vector vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	if (m_bJumpHit == false)
	{
		if (pNezuko->Get_Model()->Get_CurrentFrame() <= 20)
		{
			pNezuko->Get_Model()->Play_Animation(fTimeDelta * 1.5f, false);
			pNezuko->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pNezuko->Get_NavigationCom());

			_vector vPlayerY = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
			if (vPlayerY.m128_f32[1] < 0)
				vPlayerY.m128_f32[1] = 0;

			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);

			if (pNezuko->Get_Model()->Get_CurrentFrame() == 19)
				return new CIdleState();
		}
	}
	else
	{
		if (pNezuko->Get_Model()->Get_CurrentFrame() <= 60)
		{
			if (!m_bTrun)
			{
				_vector vPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vLook = pNezuko->Get_Transform()->Get_State(CTransform::STATE_LOOK);
				vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
				if (pNezuko->Get_NavigationCom()->Cheak_Cell(vPos))
					pNezuko->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pNezuko->Get_NavigationCom());
				else
				{
					m_bTrun = true;
				}
			}
			if (m_bTrun)
			{
				pNezuko->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pNezuko->Get_NavigationCom());
			}
		}

		pNezuko->Get_Model()->Play_Animation(fTimeDelta, false);


		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
		}

	}






	return nullptr;
}
	


void CHitState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_HIT);
	pNezuko->Set_AnimIndex(CNezuko::ANIM_HIT);
	pNezuko->Get_Model()->Reset_Anim(CNezuko::ANIM_HIT);
	//pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
	//pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.0f);

	if (m_bJumpHit == false)
	{
		pNezuko->Get_Model()->Set_FrameNum(pNezuko->Get_AnimIndex(), 100);
		//pNezuko->Get_Model()->Set_FrameTime(pNezuko->Get_AnimIndex(), 0, 20, 1.f);
		pNezuko->Get_Model()->Set_UsingFrame(CNezuko::ANIM_HIT, 8, 20);
	}
	else
	{
		pNezuko->Get_Model()->Set_FrameNum(pNezuko->Get_AnimIndex(), 100);
		//pNezuko->Get_Model()->Set_FrameTime(pNezuko->Get_AnimIndex(), 0, 20, 1.f);
		pNezuko->Get_Model()->Set_UsingFrame(CNezuko::ANIM_HIT, 25, 100);
	}


}
CNezukoState * CHitState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 7.f;
	static _float fGravity = 14.f;


	_vector      vPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Exit(CNezuko* pNezuko)
{
	pNezuko->Get_Model()->Set_UsingFrame(CNezuko::ANIM_HIT, 0, 100);
	pNezuko->Set_HitTime(0.5f);
}



