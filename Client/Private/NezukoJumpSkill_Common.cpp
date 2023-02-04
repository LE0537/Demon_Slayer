#include "stdafx.h"
#include "NezukoJumpSkill_Common.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "NezukojumpState.h"
using namespace Nezuko;


CJumpSkill_CommmonState::CJumpSkill_CommmonState(STATE_TYPE eType, _uint iCount)
	: m_iCount(iCount)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	
	RELEASE_INSTANCE(CGameInstance);

}

CNezukoState * CJumpSkill_CommmonState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	

	return nullptr;
}

CNezukoState * CJumpSkill_CommmonState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CJumpSkill_CommmonState(TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CJumpSkill_CommmonState(TYPE_END);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			break;
		}
	}


	if (TYPE_END == m_eStateType)
	{
	

		if (pNezuko->Get_Model()->Get_CurrentTime() >=30.f)
		{
			Jump(pNezuko, fTimeDelta);

		
		}


		if (pNezuko->Get_Model()->Get_CurrentTime() >= 40.f)
			return new CJumpState(TYPE_END, 0.f,0.f);
	}
	

	

	return nullptr;
}

CNezukoState * CJumpSkill_CommmonState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CJumpSkill_CommmonState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_JUMP;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_0);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_JUMPCOMMON_0);
		pNezuko->Get_Transform()->Set_PlayerLookAt(pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_0);
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_1);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_JUMPCOMMON_1);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_1, false);
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_2);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_JUMPCOMMON_2);
		pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_SKILL_JUMPCOMMON_2);
		pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		Initialize_value(pNezuko);
		break;
	}

}

void CJumpSkill_CommmonState::Exit(CNezuko* pNezuko)
{
	
}

void CJumpSkill_CommmonState::Jump(CNezuko* pNezuko, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;



	vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));



	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
			 
			m_bNextAnim = true;
			//pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIMID::ANIM_JUMP_END);
			//pNezuko->Set_AnimIndex(CNezuko::ANIM_JUMP_END);
			//pNezuko->Get_Model()->Set_Loop(CNezuko::ANIMID::ANIM_JUMP_END, false);
			//pNezuko->Get_Model()->Set_LinearTime(pNezuko->Get_AnimIndex(), 0.01f);
		}
	}
	else
	{
		if (pNezuko->Get_NavigationCom()->Cheak_Cell(vecPos))
			pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}

}

void CJumpSkill_CommmonState::Initialize_value(CNezuko* pNezuko)
{

	m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}


