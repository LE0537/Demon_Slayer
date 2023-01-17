#include "stdafx.h"
#include "KyoujuroMoveState.h"
#include "KyoujuroIdleState.h"
#include "Kyoujuro.h"
#include "KyoujuroMoveJumpState.h"
#include "GameInstance.h"
#include "KyoujuroAtk_1_State.h"
#include "Characters.h"
#include "Layer.h"


using namespace Kyoujuro;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CKyoujuroState * CMoveState::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		return new CAtk_1_State();

	if (pGameInstance->Key_Pressing(DIK_UP)) // 菊
	{
		if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
		{
			if (pGameInstance->Key_Down(DIK_SLASH))
			{
				_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_LF, CKyoujuroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_LF, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
		{
			if (pGameInstance->Key_Down(DIK_SLASH))
			{
				_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_RF, CKyoujuroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_RF, STATE_TYPE::TYPE_START);
		}
		else
		{
			if (pGameInstance->Key_Down(DIK_SLASH))
			{
				_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_STRAIGHT, CKyoujuroState::TYPE_START, fPositionY, 0.f);
			}

			return new CMoveState(OBJDIR::DIR_STRAIGHT, STATE_TYPE::TYPE_START);
		}
	}

	else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 第
	{
		if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
		{
			if (pGameInstance->Key_Down(DIK_SLASH))
			{
				_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_LB, CKyoujuroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_LB, STATE_TYPE::TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快 
		{
			if (pGameInstance->Key_Down(DIK_SLASH))
			{
				_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_RB, CKyoujuroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_RB, STATE_TYPE::TYPE_START);
		}
		else
		{
			if (pGameInstance->Key_Down(DIK_SLASH))
			{
				_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_float fPositionY = XMVectorGetY(vPosition);
				return new CMoveJumpState(OBJDIR::DIR_BACK, CKyoujuroState::TYPE_START, fPositionY, 0.f);
			}
			return new CMoveState(OBJDIR::DIR_BACK, STATE_TYPE::TYPE_START);
		}
	}


	else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 谅
	{
		if (pGameInstance->Key_Down(DIK_SLASH))
		{
			_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CMoveJumpState(OBJDIR::DIR_LEFT, CKyoujuroState::TYPE_START, fPositionY, 0.f);
		}
		return new CMoveState(OBJDIR::DIR_LEFT, STATE_TYPE::TYPE_START);
	}
	else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 快
	{
		if (pGameInstance->Key_Down(DIK_SLASH))
		{
			_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_float fPositionY = XMVectorGetY(vPosition);
			return new CMoveJumpState(OBJDIR::DIR_RIGHT, CKyoujuroState::TYPE_START, fPositionY, 0.f);
		}
		return new CMoveState(OBJDIR::DIR_RIGHT, STATE_TYPE::TYPE_START);
	}
	else
		return new CIdleState();



	return nullptr;
}

CKyoujuroState * CMoveState::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{

	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START:
			m_eStateType = CKyoujuroState::TYPE_LOOP;
			break;
		case Client::CKyoujuroState::TYPE_LOOP:
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState();
			break;
		case Client::CKyoujuroState::TYPE_DEFAULT:
			break;
		default:
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}

	return nullptr;


}

CKyoujuroState * CMoveState::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	Move(pKyoujuro, fTimeDelta);
	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CMoveState::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_MOVE_START);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_MOVE_START);
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_MOVE_END);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_MOVE_END);
		break;
	case Client::CKyoujuroState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CKyoujuro * pKyoujuro)
{
}

void CMoveState::Move(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pKyoujuro->Get_Transform()->Set_RotationY(0.f);
		break;
	case Client::DIR_LEFT:
		pKyoujuro->Get_Transform()->Set_RotationY(270.f);
		break;
	case Client::DIR_RIGHT:
		pKyoujuro->Get_Transform()->Set_RotationY(90.f);
		break;
	case Client::DIR_BACK:
		pKyoujuro->Get_Transform()->Set_RotationY(180.f);
		break;
	case Client::DIR_LF:
		pKyoujuro->Get_Transform()->Set_RotationY(305.f);
		break;
	case Client::DIR_RF:
		pKyoujuro->Get_Transform()->Set_RotationY(45.f);
		break;
	case Client::DIR_LB:
		pKyoujuro->Get_Transform()->Set_RotationY(225.f);
		break;
	case Client::DIR_RB:
		pKyoujuro->Get_Transform()->Set_RotationY(135.f);
		break;
	case Client::DIR_STOP:
		break;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_eDirection != DIR_STOP)
		pKyoujuro->Get_Transform()->Go_StraightNoNavi(fTimeDelta);

	CCollider*	pMyCollider = pKyoujuro->Get_Collider();
	CCollider*	pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"), TEXT("Com_SPHERE"));

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{
		CCharacters* m_pTarget = (CCharacters*)pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Tanjiro"))->Get_LayerFront();

		_float fSpeed = pKyoujuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;
		
		_vector vPow = XMVector3Dot(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);
		
		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
	}
	
	RELEASE_INSTANCE(CGameInstance);
}
