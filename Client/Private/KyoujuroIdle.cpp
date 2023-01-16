#include "stdafx.h"
#include "KyoujuroIdle.h"
#include "GameInstance.h"

using namespace Kyoujuro;

CIdleState::CIdleState()
{
}

CKyoujuroState * CIdleState::HandleInput(CKyoujuro * pTanjiro)
{
	return nullptr;
}

CKyoujuroState * CIdleState::Tick(CKyoujuro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

CKyoujuroState * CIdleState::Late_Tick(CKyoujuro * pTanjiro, _float fTimeDelta)
{
	return nullptr;
}

void CIdleState::Enter(CKyoujuro * pTanjiro)
{
}

void CIdleState::Exit(CKyoujuro * pTanjiro)
{
}
