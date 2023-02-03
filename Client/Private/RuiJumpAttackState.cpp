#include "stdafx.h"
#include "RuiJumpAttackState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"
#include "RuiMoveState.h"
using namespace Rui;

CJumpAttackState::CJumpAttackState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiState * CJumpAttackState::HandleInput(CRui* pRui)
{
	return nullptr;
}

CRuiState * CJumpAttackState::Tick(CRui* pRui, _float fTimeDelta)
{

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START: // 공격 모션
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CJumpAttackState(TYPE_LOOP);
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
		return new CJumpAttackState(TYPE_END);
		break;
	}



	return nullptr;
}

CRuiState * CJumpAttackState::Late_Tick(CRui* pRui, _float fTimeDelta)
{

	if (m_eStateType == TYPE_END)
		pRui->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	else
		pRui->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpAttackState::Enter(CRui* pRui)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMPATTACK);
		pRui->Set_AnimIndex(CRui::ANIM_JUMPATTACK);
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMP_LOOP_END);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_LOOP_END);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_LOOP_END, true);
		Initialize_value(pRui);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIMID::ANIM_JUMP_END);
		pRui->Set_AnimIndex(CRui::ANIM_JUMP_END);
		pRui->Get_Model()->Set_Loop(CRui::ANIMID::ANIM_JUMP_LOOP_END, false);
		break;
	}
}

void CJumpAttackState::Exit(CRui* pRui)
{
}

void CJumpAttackState::Jump(CRui* pRui, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f; 
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{ 
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpAttackState::Initialize_value(CRui* pRui)
{
	m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRui->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}

