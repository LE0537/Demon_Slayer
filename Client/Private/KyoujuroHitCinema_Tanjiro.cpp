#include "stdafx.h"
#include "KyoujuroHitCinema_Tanjiro.h"
#include "GameInstance.h"
#include "KyoujuroIdleState.h"

using namespace Kyoujuro;

CHitCinema_Tanjiro::CHitCinema_Tanjiro(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CKyoujuroState * CHitCinema_Tanjiro::HandleInput(CKyoujuro* pKyoujuro)
{
	return nullptr;
}

CKyoujuroState * CHitCinema_Tanjiro::Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_START:
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_0);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_0:
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_1);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_1:
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_2);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_2:
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CHitCinema_Tanjiro(CHitCinema_Tanjiro::SCENE_3);
		}
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_3:
		if (pKyoujuro->Get_Model()->Get_End(pKyoujuro->Get_AnimIndex()))
		{
			pKyoujuro->Get_Model()->Set_End(pKyoujuro->Get_AnimIndex());
			return new CIdleState();
		}
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_END:
		break;

	}

	return nullptr;
}

CKyoujuroState * CHitCinema_Tanjiro::Late_Tick(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Tanjiro::Enter(CKyoujuro* pKyoujuro)
{
	switch (m_eScene)
	{
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_START:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_START));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_START);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_START, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_0:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_0));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_0);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_0, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_1:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_1));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_1);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_1, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_2:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_2));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_2);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_2, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_3:
		pKyoujuro->Get_Model()->Reset_Anim(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pKyoujuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(CHitCinema_Tanjiro::ANIM_SCENE_3));
		pKyoujuro->Get_Model()->Set_Loop(CHitCinema_Tanjiro::ANIM_SCENE_3);
		pKyoujuro->Get_Model()->Set_LinearTime(CHitCinema_Tanjiro::ANIM_SCENE_3, 0.01f);
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_4:
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_5:
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_6:
		break;
	case Client::Kyoujuro::CHitCinema_Tanjiro::SCENE_END:
		break;
	default:
		break;
	}

}

void CHitCinema_Tanjiro::Exit(CKyoujuro* pKyoujuro)
{
}
