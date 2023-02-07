#include "stdafx.h"
#include "TanjiroChangeState.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "TanjiroSkill_Common.h"
#include "TanjiroSkill_WaterMill.h"
using namespace Tanjiro;


CChangeState::CChangeState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CTanjiroState * CChangeState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CTanjiroState * CChangeState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			pTanjiro->Set_ChangeInfo(false);
			switch (pTanjiro->Get_SubSkill())
			{
			case 0:
				return new CIdleState();
				break;
			case 1:
				return new CSkill_CommonState();
				break;
			case 2:
				return new CSkill_WaterMillState(STATE_TYPE::TYPE_START);
				break;
			default:
				break;
			}
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		Increase_Height(pTanjiro, fTimeDelta);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		Fall_Height(pTanjiro, fTimeDelta);
		break;
	case Client::CTanjiroState::TYPE_END:
		break;
	}


	return nullptr;
}

CTanjiroState * CChangeState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	

	RELEASE_INSTANCE(CGameInstance);

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);


	if (m_bNextAnim == true)
	{
		if (m_eStateType == TYPE_START)
		{
			pTanjiro->Set_Change(true, XMVectorSet(-50000.f, -50000.f, -50000.f, 1.f));
			return new CIdleState();
		}
		else if (m_eStateType == TYPE_LOOP)
		{
			return new CChangeState(STATE_TYPE::TYPE_END);
		}
	}
	

	return nullptr;
}

void CChangeState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_CHANGE;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_CHANGE_UP);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_CHANGE_UP, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_CHANGE_UP);
		m_vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_CHANGE_DOWN_START);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_CHANGE_DOWN_START, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_CHANGE_DOWN_START);
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
		m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fGravity = 15.81f;
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_CHANGE_DOWN_END);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_CHANGE_DOWN_END, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_CHANGE_DOWN_END);
		m_bNextAnim = false;
		break;
	}

}

void CChangeState::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
}

CTanjiroState* CChangeState::Increase_Height(CTanjiro * pTanjiro, _float fTimeDelta)
{

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 200.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 20.f)
	{
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bNextAnim = true;
	}
	else
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

CTanjiroState* CChangeState::Fall_Height(CTanjiro * pTanjiro, _float fTimeDelta)
{

	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);


	return nullptr;


}

