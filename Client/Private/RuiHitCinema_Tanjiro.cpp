#include "stdafx.h"
#include "RuiHitCinema_Tanjiro.h"
#include "GameInstance.h"
#include "RuiIdleState.h"

using namespace Rui;

CHitCinema_Tanjiro::CHitCinema_Tanjiro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CRuiState * CHitCinema_Tanjiro::HandleInput(CRui * pRui)
{
	return nullptr;
}

CRuiState * CHitCinema_Tanjiro::Tick(CRui * pRui, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Tanjiro::SCENE_START:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_0);
		}
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_0:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_1);
		}
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_1:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_2);
		}
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_2:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_3);
		}
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_3:
		if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
		{
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			if(pRui->Get_StoryDead())
				pRui->Player_UpperDown(CCharacters::HIT_BOUND, 20.f, 30.f, 0.f);
			else
				return new CIdleState();
		}
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_END:
		break;

	}

	return nullptr;
}

CRuiState * CHitCinema_Tanjiro::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	if (m_eScene == CHitCinema_Tanjiro::SCENE_2)
	{
		/*_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vRight = pRui->Get_Transform()->Get_State(CTransform::STATE_RIGHT);
		_vector vLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		vPos += XMVector3Normalize(vLook) * 3.f;
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);*/
	}
	pRui->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Tanjiro::Enter(CRui * pRui)
{
	switch (m_eScene)
	{
	case Client::Rui::CHitCinema_Tanjiro::SCENE_START:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_START));
		pRui->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_START, 0.01f);
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_0:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_0));
		pRui->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_0, 0.01f);
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_1:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_1));
		pRui->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_2:
		_vector vPos = pRui->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vRight = pRui->Get_Transform()->Get_State(CTransform::STATE_RIGHT);
		_vector vLook = pRui->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		vPos += XMVector3Normalize(vLook) * 0.46f;
		vPos += XMVector3Normalize(vRight) * 0.9f;
		pRui->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		pRui->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_2));
		pRui->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_3:
		pRui->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pRui->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pRui->Set_AnimIndex(static_cast<CRui::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_3));
		pRui->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pRui->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Rui::CHitCinema_Tanjiro::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Tanjiro::Exit(CRui * pRui)
{
}
