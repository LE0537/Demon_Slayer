#include "stdafx.h"
#include "..\Public\Skill3_EnmuState.h"
#include "GameInstance.h"
#include "EnmuIdleState.h"

using namespace Enmu;

CSkill3_EnmuState::CSkill3_EnmuState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CSkill3_EnmuState::HandleInput(CEnmu * pEnmu)
{
	return nullptr;
}

CEnmuState * CSkill3_EnmuState::Tick(CEnmu * pEnmu, _float fTimeDelta)
{
	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CSkill3_EnmuState(TYPE_END);
			break;
		case Client::CEnmuState::TYPE_END:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CSkill3_EnmuState(TYPE_CHANGE);
			break;
		//case Client::CEnmuState::TYPE_DEFAULT:
		//	pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
		//	return new CSkill3_EnmuState(TYPE_CHANGE);
		//	break;
		case Client::CEnmuState::TYPE_CHANGE:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			break;

		case Client::CEnmuState::TYPE_ADD:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CIdleState();
			break;
		default:
			break;
		}

		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		Increase_Height(pEnmu, fTimeDelta);

		if (m_bNextAnim == true)
			return new CSkill3_EnmuState(TYPE_LOOP);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		break;
	case Client::CEnmuState::TYPE_END:
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		Decrease_Height(pEnmu, fTimeDelta);

		if (m_bNextAnim == true)
			return new CSkill3_EnmuState(TYPE_ADD);
		break;
	default:
		break;
	}


	return nullptr;
}

CEnmuState * CSkill3_EnmuState::Late_Tick(CEnmu * pEnmu, _float fTimeDelta)
{
	pEnmu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill3_EnmuState::Enter(CEnmu * pEnmu)
{
	m_eStateId = STATE_SKILL3;

	

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_SKILL_JUMP_1);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_JUMP_1);
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_JUMP_1);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_JUMP_1);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_JUMP_1, 0.01f);
		
		break;
	case Client::CEnmuState::TYPE_LOOP:
		//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_SKILL_3_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_3_0);
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_3_0);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_3_0);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_3_0, 0.01f);
		
		break;
	case Client::CEnmuState::TYPE_END:
		//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_SKILL_3_1);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_3_1);
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_3_1);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_3_1);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_3_1, 0.01f);
		
		break;
	//case Client::CEnmuState::TYPE_DEFAULT:
	//	//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_SKILL_3_2);
	//	pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_3_2);
	//	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_3_2);
	//	pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_3_2);
	//	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_3_2, 0.01f);
		
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_SKILL_JUMP_2);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_JUMP_2);
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_JUMP_2);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_JUMP_2, true);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_JUMP_2, 0.01f);
		
		break;
	case Client::CEnmuState::TYPE_ADD:
		//pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_SKILL_JUMP_3);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_SKILL_JUMP_3);
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_SKILL_JUMP_3);
		pEnmu->Get_Model()->Set_Loop(CEnmu::ANIMID::ANIM_SKILL_JUMP_3);
		pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIMID::ANIM_SKILL_JUMP_3, 0.01f);
	
		break;
	default:
		break;
	}



}

void CSkill3_EnmuState::Exit(CEnmu * pEnmu)
{
}

void CSkill3_EnmuState::Increase_Height(CEnmu * pEnmu, _float fTimeDelta)
{
	//pEnmu->Get_Transform()->Set_Jump(true);

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 100.f;


	m_vPosition.x = XMVectorGetX(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.y += fGravity * fTimeDelta;
	//m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	//m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f + pEnmu->Get_NavigationHeight().y)
	{
		//	pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);



		m_bNextAnim = true;
	}
	else
		pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);



}

void CSkill3_EnmuState::Decrease_Height(CEnmu * pEnmu, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;


	m_vPosition.x = XMVectorGetX(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	m_vPosition.y += fVelocity * fTimeDelta;

	m_fOriginPosY = pEnmu->Get_NavigationHeight().y;


	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


		pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	//	pEnmu->Get_Transform()->Set_Jump(false);
		m_bNextAnim = true;
	}
	else
		pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);




}
