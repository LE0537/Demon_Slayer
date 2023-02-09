#include "stdafx.h"
#include "RuiDadMoveState.h"
#include "RuiDadIdleState.h"
#include "RuiDad.h"
#include "GameInstance.h"
#include "Characters.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace RuiDad;


CMoveState::CMoveState(OBJDIR eDir, STATE_TYPE eType)
	: m_eDirection(eDir)
{
	m_eStateType = eType;
}

CRuiDadState * CMoveState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CRuiDadState * CMoveState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{


	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START:
			m_eStateType = CRuiDadState::TYPE_LOOP;
		}
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
	}
	



	return AIMove(pRuiDad, m_eDirection, fTimeDelta);
}

CRuiDadState * CMoveState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	Move(pRuiDad, fTimeDelta);
	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);

	if (pRuiDad->Get_PlayerInfo().bSub)
	{
		return new CIdleState();
	}
	if (pRuiDad->Get_PlayerInfo().iGuard < pRuiDad->Get_PlayerInfo().iMaxGuard)
	{
		pRuiDad->Set_GuardHp(1);
		if (pRuiDad->Get_PlayerInfo().iGuard > pRuiDad->Get_PlayerInfo().iMaxGuard)
			pRuiDad->Set_ResetGuardHp();
	}
	return nullptr;
}

void CMoveState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_MOVE_0);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_MOVE_0);
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_MOVE_1);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_MOVE_1);
		break;
	case Client::CRuiDadState::TYPE_DEFAULT:
		break;
	}
}

void CMoveState::Exit(CRuiDad* pRuiDad)
{
}

void CMoveState::Move(CRuiDad* pRuiDad, _float fTimeDelta)
{
	_float fCamAngle = pRuiDad->Get_CamAngle();

	switch (m_eDirection)
	{
	case Client::DIR_STRAIGHT:
		pRuiDad->Get_Transform()->Set_RotationY(0.f + fCamAngle);
		break;
	case Client::DIR_LEFT:
		pRuiDad->Get_Transform()->Set_RotationY(270.f + fCamAngle);
		break;
	case Client::DIR_RIGHT:
		pRuiDad->Get_Transform()->Set_RotationY(90.f + fCamAngle);
		break;
	case Client::DIR_BACK:
		pRuiDad->Get_Transform()->Set_RotationY(180.f + fCamAngle);
		break;
	case Client::DIR_LF:
		pRuiDad->Get_Transform()->Set_RotationY(305.f + fCamAngle);
		break;
	case Client::DIR_RF:
		pRuiDad->Get_Transform()->Set_RotationY(45.f + fCamAngle);
		break;
	case Client::DIR_LB:
		pRuiDad->Get_Transform()->Set_RotationY(225.f + fCamAngle);
		break;
	case Client::DIR_RB:
		pRuiDad->Get_Transform()->Set_RotationY(135.f + fCamAngle);
		break;
	case Client::DIR_STOP:
		break;
	}

	if (m_eDirection != DIR_STOP)
		pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CCharacters* m_pTarget = pRuiDad->Get_BattleTarget();
	CCollider*	pMyCollider = pRuiDad->Get_SphereCollider();
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

	if (nullptr == pTargetCollider)
		return;

	if (pMyCollider->Collision(pTargetCollider))
	{
		float fSpeed = pRuiDad->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;

		_vector vPow = XMVector3Dot(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		_vector vPlayerPosY = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];

		if (pRuiDad->Get_NavigationCom()->Cheak_Cell(vPos))
			pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		else
			pRuiDad->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRuiDad->Get_NavigationCom());
	}

	RELEASE_INSTANCE(CGameInstance);
}

CRuiDadState* CMoveState::AIMove(CRuiDad * pRuiDad, OBJDIR eDir, _float fTimeDelta)
{
	static _float fContinueTime = 0.f;
	static _bool bSetLook = false;

	if (bSetLook == false)
	{
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		bSetLook = true;
	}

	switch (eDir)
	{
	case Client::DIR_STRAIGHT:
		pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
		break;
	case Client::DIR_LEFT:
		pRuiDad->Get_Transform()->Go_Left(fTimeDelta, pRuiDad->Get_NavigationCom());
		break;
	case Client::DIR_RIGHT:
		pRuiDad->Get_Transform()->Go_Right(fTimeDelta, pRuiDad->Get_NavigationCom());
		break;
	case Client::DIR_BACK:
		pRuiDad->Get_Transform()->Go_Backward(fTimeDelta, pRuiDad->Get_NavigationCom());
		break;
	}

	fContinueTime += fTimeDelta;

	if (fContinueTime >= 1.4f)
		return new CIdleState();

	return nullptr;

}
