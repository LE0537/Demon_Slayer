#include "stdafx.h"
#include "KyoujuroTargetRushState.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"
#include "KyoujuroMoveState.h"
using namespace Kyoujuro;

CTargetRushState::CTargetRushState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CKyoujuroState * CTargetRushState::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CTargetRushState::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CKyoujuroState::TYPE_START: 
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CTargetRushState(TYPE_LOOP);
			break;
		case Client::CKyoujuroState::TYPE_LOOP: 
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());;
			break;
		case Client::CKyoujuroState::TYPE_END: 
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_LOOP:
		Move(pKyoujuro, fTimeDelta);

		if (m_bNextAnim == true)
		{
			return new CTargetRushState(TYPE_END);
		}
		break;
	}



	return nullptr;
}

CKyoujuroState * CTargetRushState::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pKyoujuro->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CTargetRushState::Enter(CKyoujuro* pKyoujuro)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CKyoujuroState::TYPE_START:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_0);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_TARGET_RUSH_0);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_0, 0.01f);
		pKyoujuro->Get_Transform()->Set_PlayerLookAt(pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CKyoujuroState::TYPE_LOOP:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_1);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_TARGET_RUSH_1);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_1, true);
		Initialize_value(pKyoujuro);
		break;
	case Client::CKyoujuroState::TYPE_END:
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_2);
		pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_TARGET_RUSH_2);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_1, false);
		pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_2);
		pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_TARGET_RUSH_2, 0.01f);
		break;
	}
}

void CTargetRushState::Exit(CKyoujuro* pKyoujuro)
{
}

void CTargetRushState::Move(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	static _float fGravity = 9.8f;

	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;

	_vector vCurrentPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (pKyoujuro->Get_SphereCollider()->Collision(pKyoujuro->Get_BattleTarget()->Get_SphereCollider()))
	{
		m_bNextAnim = true;
		_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		pKyoujuro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(vPos);

		if (pKyoujuro->Get_BattleTarget()->Get_PlayerInfo().bGuard)
		{
			pKyoujuro->Get_BattleTarget()->Get_GuardHit(0);
		}
		else
		{
			pKyoujuro->Get_BattleTarget()->Take_Damage(0.3f, false);
		}
	}
	else
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CTargetRushState::Initialize_value(CKyoujuro* pKyoujuro)
{
	m_vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pKyoujuro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pKyoujuro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

