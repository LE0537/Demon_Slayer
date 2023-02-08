#include "stdafx.h"
#include "ShinobuChangeState.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "ShinobuSkill_Common.h"
#include "ShinobuSkill_Move.h"
using namespace Shinobu;


CChangeState::CChangeState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CShinobuState * CChangeState::HandleInput(CShinobu* pShinobu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CShinobuState * CChangeState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			break;
		case Client::CShinobuState::TYPE_LOOP:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			pShinobu->Set_ChangeInfo(false);
			switch (pShinobu->Get_SubSkill())
			{
			case 0:
				return new CIdleState();
				break;
			case 1:
				return new CSkill_CommonState();
				break;
			case 2:
				return new CSkill_MoveState(STATE_TYPE::TYPE_START);
				break;
			default:
				break;
			}
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_PCHANGE_UP, pShinobu);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		Increase_Height(pShinobu, fTimeDelta);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_PCHANGE_DOWN, pShinobu);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		Fall_Height(pShinobu, fTimeDelta);
		break;
	case Client::CShinobuState::TYPE_END:
		break;
	}


	return nullptr;
}

CShinobuState * CChangeState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	

	RELEASE_INSTANCE(CGameInstance);

	pShinobu->Get_Model()->Play_Animation(fTimeDelta);


	if (m_bNextAnim == true)
	{
		if (m_eStateType == TYPE_START)
		{
			pShinobu->Set_Change(true, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
			return new CIdleState();
		}
		else if (m_eStateType == TYPE_LOOP)
		{
			return new CChangeState(STATE_TYPE::TYPE_END);
		}
	}
	

	return nullptr;
}

void CChangeState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_CHANGE;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_CHANGE_UP);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_CHANGE_UP, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_CHANGE_UP);
		m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_CHANGE_DOWN_START);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_CHANGE_DOWN_START, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_CHANGE_DOWN_START);
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pShinobu->Get_NavigationHeight().y;
		m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fGravity = 15.81f;
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_CHANGE_DOWN_END);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_CHANGE_DOWN_END, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_CHANGE_DOWN_END);
		m_bNextAnim = false;
		break;
	}

}

void CChangeState::Exit(CShinobu* pShinobu)
{
	pShinobu->Get_Model()->Reset_Anim(pShinobu->Get_AnimIndex());
}

CShinobuState* CChangeState::Increase_Height(CShinobu* pShinobu, _float fTimeDelta)
{

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 200.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 20.f)
	{
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bNextAnim = true;
	}
	else
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

CShinobuState* CChangeState::Fall_Height(CShinobu* pShinobu, _float fTimeDelta)
{

	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);


	return nullptr;


}

