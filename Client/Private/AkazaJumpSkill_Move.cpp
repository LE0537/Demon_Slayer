#include "stdafx.h"
#include "AkazaJumpSkill_Move.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Akaza;


CJumpSkill_MoveState::CJumpSkill_MoveState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;

	RELEASE_INSTANCE(CGameInstance);

}

CAkazaState * CJumpSkill_MoveState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CAkazaState * CJumpSkill_MoveState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			if (m_bNextAnim == true)
			return new CJumpSkill_MoveState(TYPE_END);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		if (pAkaza->Get_Model()->Get_CurrentTime() <= 5.f)
			Move(pAkaza, fTimeDelta);
		else if (pAkaza->Get_Model()->Get_CurrentTime() >= 57.f)
			m_bJump = true;
		break;
	}

	if (m_bJump == true)
		Jump(pAkaza, fTimeDelta);



	return nullptr;
}

CAkazaState * CJumpSkill_MoveState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CJumpSkill_MoveState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_JUMP;


	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SKILL_JUMPMOVE_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_JUMPMOVE_0);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_SKILL_JUMPMOVE_0);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		Initialize_MoveValue(pAkaza);
		Initialize_value(pAkaza);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SKILL_JUMPMOVE_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_JUMPMOVE_1);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_SKILL_JUMPMOVE_1);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SKILL_JUMPMOVE_2);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_JUMPMOVE_2);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_SKILL_JUMPMOVE_2);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		break;
	}


}


void CJumpSkill_MoveState::Exit(CAkaza* pAkaza)
{

}

void CJumpSkill_MoveState::Jump(CAkaza * pAkaza, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;



	vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));



	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

			m_bNextAnim = true;
			//pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_END);
			//pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_END);
			//pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_END, false);
			//pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		}
	}
	else
	{
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vecPos))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}
}

void CJumpSkill_MoveState::Move(CAkaza * pAkaza, _float fTimeDelta)
{
	static _float fGravity = 19.8f;

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));


	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f * fTimeDelta;
	m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z * 15.f * fTimeDelta;

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (fDistance <= 3.f)
	{
		
	}
	else
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


}

void CJumpSkill_MoveState::Initialize_value(CAkaza * pAkaza)
{

	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;


	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

}

void CJumpSkill_MoveState::Initialize_MoveValue(CAkaza * pAkaza)
{
	m_vMovePosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vMovePosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vMovePosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vMoveVelocity.x = 1.f;
	m_vMoveVelocity.y = 0.f;
	m_vMoveVelocity.z = 1.f;

	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fMoveOriginPosY = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}



