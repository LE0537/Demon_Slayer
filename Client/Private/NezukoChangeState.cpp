#include "stdafx.h"
#include "NezukoChangeState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "NezukoSkill_Common.h"
#include "NezukoSkill_Move.h"
using namespace Nezuko;


CChangeState::CChangeState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CNezukoState * CChangeState::HandleInput(CNezuko* pNezuko)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CNezukoState * CChangeState::Tick(CNezuko* pNezuko, _float fTimeDelta)
{

	pNezuko->Get_Model()->Set_Loop(pNezuko->Get_AnimIndex());

	if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CNezukoState::TYPE_START:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			break;
		case Client::CNezukoState::TYPE_LOOP:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			//return new CChangeState(CTanjiroState::TYPE_END);
			break;
		case Client::CNezukoState::TYPE_END:
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			pNezuko->Set_ChangeInfo(false);
			switch (pNezuko->Get_SubSkill())
			{
			case 0:
				return new CIdleState();
				break;
			case 1:
				return new CSkill_CommonState(STATE_TYPE::TYPE_START);
				break;
			case 2:
				return new CSkill_MoveState(STATE_TYPE::TYPE_START);
				break;
			default:
				break;
			}
			break;
		}
		pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		Increase_Height(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_LOOP:
		Fall_Height(pNezuko, fTimeDelta);
		break;
	case Client::CNezukoState::TYPE_END:
		break;
	}


	return nullptr;
}

CNezukoState * CChangeState::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	

	RELEASE_INSTANCE(CGameInstance);

	pNezuko->Get_Model()->Play_Animation(fTimeDelta);


	if (m_bNextAnim == true)
	{
		if (m_eStateType == TYPE_START)
		{
			pNezuko->Set_Change(true, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
			return new CIdleState();
		}
		else if (m_eStateType == TYPE_LOOP)
		{
			return new CChangeState(STATE_TYPE::TYPE_END);
		}
	}
	

	return nullptr;
}

void CChangeState::Enter(CNezuko* pNezuko)
{
	m_eStateId = STATE_ID::STATE_CHANGE;

	switch (m_eStateType)
	{
	case Client::CNezukoState::TYPE_START:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_CHANGE_UP);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_CHANGE_UP, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_CHANGE_UP);
		m_vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		break;
	case Client::CNezukoState::TYPE_LOOP:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_CHANGE_DOWN_START);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_CHANGE_DOWN_START, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_CHANGE_DOWN_START);
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		pNezuko->Set_NavigationHeight(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pNezuko->Get_NavigationHeight().y;
		m_vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fGravity = 15.81f;
		break;
	case Client::CNezukoState::TYPE_END:
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CNezuko::ANIM_CHANGE_DOWN_END);
		pNezuko->Get_Model()->Set_LinearTime(CNezuko::ANIM_CHANGE_DOWN_END, 0.01f);
		pNezuko->Set_AnimIndex(CNezuko::ANIM_CHANGE_DOWN_END);
		m_bNextAnim = false;
		break;
	}

}

void CChangeState::Exit(CNezuko* pNezuko)
{
	pNezuko->Get_Model()->Reset_Anim(pNezuko->Get_AnimIndex());
}

CNezukoState* CChangeState::Increase_Height(CNezuko* pNezuko, _float fTimeDelta)
{

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 200.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 20.f)
	{
		
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

CNezukoState* CChangeState::Fall_Height(CNezuko* pNezuko, _float fTimeDelta)
{

	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pNezuko->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pNezuko->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);


	return nullptr;


}

