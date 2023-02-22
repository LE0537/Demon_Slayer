#include "stdafx.h"
#include "..\Public\Shinobu_CinemaState.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"


using namespace Shinobu;

CShinobu_CinemaState::CShinobu_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CShinobuState * CShinobu_CinemaState::HandleInput(CShinobu * pShinobu)
{
	return nullptr;
}

CShinobuState * CShinobu_CinemaState::Tick(CShinobu * pShinobu, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CShinobu_CinemaState::SCENE_START:
		Scene_Start(pShinobu, fTimeDelta);

		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_START))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_START);
			return new CShinobu_CinemaState(CShinobu_CinemaState::SCENE_0);
		}
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_0:
		Scene_0(pShinobu, fTimeDelta);


		if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_0_END))
		{
			pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_0_END);
			return new CIdleState();
		}
		break;

		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_1:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_2:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_3:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_4:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_5:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_6:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CShinobuState * CShinobu_CinemaState::Late_Tick(CShinobu * pShinobu, _float fTimeDelta)
{
	if (m_bAnimStop == false)
		pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CShinobu_CinemaState::Enter(CShinobu * pShinobu)
{
	m_eStateId = STATE_SKILL_COMMON;

	pShinobu->Set_SplSkl(true);

	//ÅºÁö·Î 57.932f, 1.f, 43.087f
	//	»ó´ë 59.838f, 0.f, 45.601f

	switch (m_eScene)
	{
	case Client::Shinobu::CShinobu_CinemaState::SCENE_START:
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_START));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_START, 0.02f);

		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(57.932f, pShinobu->Get_NavigationHeight().y, 43.087f, 1.f));
		pShinobu->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(59.838f, pShinobu->Get_NavigationHeight().y, 45.601f, 1.f));

		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pShinobu->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_0:
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_0_START);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_0_LOOP);
		pShinobu->Get_Model()->Reset_Anim(CShinobu_CinemaState::ANIM_SCENE_0_END);

		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_0_START);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_0_START));
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_0_START);
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_0_LOOP, true);
		pShinobu->Get_Model()->Set_Loop(CShinobu_CinemaState::ANIM_SCENE_0_END);

		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_0_START, 0.02f);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_0_LOOP, 0.01f);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu_CinemaState::ANIM_SCENE_0_END, 0.01f);
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_1:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_2:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_3:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_4:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_5:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_6:
		break;
	case Client::Shinobu::CShinobu_CinemaState::SCENE_END:
		break;
	default:
		break;
	}

}

void CShinobu_CinemaState::Exit(CShinobu * pShinobu)
{
}

CShinobuState * CShinobu_CinemaState::Scene_Start(CShinobu * pShinobu, _float fTimeDelta)
{
	if (pShinobu->Get_Model()->Get_CurrentTime_Index(pShinobu->Get_AnimIndex()) >= 38.f)
	{
		if (m_bControlMotion == false)
		{
			pShinobu->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_TYPE::HIT_KNOCKBACK, 5.f, 10.f, 7.f);
			m_bControlMotion = true;
		}
	}
	else if (pShinobu->Get_Model()->Get_CurrentTime_Index(pShinobu->Get_AnimIndex()) < 30.f)
	{
		if (m_bIsHitMotion == false)
		{
			pShinobu->Get_BattleTarget()->Take_Damage(0.0f, false);
			m_bIsHitMotion = true;
		}

		m_fTime += fTimeDelta;

		if (m_fTime >= 0.3f)
		{
			m_bIsHitMotion = false;
			m_fTime = 0.f;
		}
	}



	return nullptr;
}

CShinobuState * CShinobu_CinemaState::Scene_0(CShinobu * pShinobu, _float fTimeDelta)
{
	if (pShinobu->Get_AnimIndex() == CShinobu_CinemaState::ANIM_SCENE_0_START)
	{
		m_bAnimStop = true;
		pShinobu->Get_Model()->Play_Animation(fTimeDelta * 0.4f);
		Increase_Height(pShinobu, fTimeDelta);

		if (m_bNextAnim == true)
		{
			if (pShinobu->Get_Model()->Get_End(CShinobu_CinemaState::ANIM_SCENE_0_START))
			{
				pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_0_LOOP);
				pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_0_LOOP));
				m_bNextAnim = false;
				m_bAnimStop = false;
				pShinobu->Get_Model()->Set_End(CShinobu_CinemaState::ANIM_SCENE_0_START);
				Initialize_Value(pShinobu, fTimeDelta);
			}
		}
	}
	else if (pShinobu->Get_AnimIndex() == CShinobu_CinemaState::ANIM_SCENE_0_LOOP)
	{
		m_fTime += fTimeDelta;

		if (m_fTime >= 2.f)
		{
			g_bDeathTime = true;

			if (m_bIsHitMotion == false)
			{
				pShinobu->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_TYPE::HIT_BOUND, 20.f, 30.f, 0.f);
				m_bIsHitMotion = true;
			}

			Decrease_Height(pShinobu, fTimeDelta);

			if (m_bNextAnim == true)
			{
				pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu_CinemaState::ANIM_SCENE_0_END);
				pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CShinobu_CinemaState::ANIM_SCENE_0_END));
			}
		}
	}



	return nullptr;
}

void CShinobu_CinemaState::Increase_Height(CShinobu * pShinobu, _float fTimeDelta)
{
	static _float fJump_Velocity = 5.f;
	static _float fGravity = 9.8f;
	static _float3 vPosition;

	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));


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
		m_bNextAnim = true;
	}
	else
	{
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPosition);
	}



}

void CShinobu_CinemaState::Decrease_Height(CShinobu * pShinobu, _float fTimeDelta)
{
	if (g_bDeathTime == true)
		fTimeDelta *= 5.f;


	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fPositionY = pShinobu->Get_NavigationHeight().y;

	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	static _float fGravity = 10.f;


	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;


	m_vPosition.x += m_vVelocity.x * fTimeDelta * XMVectorGetX(m_vTargetPosition)* 3.f;
	m_vPosition.y += m_vVelocity.y * fTimeDelta * XMVectorGetY(m_vTargetPosition)* 3.f;
	m_vPosition.z += m_vVelocity.z * fTimeDelta * XMVectorGetZ(m_vTargetPosition)* 3.f;

	_vector vCurrentPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) < 2.f)
	{
		if (m_bControlMotion == false)
		{
			pShinobu->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_TYPE::HIT_BOUND, 20.f, 30.f, 2.f);
			m_bControlMotion = true;
		}
	}

	if (XMVectorGetY(vCurrentPos) < m_fPositionY)
	{
		_vector vPosition;
		m_vPosition.y = m_fPositionY;
		m_vVelocity.y = m_fPositionY;
		m_bOnGround = true;
		g_bDeathTime = false;
		m_bNextAnim = true;
		vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);




}

void CShinobu_CinemaState::Initialize_Value(CShinobu * pShinobu, _float fTimeDelta)
{

	m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;


	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);

}


