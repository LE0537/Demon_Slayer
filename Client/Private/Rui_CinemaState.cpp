#include "stdafx.h"
#include "..\Public\Rui_CinemaState.h"
#include "GameInstance.h"
#include "RuiIdleState.h"

using namespace Rui;

CRui_CinemaState::CRui_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CRuiState * CRui_CinemaState::HandleInput(CRui * pRui)
{
	return nullptr;
}

CRuiState * CRui_CinemaState::Tick(CRui * pRui, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Rui::CRui_CinemaState::SCENE_START:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CRui_CinemaState(CRui_CinemaState::SCENE_0);
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_0:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CRui_CinemaState(CRui_CinemaState::SCENE_1);
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_1:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CRui_CinemaState(CRui_CinemaState::SCENE_2);
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_2:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CRui_CinemaState(CRui_CinemaState::SCENE_3);
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_3:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
		}
		break;
	case Client::Rui::CRui_CinemaState::SCENE_4:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_5:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_6:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_END:
		break;
	
	}

	return nullptr;
}

CRuiState * CRui_CinemaState::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	
		pRui->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CRui_CinemaState::Enter(CRui * pRui)
{
	m_eStateId = STATE_SKILL_SPHERE;
	pRui->Set_SplSkl(true);

	switch (m_eScene)
	{
	case Client::Rui::CRui_CinemaState::SCENE_START:
		pRui->Get_Model()->Reset_Anim(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_START));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_START, 0.05f);
		
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pRui->Get_NavigationHeight().y, 64.f, 1.f));
		//pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pRui->Get_NavigationHeight().y, 38.5f, 1.f));
		
		//pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		//pRui->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::Rui::CRui_CinemaState::SCENE_0:

		pRui->Get_Model()->Reset_Anim(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_0));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_0, 0.05f);

		//pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pRui->Get_NavigationHeight().y, 64.f, 1.f));
		//pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pRui->Get_NavigationHeight().y, 38.5f, 1.f));

		//pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		//pRui->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		break;
	case Client::Rui::CRui_CinemaState::SCENE_1:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_1);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_1));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_1);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_1, 0.05f);
		break;
	case Client::Rui::CRui_CinemaState::SCENE_2:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_2);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_2));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_2);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_2, 0.05f);
		break;
	case Client::Rui::CRui_CinemaState::SCENE_3:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_3);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_3));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_3);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_3, 0.05f);
		break;
	case Client::Rui::CRui_CinemaState::SCENE_4:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_5:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_6:
		break;
	case Client::Rui::CRui_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

}

void CRui_CinemaState::Exit(CRui * pRui)
{
}

void CRui_CinemaState::Increase_Height(CRui * pRui, _float fTimeDelta)
{
	static _float fJump_Velocity = 5.f;
	static _float fGravity = 10.f;
	static _float3 vPosition;

	m_vPosition.x = XMVectorGetX(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));


	fJump_Velocity += fGravity * fTimeDelta;

	m_vPosition.y += fJump_Velocity * fTimeDelta;


	//m_vPosition.x += m_vVelocity.x * XMVectorGetX(vLookAt) *  fTimeDelta;
	//m_vPosition.z += m_vVelocity.z * XMVectorGetZ(vLookAt) *  fTimeDelta;

	//_vector vCurrentPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vecPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vecPosition) > 20.f)
	{
		/*	fDelay += fTimeDelta;

		if(fDelay >= 0.3f)*/
		//m_bNextAnim = true;
	}
	else
	{
		pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPosition);
	}
}

void CRui_CinemaState::Decrease_Height(CRui * pRui, _float fTimeDelta)
{
	
	pRui->Set_NavigationHeight(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fPositionY = pRui->Get_BattleTarget()->Get_NavigationHeight().y;

	m_vPosition.x = XMVectorGetX(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	static _float fJump_Velocity = 10.f;
	static _float fGravity = -50.8f;


	m_vVelocity.y += fGravity * fTimeDelta;
	//m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	//m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) < m_fPositionY)
	{
		_vector vPosition;
		m_vPosition.y = m_fPositionY;
		m_vVelocity.y = m_fPositionY;
		m_bOnGround = true;
		g_bDeathTime = false;
		vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
		pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CRui_CinemaState::Gravity_Field(CRui * pRui, _float fTimeDelta)
{
	




}

CRuiState * CRui_CinemaState::Scene_Start(CRui * pRui, _float fTimeDelta)
{
	
	if (pRui->Get_Model()->Get_CurrentTime_Index(pRui->Get_AnimIndex()) <= 40.f)
	{
		m_fCurrentDuration += fTimeDelta;

		if (m_bIsHitMotion == false)
		{
			pRui->Get_BattleTarget()->Player_TakeDown(0.2f, false);
			m_bIsHitMotion = true;
		}

		if (m_fCurrentDuration >= 0.5f)
		{
			m_bIsHitMotion = false;
			m_fCurrentDuration = 0.f;
		}

	}
	else if (pRui->Get_Model()->Get_CurrentTime_Index(pRui->Get_AnimIndex()) >= 50.f)
	{
		m_fCurrentDuration += fTimeDelta;

		if (m_bControlMotion == false)
		{
			pRui->Get_BattleTarget()->Player_TakeDown(0.2f, false);
			m_bControlMotion = true;
		}

		if (m_fCurrentDuration >= 0.5f)
		{
			m_bControlMotion = false;
			m_fCurrentDuration = 0.f;
		}

		Increase_Height(pRui, fTimeDelta);
	}


	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{

		m_bNextAnim = true;
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	return nullptr;
}

CRuiState * CRui_CinemaState::Scene_0(CRui * pRui, _float fTimeDelta)
{

	Decrease_Height(pRui, fTimeDelta);

	m_fCurrentDuration += fTimeDelta;

	if (m_bIsHitMotion == false)
	{
		pRui->Get_BattleTarget()->Player_TakeDown(0.2f, false);
		m_bIsHitMotion = true;
	}

	if (m_fCurrentDuration >= 0.5f)
	{
		m_bIsHitMotion = false;
		m_fCurrentDuration = 0.f;
	}


	if (m_bOnGround == true)
		m_bNextAnim = true;


	return nullptr;
}
