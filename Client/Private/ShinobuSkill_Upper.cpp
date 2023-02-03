#include "stdafx.h"
#include "ShinobuSkill_Upper.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Shinobu;


CSkill_UpperState::CSkill_UpperState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	m_eStateType = eType;
	

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CShinobuState * CSkill_UpperState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();




	return nullptr;
}

CShinobuState * CSkill_UpperState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			if (m_bNextAnim == true)
			{
				return new CSkill_UpperState(STATE_TYPE::TYPE_LOOP);
			}
			break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}



	


	return nullptr;
}

CShinobuState * CSkill_UpperState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();

	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		if (m_fTime <= 0.4f)
			Move(pShinobu, fTimeDelta);
		else
			Increase_Height(pShinobu, fTimeDelta);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		Fall_Height(pShinobu, fTimeDelta);
		if (m_bNextAnim == true)
		{
			return new CSkill_UpperState(TYPE_END);
		}
		break;
	}

	

	return nullptr;
}

void CSkill_UpperState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_SKILL_UPPER;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_SKILL_UPPER_0);
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_SKILL_UPPER_1);
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_SKILL_UPPER_2);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_UPPER_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_UPPER_0, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_UPPER_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SKILL_UPPER_0, false);
		m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;

		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_UPPER_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_UPPER_1, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_UPPER_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SKILL_UPPER_1, true);
		m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fCurrentPosY = pShinobu->Get_NavigationHeight().y;
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_UPPER_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_UPPER_2, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_UPPER_2);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SKILL_UPPER_2, false);
		break;
	}


	

}

void CSkill_UpperState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead();
}

void CSkill_UpperState::Move(CShinobu * pShinobu, _float fTimeDelta)
{
	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));

	pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	//pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.3f, pShinobu->Get_NavigationCom());

	static _float fGravity = 10.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;  XMStoreFloat3(&vPosition, vMyPosition);

	fVelocity += fGravity * fTimeDelta;

	vPosition.x += XMVectorGetX(XMVector3Normalize(vTargetPosition- vMyPosition)) * fVelocity * fTimeDelta;
		//vPosition.y += XMVectorGetY(m_vTargetPosition) * -fVelocity * fTimeDelta;
	vPosition.z += XMVectorGetZ(XMVector3Normalize(vTargetPosition- vMyPosition)) * fVelocity * fTimeDelta;


	_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vecPos = XMVectorSetX(vecPos, vPosition.x);
	vecPos = XMVectorSetZ(vecPos, vPosition.z);

	
	if(fDistance >=3.f)
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
}


CShinobuState * CSkill_UpperState::Increase_Height(CShinobu * pShinobu, _float fTimeDelta)
{
	static _float fJump_Velocity = 10.f;
	static _float fGravity = 100.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(XMVectorGetX(vCurrentPos), m_vPosition.y, XMVectorGetZ(vCurrentPos), 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f)
	{

		//pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}

CShinobuState * CSkill_UpperState::Fall_Height(CShinobu * pShinobu, _float fTimeDelta)
{
	static _float fGravity = -80.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	fVelocity += fGravity * fTimeDelta;

	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vecPos = XMVectorSetX(vecPos, vPosition.x);
	vecPos = XMVectorSetY(vecPos, vPosition.y);
	vecPos = XMVectorSetZ(vecPos, vPosition.z);

	if (vPosition.y <= m_fCurrentPosY)// m_fCurrentPosY)
	{
		vPosition.y = m_fCurrentPosY;
		fVelocity = m_fCurrentPosY;

		_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		
		vecPos = XMVectorSetY(vecPos, vPosition.y);
	
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		
		m_bNextAnim = true;
	}
	else
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	

	return nullptr;
}

