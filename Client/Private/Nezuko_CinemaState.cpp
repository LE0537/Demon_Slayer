#include "stdafx.h"
#include "..\Public\Nezuko_CinemaState.h"
#include "NezukoIdleState.h"
#include "GameInstance.h"

using namespace Nezuko;

CNezuko_CinemaState::CNezuko_CinemaState(CINEMASCENE eScene)
{
	m_eScene = eScene;
}

CNezukoState * CNezuko_CinemaState::HandleInput(CNezuko * pNezuko)
{
	return nullptr;
}

CNezukoState * CNezuko_CinemaState::Tick(CNezuko * pNezuko, _float fTimeDelta)
{
	return nullptr;
}

CNezukoState * CNezuko_CinemaState::Late_Tick(CNezuko * pNezuko, _float fTimeDelta)
{
	return nullptr;
}

void CNezuko_CinemaState::Enter(CNezuko * pNezuko)
{
	


}

void CNezuko_CinemaState::Exit(CNezuko * pNezuko)
{

}
