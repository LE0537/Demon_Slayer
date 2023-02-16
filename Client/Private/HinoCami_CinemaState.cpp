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
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_0); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_0:
		Scene_0(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_1); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_1:
		Scene_1(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_2); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_2:
		Scene_2(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_3); }
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_3:
		Scene_3(pTanjiro, fTimeDelta);
		if (m_bNextAnim == true) { return new CHinoCami_CinemaState(Tanjiro::CHinoCami_CinemaState::SCENE_4); }
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

		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(30.f, pTanjiro->Get_NavigationHeight().y, 75.f, 1.f));
		break;
	case Client::Tanjiro::CHinoCami_CinemaState::SCENE_0:
		pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Set_AnimIndex(static_cast<CTanjiro::ANIMID>(CHinoCami_CinemaState::ANIM_SCENE_0));
		pTanjiro->Get_Model()->Set_Loop(CHinoCami_CinemaState::ANIM_SCENE_0);
		pTanjiro->Get_Model()->Set_LinearTime(CHinoCami_CinemaState::ANIM_SCENE_0, 0.01f);
		pTanjiro->Get_Model()->Set_CurrentTime(41.f);
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
		//pTanjiro->Get_Model()->Reset_Anim(CHinoCami_CinemaState::ANIM_SCENE_4);
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
	pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
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

		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta, pTanjiro->Get_NavigationCom());

		if (fDistance < 17.f)
			m_bNextAnim = true;

	}

	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_0(CTanjiro * pTanjiro, _float fTimeDelta)
{
	//pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());

	
	//if(m_bAnimStop == false && pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) <= 42.f)
	//	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	m_bAnimStop = true;

	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));

	if (fDistance > 7.f)
	{
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
		}
	}


	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_1(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 5.5f && pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) <= 6.f)
	{
		if (m_bIsHitMotion == false)
		{
			pTanjiro->Get_BattleTarget()->Take_Damage(0.1f, false);
			m_bIsHitMotion = true;
		}
	}
	else if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 14.5f && pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) <= 15.f)
	{
		if (m_bIsHitMotion == true)
		{
			pTanjiro->Get_BattleTarget()->Take_Damage(0.1f, false);
			m_bIsHitMotion = false;
		}
	}
	else if (pTanjiro->Get_Model()->Get_CurrentTime_Index(pTanjiro->Get_AnimIndex()) >= 22.f)
		m_bNextAnim = true;



	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_2(CTanjiro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_3(CTanjiro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_4(CTanjiro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_5(CTanjiro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

CTanjiroState * CHinoCami_CinemaState::Scene_6(CTanjiro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}


