#include "stdafx.h"
#include "EnmuDashState.h"
#include "EnmuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "EnmuMoveState.h"


using namespace Enmu;

CDashState::CDashState(OBJDIR eDir, _bool bSecondJump, _bool bJump)
	: m_eDir(eDir), m_bSecondDash(bSecondJump), m_bJump(bJump)
{
}

CEnmuState * CDashState::HandleInput(CEnmu* pEnmu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CEnmuState * CDashState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	
		if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
		{
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			pEnmu->Get_Transform()->Set_PlayerLookAt(pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			return new CIdleState();
		}
	



	return nullptr;
}

CEnmuState * CDashState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	Move(pEnmu, fTimeDelta);
	pEnmu->Get_Model()->Play_Animation(fTimeDelta * 1.2f);


	return nullptr;
}

void CDashState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_DASH;

	if (!m_bTrue)
	{
		_float fCamAngle = pEnmu->Get_CamAngle();
		iIndex = pEnmu->Get_iTargetIndex();
		pEnmu->Get_Transform()->Set_PlayerLookAt(pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		//switch (iIndex)
		//{
		//case 1:
		//	pEnmu->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		//	break;
		//case 2:
		//	pEnmu->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		//	break;
		//default:
		//	break;
		//}
		m_bTrue = true;
	}



	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_F);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_F);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_F, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_F);
		}
		else if (iIndex == 2)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_B);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_B);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_B, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_B);
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_L, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_L);
		}
		else if (iIndex == 2)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_R, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_R);
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_R, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_R);
		}
		else if (iIndex == 2)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_L, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_L);
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_B);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_B);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_B, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_B);
		}
		else if (iIndex == 2)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_F);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_F);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_F, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_F);
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_L, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_L);
		}
		else if (iIndex == 2)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_R, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_R);
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_R, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_R);
		}
		else if (iIndex == 2)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_L, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_L);
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_L, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_L);
		}
		else if (iIndex == 2)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_R, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_R);
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_R);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_R, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_R);
		}
		else if (iIndex == 2)
		{
			pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_DASH_L);
			pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_DASH_L, 0.01f);
			pEnmu->Set_AnimIndex(CEnmu::ANIM_DASH_L);
		}
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}



}

void CDashState::Exit(CEnmu* pEnmu)
{
	pEnmu->Get_Model()->Reset_Anim(pEnmu->Get_AnimIndex());
}

void CDashState::Move(CEnmu* pEnmu, _float fTimeDelta)
{
	m_fTime += fTimeDelta;

	switch (m_eDir)
	{
	case Client::DIR_STRAIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		break;
	case Client::DIR_LEFT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		break;
	case Client::DIR_BACK:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		break;
	case Client::DIR_LF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Left(fTimeDelta, pEnmu->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Right(fTimeDelta, pEnmu->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RF:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Right(fTimeDelta, pEnmu->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Left(fTimeDelta, pEnmu->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Left(fTimeDelta, pEnmu->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Right(fTimeDelta, pEnmu->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_RB:
		if (iIndex == 1)
		{
			if (m_fTime < 0.4f)
			{
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Right(fTimeDelta, pEnmu->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
			}
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.4f)
			{
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Left(fTimeDelta, pEnmu->Get_NavigationCom());
			}
			else if (m_fTime >= 0.4f && m_fTime < 0.55f)
			{
				pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
			}
		}
		break;
	case Client::DIR_LEFT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		break;
	case Client::DIR_RIGHT_DASH:
		if (iIndex == 1)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		else if (iIndex == 2)
		{
			if (m_fTime < 0.25f)
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
			else
				pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
		}
		break;
	case Client::DIR_STOP:
		break;
	case Client::DIR_END:
		break;
	default:
		break;
	}
	Check_Coll(pEnmu, fTimeDelta);
}

void CDashState::Check_Coll(CEnmu* pEnmu, _float fTimeDelta)
{
	CCharacters* m_pTarget = pEnmu->Get_BattleTarget();
	CCollider*	pMyCollider = pEnmu->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{

		_float fSpeed = pEnmu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;

		_vector vPow = XMVector3Dot(pEnmu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPos))
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		else
		{
			switch (m_eDir)
			{
			case Client::DIR_STRAIGHT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				break;
			case Client::DIR_LEFT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				break;
			case Client::DIR_RIGHT:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				break;
			case Client::DIR_BACK:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				break;
			case Client::DIR_LF:
				if (iIndex == 1)
				{
					if (m_fTime < 0.4f)
					{
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Right(fTimeDelta, pEnmu->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.4f)
					{
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 2, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 2, pEnmu->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_RF:
				if (iIndex == 1)
				{
					if (m_fTime < 0.4f)
					{
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Left(fTimeDelta, pEnmu->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.4f)
					{
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Right(fTimeDelta, pEnmu->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_LB:
				if (iIndex == 1)
				{
					if (m_fTime < 0.4f)
					{
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Right(fTimeDelta, pEnmu->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.4f)
					{
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Left(fTimeDelta, pEnmu->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_RB:
				if (iIndex == 1)
				{
					if (m_fTime < 0.4f)
					{
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Left(fTimeDelta, pEnmu->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pEnmu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
					}
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.4f)
					{
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Right(fTimeDelta, pEnmu->Get_NavigationCom());
					}
					else if (m_fTime >= 0.4f && m_fTime < 0.55f)
					{
						pEnmu->Get_Transform()->Go_Backward(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.2f, pEnmu->Get_NavigationCom());
					}
				}
				break;
			case Client::DIR_LEFT_DASH:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				break;
			case Client::DIR_RIGHT_DASH:
				if (iIndex == 1)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Left(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				else if (iIndex == 2)
				{
					if (m_fTime < 0.25f)
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 1.5f, pEnmu->Get_NavigationCom());
					else
						pEnmu->Get_Transform()->Go_Right(fTimeDelta * 0.4f, pEnmu->Get_NavigationCom());
				}
				break;
			case Client::DIR_STOP:
				break;
			case Client::DIR_END:
				break;
			default:
				break;
			}
		}
	}
}


