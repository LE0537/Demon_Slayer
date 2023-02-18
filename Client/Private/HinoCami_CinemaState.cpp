#include "stdafx.h"
#include "..\Public\HinoCami_CinemaState.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"

using namespace Tanjiro;

CHinoCami_CinemaState::CHinoCami_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;

}

CTanjiroState * CHinoCami_CinemaState::HandleInput(CTanjiro * pTanjiro)
{
	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	
	switch (m_eScene)
	{
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_START:
		Scene_Start(pTanjiro, fTimeDelta);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_2);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_0); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_0:
		Scene_0(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_1); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_1:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_0);
		Scene_1(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_2); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_2:
		Scene_2(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_3); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_3:
		Scene_3(pTanjiro, fTimeDelta);
		//return new CIdleState();
		//if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_4); }
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_5); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_4:
		Scene_4(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_5); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_5:
		Scene_5(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_6); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_6:
		Scene_6(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CIdleState(); }
		break;
	}




	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if(m_bAnimStop == false)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CHinoCami_CinemaState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_SKILL_KAGURA_COMMON;

	pTanjiro->Set_SplSkl(true);
	
	switch (m_eScene)
	{
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_START:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SCENE_START));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_START);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_START, 0.01f);

		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pTanjiro->Get_NavigationHeight().y, 64.f, 1.f));
		pTanjiro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pTanjiro->Get_NavigationHeight().y, 38.5f, 1.f));

		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pTanjiro->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		break; 
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_0:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SCENE_0));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_0, 0.01f);
		
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_1:

		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SCENE_1));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_1);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_2:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SCENE_2));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_2);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_3:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SCENE_3));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_3);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_4:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_4);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_4);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SCENE_4));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_4, true);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_4, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_5:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_5);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_5);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SCENE_5));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_5);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_5, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_6:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SECNE_6);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SECNE_6);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SECNE_6));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SECNE_6);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SECNE_6, 0.01f);
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_END:
		break;
	}



}

void CHinoCami_CinemaState::Exit(CTanjiro * pTanjiro)
{

}

CTanjiroState * CHinoCami_CinemaState::Scene_Start(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (pTanjiro->Get_Model()->Get_End(CHinoCami_CinemaState::ANIM_SCENE_START))
	{
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_MOVE_LOOP);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_LOOP);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIMID::ANIM_MOVE_LOOP, true);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIMID::ANIM_MOVE_LOOP, 0.01f);

		pTanjiro->Get_Model()->Set_End(CHinoCami_CinemaState::ANIM_SCENE_START);
	}

	if (pTanjiro->Get_AnimIndex() == CTanjiro::ANIMID::ANIM_MOVE_LOOP)
	{
		_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));

		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());

		if (fDistance < 20.f)
			m_bNextAnim = true;

	}

	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_0(CTanjiro * pTanjiro, _float fTimeDelta)
{
	//pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());

	
	//if(m_bAnimStop == false && pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) <= 42.f)
	//	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));

	if (fDistance > 10.f)
	{
		if (m_bAnimStop == false)
		{
			pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
			pTanjiro->Get_Model()->Set_CurrentTime(pTanjiro->Get_AnimIndex(), 41.f);
			m_bAnimStop = true;
		}

	
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 0.3f);
		
		
	
		pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta, pTanjiro->Get_NavigationCom());
	}
	else
	{
		m_bAnimStop = false;                   
		if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 60.f)
		{
			pTanjiro->Get_BattleTarget()->Take_Damage(0.1f, false);
			m_bNextAnim = true;
			pTanjiro->Get_Model()->Set_CurrentTime(pTanjiro->Get_AnimIndex(), 69.f);
			pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
		}
	}


	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_1(CTanjiro * pTanjiro, _float fTimeDelta)
{
	//_float fTest = pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex());
	//printf("CurrentTime : %f \n", fTest);

	if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 5.5f && m_bIsHitMotion == false)
	{
		if (m_bIsHitMotion == false)
		{
			pTanjiro->Get_BattleTarget()->Take_Damage(0.1f, false);
			m_bIsHitMotion = true;
			m_bControlMotion = true;
		} 
	}
	else if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 14.5f && m_bControlMotion == true)
	{
		if (m_bControlMotion == true)
		{
			pTanjiro->Get_BattleTarget()->Take_Damage(0.1f, false);
			m_bControlMotion = false;
		}
	}
	else if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 22.f)
	{
		pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex()); 
		m_bNextAnim = true;
	}



	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_2(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fTest = pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex());
	printf("CurrentTime : %f \n", fTest);
	if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 5.5f && m_bIsHitMotion == false)
	{
		if (m_bIsHitMotion == false)
		{
			pTanjiro->Get_BattleTarget()->Take_Damage(0.1f, false);
			m_bIsHitMotion = true;
			m_vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			m_vDirection = XMVector3Normalize(m_vTargetPosition - m_vMyPosition);
		}
	}
	else if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 10.f)
	{
		
		m_bAnimStop = true;
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 0.2f);
		


		m_vMyPosition += m_vDirection * 0.3f;

		m_fTime += fTimeDelta;


		if (m_fTime >= 1.f)
		{
			//pTanjiro->Get_BattleTarget()->Player_UpperDown(CCharacters::HIT_TYPE::HIT_UPPER_2, 20.f, 20.f, 0.f);
			m_bNextAnim = true;
			m_fTime = 0.f;
		}

		if(m_fTime >= 0.1f)
			Increase_Height(pTanjiro, fTimeDelta);


		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, m_vMyPosition);
	}


	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_3(CTanjiro * pTanjiro, _float fTimeDelta)
{


	m_vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vDirection = XMVector3Normalize(m_vTargetPosition - m_vMyPosition);

	pTanjiro->Get_Transform()->LookAt(m_vTargetPosition);
	m_vMyPosition += m_vDirection * 0.85f;
	pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, m_vMyPosition);

	
	if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 22.f && m_bIsHitMotion == false)
	{
		m_bNextAnim = true;
	}


	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_4(CTanjiro * pTanjiro, _float fTimeDelta)
{


	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_5(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if (m_bIsHitMotion == false)
	{
		// ÅºÁö·Î 57.932f, 1.f, 43.087f
		// »ó´ë 59.838f, 0.f, 45.601f
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(57.932f, 11.f, 43.087f, 1.f));
		pTanjiro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(59.838f, 10.f, 45.601f, 1.f));
		pTanjiro->Get_Model()->Set_CurrentTime(pTanjiro->Get_AnimIndex(), 13.5f);
		m_bAnimStop = true;
		m_bIsHitMotion = true;
	}

	if (m_bAnimStop == true)
	{
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 0.1f);
	}


	if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 40.f)
	{
		int a = 0;
	}




	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_6(CTanjiro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

void CHinoCami_CinemaState::Increase_Height(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fJump_Velocity = 10.f;
	static _float fGravity = 70.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));


	fJump_Velocity += fGravity * fTimeDelta;
	
	vPosition.y += fJump_Velocity * fTimeDelta;


	//m_vPosition.x += m_vVelocity.x * XMVectorGetX(vLookAt) *  fTimeDelta;
	//m_vPosition.z += m_vVelocity.z * XMVectorGetZ(vLookAt) *  fTimeDelta;

	//_vector vCurrentPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vecPosition = XMVectorSet(vPosition.x, vPosition.y, vPosition.z, 1.f);

	if (XMVectorGetY(vecPosition) > 10.f)
	{
		/*	fDelay += fTimeDelta;

		if(fDelay >= 0.3f)*/
		//m_bNextAnim = true;
	}
	else
	{
		pTanjiro->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPosition);
	}
}


