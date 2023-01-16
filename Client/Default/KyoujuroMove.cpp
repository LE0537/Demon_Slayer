#include "stdafx.h"
#include "KyoujuroMove.h"
#include "GameInstance.h"

using namespace Kyoujuro;


CMoveState::CMoveState()
{
}

CKyoujuroState * CMoveState::HandleInput(CKyoujuro * pTanjiro)
{
	return nullptr;
}

CKyoujuroState * CMoveState::Tick(CKyoujuro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

CKyoujuroState * CMoveState::Late_Tick(CKyoujuro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

void CMoveState::Enter(CKyoujuro * pTanjiro)
{
}

void CMoveState::Exit(CKyoujuro * pTanjiro)
{
}
