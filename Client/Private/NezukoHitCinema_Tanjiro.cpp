#include "stdafx.h"
#include "NezukoHitCinema_Tanjiro.h"
#include "GameInstance.h"
#include "NezukoIdleState.h"

using namespace Nezuko;

CHitCinema_Tanjiro::CHitCinema_Tanjiro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CNezukoState * CHitCinema_Tanjiro::HandleInput(CNezuko* pNezuko)
{
	return nullptr;
}

CNezukoState * CHitCinema_Tanjiro::Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_START:
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_0);
		}
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_0:
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_1);
		}
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_1:
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_2);
		}
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_2:
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_3);
		}
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_3:
		if (pNezuko->Get_Model()->Get_End(pNezuko->Get_AnimIndex()))
		{
			pNezuko->Get_Model()->Set_End(pNezuko->Get_AnimIndex());
			return new CIdleState();
		}
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_END:
		break;

	}

	return nullptr;
}

CNezukoState * CHitCinema_Tanjiro::Late_Tick(CNezuko* pNezuko, _float fTimeDelta)
{
	pNezuko->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Tanjiro::Enter(CNezuko* pNezuko)
{
	switch (m_eScene)
	{
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_START:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_START));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_START, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_0:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_0));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_0, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_1:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_1));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_2:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_2));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_3:
		pNezuko->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pNezuko->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pNezuko->Set_AnimIndex(static_cast<CNezuko::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_3));
		pNezuko->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pNezuko->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Nezuko::CHitCinema_Tanjiro::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Tanjiro::Exit(CNezuko* pNezuko)
{
}
