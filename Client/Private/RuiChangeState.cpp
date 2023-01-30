#include "stdafx.h"
#include "RuiChangeState.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Rui;


CChangeState::CChangeState(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CRuiState * CChangeState::HandleInput(CRui* pRui)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CRuiState * CChangeState::Tick(CRui* pRui, _float fTimeDelta)
{

	pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			//return new CChangeState(CTanjiroState::TYPE_END);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}


	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		Increase_Height(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_LOOP:
		Fall_Height(pRui, fTimeDelta);
		break;
	case Client::CRuiState::TYPE_END:
		break;
	}


	return nullptr;
}

CRuiState * CChangeState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	

	RELEASE_INSTANCE(CGameInstance);

	pRui->Get_Model()->Play_Animation(fTimeDelta);


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

void CChangeState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_CHANGE;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_CHANGE_UP);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_CHANGE_UP, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_CHANGE_UP);
		m_vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_CHANGE_DOWN_START);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_CHANGE_DOWN_START, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_CHANGE_DOWN_START);
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		pRui->Set_NavigationHeight(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fOriginPosY = pRui->Get_NavigationHeight().y;
		m_vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fGravity = 15.81f;
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_CHANGE_DOWN_END);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_CHANGE_DOWN_END, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_CHANGE_DOWN_END);
		m_bNextAnim = false;
		break;
	}

}

void CChangeState::Exit(CRui* pRui)
{
	pRui->Get_Model()->Reset_Anim(pRui->Get_AnimIndex());
}

CRuiState* CChangeState::Increase_Height(CRui* pRui, _float fTimeDelta)
{

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 200.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 20.f)
	{
		
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

CRuiState* CChangeState::Fall_Height(CRui* pRui, _float fTimeDelta)
{

	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);


	return nullptr;


}

