#include "stdafx.h"
#include "AkazaAdvSkill_Common.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Akaza;


CAdvSkill_CommmonState::CAdvSkill_CommmonState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CAkazaState * CAdvSkill_CommmonState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	

	return nullptr;
}

CAkazaState * CAdvSkill_CommmonState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	_vector vMyPositionLoop = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPositionLoop = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vTargetPositionLoop = XMVectorSetY(vTargetPositionLoop, 0.f);
	vMyPositionLoop = XMVectorSetY(vMyPositionLoop, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPositionLoop - vMyPositionLoop));
	m_vTargetPosition = XMVector3Normalize(vTargetPositionLoop - vMyPositionLoop);
	m_fHeight = XMVectorGetY(m_vTargetPosition);
	m_fHeight += 7.f;
	m_vTargetPosition = XMVectorSetY(m_vTargetPosition, m_fHeight);


	m_fDelay += fTimeDelta;

	if (m_fDistance <= 3.f)
		m_bRange = true;
	else
		m_bRange = false;

	//pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

			if (m_bNextAnim == true)
				return new CAdvSkill_CommmonState(CAkazaState::TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
		//	pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
		//	return new CAdvSkill_CommmonState(CAkazaState::TYPE_END);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		//pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	

		if (m_fDelay < 0.5f && m_bRange == false)
		{
			//pAkaza->Get_Model()->Set_CurrentAnimIndex(25);
			//pAkaza->Get_Model()->Set_LinearTime(25, 0.01f);
			//pAkaza->Get_Model()->Set_Loop(25, true);

			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_MOVE_START);

		}
		else
		{
			pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_ADVSKILL_COMMON_0);
			pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_ADVSKILL_COMMON_0, 0.01f);
			pAkaza->Set_AnimIndex(CAkaza::ANIM_ADVSKILL_COMMON_0);
		}
		

		Increase_Height(pAkaza, fTimeDelta);
	
		if (m_bNextAnim == true && pAkaza->Get_Model()->Get_CurrentTime() >= 15.f)
			return new CAdvSkill_CommmonState(CAkazaState::TYPE_LOOP);

		break;
	case Client::CAkazaState::TYPE_LOOP:
		//pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
		Fall_Height(pAkaza, fTimeDelta);

		if (m_bNextAnim == true)
			return new CAdvSkill_CommmonState(TYPE_END);
		break;
	}


	return nullptr;
}

CAkazaState * CAdvSkill_CommmonState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	


	if (m_fDelay < 0.5f && m_bRange == false)
	{
		pAkaza->Get_Model()->Play_Animation(fTimeDelta *  2.f);
	}
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.1f);

	return nullptr;
}

void CAdvSkill_CommmonState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_ADVSKILL_COMMON;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_ADVSKILL_COMMON_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_ADVSKILL_COMMON_0, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_ADVSKILL_COMMON_0);

		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		////////////////////////////////////
		_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
		m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
		m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 1.f;
		m_vVelocity.y = 5.f;
		m_vVelocity.z = 1.f;
		m_fGravity = 0.f;
		pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pAkaza->Get_NavigationHeight().y;
		m_fHeight = XMVectorGetY(m_vTargetPosition);
		m_fHeight += 5.f;
		m_vTargetPosition = XMVectorSetY(m_vTargetPosition, m_fHeight);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_ADVSKILL_COMMON_0, false);
		//////////////////////////////////////////////////////
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_ADVSKILL_COMMON_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_ADVSKILL_COMMON_1, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_ADVSKILL_COMMON_1);
		///////////////////////////////
		_vector vMyPositionLoop = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPositionLoop = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		m_fDistance = XMVectorGetX(XMVector3Length(vTargetPositionLoop - vMyPositionLoop));
		m_vTargetPosition = XMVector3Normalize(vTargetPositionLoop - vMyPositionLoop);
		m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;

		pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pAkaza->Get_NavigationHeight().y;
		/////////////////////////////////////////////
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_ADVSKILL_COMMON_1, true);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_ADVSKILL_COMMON_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_ADVSKILL_COMMON_2, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_ADVSKILL_COMMON_2);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_ADVSKILL_COMMON_2, false);
		break;
	}

	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pCamera = dynamic_cast<CCamera_Dynamic*>(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront());
	RELEASE_INSTANCE(CGameInstance);

}

void CAdvSkill_CommmonState::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead();
}

CAkazaState * CAdvSkill_CommmonState::Increase_Height(CAkaza * pAkaza, _float fTimeDelta)
{

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 9.8f;

	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;



	if (m_fDelay < 0.5f && m_bRange == true)
	{
		m_vPosition.y += XMVectorGetY(m_vTargetPosition) *	 m_vVelocity.y * fTimeDelta;
	}
	else if(m_fDelay < 0.5f && m_bRange == false)
	{
		m_vPosition.x += XMVectorGetX(m_vTargetPosition) *   m_vVelocity.x * 15.f *fTimeDelta;
		m_vPosition.z += XMVectorGetZ(m_vTargetPosition) *   m_vVelocity.z  *15.f*  fTimeDelta;
	}
	else
	{
		m_vPosition.y += XMVectorGetY(m_vTargetPosition) *	 m_vVelocity.y * fTimeDelta;
	}


	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);



	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (XMVectorGetY(vCurrentPos) >= 7.f)
	{
		m_bNextAnim = true;
	}
	else
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);



	return nullptr;
}

CAkazaState * CAdvSkill_CommmonState::Fall_Height(CAkaza * pAkaza, _float fTimeDelta)
{

	static _float fGravity = -200.f;
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

		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		m_bNextAnim = true;
	}
	else
	{
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}


	//m_fGravity += fTimeDelta;
	//m_pCamera->Camera_Shake(m_fGravity);


	return nullptr;
}

