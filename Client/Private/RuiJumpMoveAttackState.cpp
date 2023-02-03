#include "stdafx.h"
#include "RuiJumpMoveAttackState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"
#include "RuiMoveState.h"
using namespace Rui;

CJumpMoveAttackState::CJumpMoveAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiState * CJumpMoveAttackState::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CJumpMoveAttackState::Tick(CRui* pRui, _float fTimeDelta)
{

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START: // 공격 모션
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CJumpMoveAttackState(TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP: // 떨어지는 모션
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());;
			break;
		case Client::CRuiState::TYPE_END: // 착지 모션
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState(STATE_JUMP);
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_LOOP:
		Jump(pRui, fTimeDelta);

		if (m_bNextAnim == true)
			return new CJumpMoveAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CRuiState * CJumpMoveAttackState::Late_Tick(CRui* pRui, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pRui->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pRui->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpMoveAttackState::Enter(CRui* pRui)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_MOVE_ATTACK_0);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_0, 0.01f);
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_1);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_MOVE_ATTACK_1);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, true);
		Initialize_value(pRui);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_MOVE_ATTACK_2);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_1, false);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIMID::ANIM_JUMP_MOVE_ATTACK_2, 0.01f);
		break;
	}
}

void CJumpMoveAttackState::Exit(CRui* pRui)
{
}

void CJumpMoveAttackState::Jump(CRui* pRui, _float fTimeDelta)
{
	static _float fGravity = 100.f;
	static _float fVelocity = 0.f;
	

	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;
	 


	if (m_bRange == true)
	{
		//m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition);
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition) * 5.f;
		//m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition);
	}
	else if (m_bRange == false)
	{
		m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition)* 3.f;
		m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition)* 5.f;
		m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition)* 3.f;
	}


	_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetX(vecPos, m_vPosition.x);
	vecPos = XMVectorSetY(vecPos, m_vPosition.y);
	vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, m_vPosition.x);
		vecPos = XMVectorSetY(vecPos, m_vPosition.y);
		vecPos = XMVectorSetZ(vecPos, m_vPosition.z);

		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpMoveAttackState::Initialize_value(CRui* pRui)
{
	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRui->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;
}

