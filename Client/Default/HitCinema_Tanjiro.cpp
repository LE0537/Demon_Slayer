#include "stdafx.h"
#include "HitCinema_Tanjiro.h"
#include "GameInstance.h"


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
	return nullptr;
}

CRuiState * CHitCinema_Tanjiro::Late_Tick(CRui * pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Play_Animation_Skill(fTimeDelta);

	return nullptr;
}

void CHitCinema_Tanjiro::Enter(CRui * pRui)
{
}

void CHitCinema_Tanjiro::Exit(CRui * pRui)
{
}
