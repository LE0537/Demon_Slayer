#include "stdafx.h"
#include "ShinobuHitState.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"

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
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_HIT);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_HIT);
		m_bReset = true;
	}
	fHitTime += fTimeDelta * 60.f;

	if (!m_bJumpHit)
	{
		if (fHitTime <= 20.f)
			pShinobu->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pShinobu->Get_NavigationCom());

		_vector vPlayerY = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	
		vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
		if (vPlayerY.m128_f32[1] < 0)
			vPlayerY.m128_f32[1] = 0;

		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);
		if (fHitTime >= 35.f)
			return new CIdleState();
	}
	else if (m_bJumpHit)
	{
		if (fHitTime <= 35.f)
		{
			if (!m_bTrun)
			{
				_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK);
				vPos += XMVector3Normalize(vLook) * 15.f * fTimeDelta * m_fPow;
				if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPos))
					pShinobu->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pShinobu->Get_NavigationCom());
				else
				{
					m_bTrun = true;
				}
			}
			if (m_bTrun)
			{
				pShinobu->Get_Transform()->Go_Straight(fTimeDelta * m_fPow, pShinobu->Get_NavigationCom());
			}
		}

		if (pShinobu->Get_Model()->Get_End(CShinobu::ANIM_HIT))
		{
			pShinobu->Get_Model()->Set_End(CShinobu::ANIM_HIT);
			pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_HIT);
			return new CIdleState();
		}
	}
	return nullptr;
}

CShinobuState * CHitState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pShinobu, m_fJumpTime);
	}
	pShinobu->Get_Model()->Play_Animation(fTimeDelta * 1.1f);




	return nullptr;
}

void CHitState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_HIT;

	pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_HIT);
	pShinobu->Set_AnimIndex(CShinobu::ANIM_HIT);
}
CShinobuState * CHitState::Jump(CShinobu* pShinobu, _float fTimeDelta)
{
	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 12.5f;
	static _float fGravity = 15.f;


	_vector      vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}

	pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CHitState::Exit(CShinobu* pShinobu)
{
	pShinobu->Set_HitTime(0.2f);
}



