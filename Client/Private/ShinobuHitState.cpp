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
	m_fJumpTime += 0.035f;
	if (m_bJumpHit && !m_bJump)
	{
		Jump(pShinobu, m_fJumpTime);
	}

	_vector vPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	if (m_bJumpHit == false)
	{
		if (pShinobu->Get_Model()->Get_CurrentFrame() <= 60)
		{
			pShinobu->Get_Transform()->Go_Backward(fTimeDelta * m_fPow, pShinobu->Get_NavigationCom());

			_vector vPlayerY = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			vPlayerY.m128_f32[1] -= fTimeDelta * 3.f;
			if (vPlayerY.m128_f32[1] < 0)
				vPlayerY.m128_f32[1] = 0;

			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPlayerY);

		
		}

		pShinobu->Get_Model()->Play_Animation(fTimeDelta, false);

		if (pShinobu->Get_Model()->Get_CurrentFrame() >= 54)
			return new CIdleState();
	}
	else
	{
		if (pShinobu->Get_Model()->Get_CurrentFrame() <= 60)
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
			pShinobu->Get_Model()->Play_Animation(fTimeDelta, false);

		}
		
		else if (pShinobu->Get_Model()->Get_CurrentTime() >= 86)
		{
			//if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
			//{
			//	pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				//pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_HIT);
				return new CIdleState();
			//}
		}
		else
			pShinobu->Get_Model()->Play_Animation(fTimeDelta, false);

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
	
	
	
	if (m_bJumpHit == false)
	{
		pShinobu->Get_Model()->Set_FrameNum(pShinobu->Get_AnimIndex(), 100);
		//pAkaza->Get_Model()->Set_FrameTime(pAkaza->Get_AnimIndex(), 0, 20, 1.f);
		pShinobu->Get_Model()->Set_UsingFrame(CShinobu::ANIM_HIT, 30, 60);
	}
	else
	{
		pShinobu->Get_Model()->Set_FrameNum(pShinobu->Get_AnimIndex(), 100);
		//pAkaza->Get_Model()->Set_FrameTime(pAkaza->Get_AnimIndex(), 0, 20, 1.f);
		pShinobu->Get_Model()->Set_UsingFrame(CShinobu::ANIM_HIT, 30, 100);
	}

	_uint iRand = rand() % 4;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_Hit_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_Hit_2.wav"), fEFFECT);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_Hit_3.wav"), fEFFECT);
	else if (iRand == 3)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_Hit_4.wav"), fEFFECT);
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
	pShinobu->Set_HitTime(0.5f);
	pShinobu->Get_Model()->Set_UsingFrame(CShinobu::ANIM_HIT, 0, 100);
	pShinobu->Get_Model()->Clear_Frame(CShinobu::ANIM_HIT);
}



