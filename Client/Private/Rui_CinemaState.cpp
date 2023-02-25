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
	/*CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Key_Down(DIK_R) && !pRui->Get_StoryKey())
	{
	RELEASE_INSTANCE(CGameInstance);
	pRui->Get_BattleTarget()->Play_Scene();
	return new CRui_CinemaState(CRui_CinemaState::CINEMASCENE::SCENE_1);
	}
	if (pGameInstance->Key_Down(DIK_F) && !pRui->Get_StoryKey())
	{
	RELEASE_INSTANCE(CGameInstance);

	pRui->Get_BattleTarget()->Play_Scene();
	return new CRui_CinemaState(CRui_CinemaState::CINEMASCENE::SCENE_1);
	}
	RELEASE_INSTANCE(CGameInstance);*/
	

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
		m_fTime += fTimeDelta;

		if (m_fTime >= 2.67f)
			return new CRui_CinemaState(CRui_CinemaState::SCENE_1);

		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
		   pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());

			//	return new CRui_CinemaState(CRui_CinemaState::SCENE_1);
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

	switch (m_eScene)
	{
	case Client::Rui::CRui_CinemaState::SCENE_START:
		pRui->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_010);
		pRui->Get_Model()->Reset_Anim(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_START));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_START);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_START, 0.01f);
		
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.5183f, pRui->Get_NavigationHeight().y, 56.1f, 1.f));
		pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(56.56f, pRui->Get_NavigationHeight().y, 50.03f, 1.f));
		

		pRui->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(XMVectorSet(66.97f, pRui->Get_NavigationHeight().y, 44.423f, 1.f));
		pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		
		break;
	case Client::Rui::CRui_CinemaState::SCENE_0:
		pRui->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_020);
		pRui->Get_Model()->Reset_Anim(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_0));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_0);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_0, 0.01f);

		//pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, pRui->Get_NavigationHeight().y, 64.f, 1.f));
		//pRui->Get_BattleTarget()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, pRui->Get_NavigationHeight().y, 38.5f, 1.f));

		//pRui->Get_Transform()->Set_PlayerLookAt(pRui->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		//pRui->Get_BattleTarget()->Get_Transform()->Set_PlayerLookAt(pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		break;
	case Client::Rui::CRui_CinemaState::SCENE_1:
		pRui->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_040);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_1);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_1));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_1);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Rui::CRui_CinemaState::SCENE_2:
		pRui->Set_SkillType(CCharacters::SKILL_TYPE::SKILL_050);
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui_CinemaState::ANIM_SCENE_2);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CRui_CinemaState::ANIM_SCENE_2));
		pRui->Get_Model()->Set_Loop(CRui_CinemaState::ANIM_SCENE_2);
		pRui->Get_Model()->Set_LinearTime(CRui_CinemaState::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Rui::CRui_CinemaState::SCENE_3:
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

