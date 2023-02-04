#include "stdafx.h"
#include "TanjiroJumpSkillCommonState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
using namespace Tanjiro;

CJumpSkillCommonState::CJumpSkillCommonState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * CJumpSkillCommonState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CJumpSkillCommonState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	//if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	//{
	//	switch (m_eStateType)
	//	{
	//	case Client::CTanjiroState::TYPE_START: // 공격 모션
	//		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	//		return new CJumpSkillCommonState(TYPE_LOOP);
	//		break;
	//	case Client::CTanjiroState::TYPE_LOOP: // 떨어지는 모션
	//		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());;
	//		break;
	//	case Client::CTanjiroState::TYPE_END: // 착지 모션
	//		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	//		return new CIdleState(STATE_JUMP);
	//		break;
	//	}
	//	pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	//}

	//switch (m_eStateType)
	//{
	//case Client::CTanjiroState::TYPE_LOOP:
	//	Jump(pTanjiro, fTimeDelta);

	//	if(m_bNextAnim == true)
	//		return new CJumpSkillCommonState(TYPE_END);
	//}

	if(m_eStateType != TYPE_CHANGE && m_bNextAnim != true)
		Jump(pTanjiro, fTimeDelta);



	//if (m_bNextAnim == true && pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	//{
	//	pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	//	//return new CJumpSkillCommonState(TYPE_CHANGE);
	//}

	
	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		if(m_bNextAnim == true)
			return new CIdleState();
	}



	return nullptr;
}

CTanjiroState * CJumpSkillCommonState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{


	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CJumpSkillCommonState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_JUMP;

	//switch (m_eStateType)
	//{
	//case Client::CTanjiroState::TYPE_START:
	//	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_0);
	//	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPCOMMON_0);
	//	pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//	break;
	//case Client::CTanjiroState::TYPE_LOOP:
	//	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_1);
	//	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPCOMMON_1);
	//	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_1, false);
	//	Initialize_value(pTanjiro);
	//	break;
	//case Client::CTanjiroState::TYPE_END:
	//	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_2);
	//	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_JUMPCOMMON_2);
	//	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_SKILL_JUMPCOMMON_2, false);
	//	break;
	//}

	if (m_eStateType == TYPE_CHANGE)
	{
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_JUMP_END);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
	}
	else
	{
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_COMMON);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_COMMON);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_SKILL_COMMON, false);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_COMMON, 0.01f);
		Initialize_value(pTanjiro);
	}




}

void CJumpSkillCommonState::Exit(CTanjiro * pTanjiro)
{
}

void CJumpSkillCommonState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f; 
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));



	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

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

void CJumpSkillCommonState::Initialize_value(CTanjiro * pTanjiro)
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

