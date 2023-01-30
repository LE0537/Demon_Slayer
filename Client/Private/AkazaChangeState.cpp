#include "stdafx.h"
#include "AkazaChangeState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Akaza;


CChangeState::CChangeState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CAkazaState * CChangeState::HandleInput(CAkaza* pAkaza)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CAkazaState * CChangeState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			//return new CChangeState(CTanjiroState::TYPE_END);
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
		Increase_Height(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		Fall_Height(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_END:
		break;
	}


	return nullptr;
}

CAkazaState * CChangeState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	

	RELEASE_INSTANCE(CGameInstance);

	pAkaza->Get_Model()->Play_Animation(fTimeDelta);


	if (m_bNextAnim == true)
	{
		if(m_eStateType == TYPE_START)
			return new CIdleState();
		else if (m_eStateType == TYPE_LOOP)
		{
			return new CChangeState(STATE_TYPE::TYPE_END);
		}
	}
	

	return nullptr;
}

void CChangeState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_CHANGE;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_CHANGE_UP);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_CHANGE_UP, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_CHANGE_UP);
		m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_CHANGE_DOWN_START);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_CHANGE_DOWN_START, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_CHANGE_DOWN_START);
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pAkaza->Get_NavigationHeight().y;
		m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fGravity = 15.81f;
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_CHANGE_DOWN_END);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_CHANGE_DOWN_END, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_CHANGE_DOWN_END);
		m_bNextAnim = false;
		break;
	}

}

void CChangeState::Exit(CAkaza* pAkaza)
{
	pAkaza->Get_Model()->Reset_Anim(pAkaza->Get_AnimIndex());
}

CAkazaState* CChangeState::Increase_Height(CAkaza* pAkaza, _float fTimeDelta)
{

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 200.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 20.f)
	{
		
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

CAkazaState* CChangeState::Fall_Height(CAkaza* pAkaza, _float fTimeDelta)
{

	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

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
		pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);


	return nullptr;


}

