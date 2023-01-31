#include "stdafx.h"
#include "NezukoSkill_FallCut.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Nezuko;


CSkill_FallCutState::CSkill_FallCutState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;

}

CNezukoState * CSkill_FallCutState::HandleInput(CNezuko* pNezuko)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	return nullptr;
}

CNezukoState * CSkill_FallCutState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CSkill_FallCutState(CNezukoState::TYPE_LOOP);
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());

			if (m_bNextAnim == true)
			{		
				return new CSkill_FallCutState(CNezukoState::TYPE_END);
			}

			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}


	if (m_eStateType == STATE_TYPE::TYPE_START)
		Increase_Height(pNezuko, fTimeDelta);
	if (m_eStateType == STATE_TYPE::TYPE_LOOP)
		Fall_Height(pNezuko, fTimeDelta);

	return nullptr;
}

CNezukoState * CSkill_FallCutState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CCharacters* m_pTarget = pNezuko->Get_BattleTarget();

	pNezuko->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_FallCutState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_SKILL_FALLCUT;



	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		_vector vLook = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		pNezuko->Get_Transform()->Set_PlayerLookAt(vLook);

		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_0);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_0, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_0);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());

		m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_1);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_1, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_1);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		_vector vMyPosition = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPosition = pNezuko->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		m_vTargetPosition = XMVector3Normalize(vTargetPosition- vMyPosition);
		m_fGravity = 15.81f;
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_SKILL_FALLCUT_2);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_SKILL_FALLCUT_2, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_SKILL_FALLCUT_2);
		pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());

		break;
	}

}

void CSkill_FallCutState::Exit(CNezuko* pNezuko)
{
	m_pCollBox->Set_Dead();
}

CNezukoState * CSkill_FallCutState::Increase_Height(CNezuko * pNezuko, _float fTimeDelta)
{
	static _float fJump_Velocity = 10.f;
	static _float fGravity = 100.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f)
	{

		//pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);



	return nullptr;
}

CNezukoState * CSkill_FallCutState::Fall_Height(CNezuko * pNezuko, _float fTimeDelta)
{

	static _float fGravity = -500.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity * fTimeDelta;

	vPosition.x += XMVectorGetX(m_vTargetPosition) * 50.f * fTimeDelta;
	vPosition.y += XMVectorGetY(m_vTargetPosition) * -fVelocity * fTimeDelta;
	vPosition.z += XMVectorGetZ(m_vTargetPosition) * 50.f * fTimeDelta;

	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vecPos = XMVectorSetX(vecPos, vPosition.x);
	vecPos = XMVectorSetY(vecPos, vPosition.y);
	vecPos = XMVectorSetZ(vecPos, vPosition.z);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetX(vecPos, vPosition.x);
		vecPos = XMVectorSetY(vecPos, vPosition.y);
		vecPos = XMVectorSetZ(vecPos, vPosition.z);

		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);


	return nullptr;
}

