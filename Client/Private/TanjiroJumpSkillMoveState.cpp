#include "stdafx.h"
#include "TanjiroJumpSkillMoveState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
using namespace Tanjiro;

CJumpSkillMoveState::CJumpSkillMoveState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * CJumpSkillMoveState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CJumpSkillMoveState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START: // 공격 모션
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CJumpSkillMoveState(TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP: // 떨어지는 모션
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			
			if(m_bNextAnim == true)
			return new CJumpSkillMoveState(TYPE_END);
			break;
		case Client::CTanjiroState::TYPE_END: // 착지 모션
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_LOOP:
		Jump(pTanjiro, fTimeDelta);
	}



	return nullptr;
}

CTanjiroState * CJumpSkillMoveState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpSkillMoveState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPMOVE_0);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPMOVE_0);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_WATERMILL_0, 0.01f);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPMOVE_1);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPMOVE_1);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_JUMPMOVE_1, 0.01f);
		Initialize_value(pTanjiro);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPMOVE_2);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPMOVE_2);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_JUMPMOVE_2, 0.01f);
		break;
	}
}

void CJumpSkillMoveState::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
}

void CJumpSkillMoveState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f; 
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	//pTanjiro->Get_Transform()->Go_Straight(fTimeDelta, pTanjiro->Get_NavigationCom());

	_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{ 
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
			m_bNextAnim = true;
		}
	}
	else
	{
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpSkillMoveState::Initialize_value(CTanjiro * pTanjiro)
{
	m_vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}

