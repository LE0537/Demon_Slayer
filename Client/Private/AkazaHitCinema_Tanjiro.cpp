#include "stdafx.h"
#include "AkazaHitCinema_Tanjiro.h"
#include "GameInstance.h"
#include "AkazaIdleState.h"

using namespace Akaza;

CHitCinema_Tanjiro::CHitCinema_Tanjiro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CAkazaState * CHitCinema_Tanjiro::HandleInput(CAkaza* pAkaza)
{
	return nullptr;
}

CAkazaState * CHitCinema_Tanjiro::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_START:
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_0);
		}
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_0:
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_1);
		}
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_1:
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_2);
		}
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_2:
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_3);
		}
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_3:
		if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
		{
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
		}
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_END:
		break;

	}

	return nullptr;
}

CAkazaState * CHitCinema_Tanjiro::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Tanjiro::Enter(CAkaza* pAkaza)
{
	switch (m_eScene)
	{
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_START:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_START));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_START, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_0:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_0));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_0, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_1:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_1));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_2:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_2));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_3:
		pAkaza->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pAkaza->Set_AnimIndex(static_cast<CAkaza::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_3));
		pAkaza->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pAkaza->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Akaza::CHitCinema_Tanjiro::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Tanjiro::Exit(CAkaza* pAkaza)
{
}
