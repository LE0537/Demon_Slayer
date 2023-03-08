#include "stdafx.h"
#include "ShinobuHitCinema_Tanjiro.h"
#include "GameInstance.h"
#include "ShinobuIdleState.h"

using namespace Shinobu;

CHitCinema_Tanjiro::CHitCinema_Tanjiro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CShinobuState * CHitCinema_Tanjiro::HandleInput(CShinobu* pShinobu)
{
	return nullptr;
}

CShinobuState * CHitCinema_Tanjiro::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_START:
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_0);
		}
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_0:
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_1);
		}
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_1:
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_2);
		}
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_2:
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_3);
		}
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_3:
		if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		{
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
		}
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_END:
		break;

	}

	return nullptr;
}

CShinobuState * CHitCinema_Tanjiro::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Tanjiro::Enter(CShinobu* pShinobu)
{
	switch (m_eScene)
	{
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_START:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_START));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_START, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_0:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_0));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_0, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_1:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_1));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_2:
		_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vRight = pShinobu->Get_Transform()->Get_State(CTransform::STATE_RIGHT);
		vPos += XMVector3Normalize(vRight) * 0.67f;
		pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_2));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_3:
		pShinobu->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pShinobu->Set_AnimIndex(static_cast<CShinobu::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_3));
		pShinobu->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pShinobu->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Shinobu::CHitCinema_Tanjiro::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Tanjiro::Exit(CShinobu* pShinobu)
{
	pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
}
