#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Cinema_Manager.h"

IMPLEMENT_SINGLETON(CCinema_Manager)

CCinema_Manager::CCinema_Manager()
{
}


CCinema_Manager::~CCinema_Manager()
{
}

HRESULT CCinema_Manager::Initialize()
{

	return S_OK;
}

void CCinema_Manager::Free()
{
}
