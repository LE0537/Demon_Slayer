#include "stdafx.h"
#include "..\Public\EnmuBoss.h"
#include "GameInstance.h"

#include "Enmu_Chaos_Head.h"
#include "Enmu_Left_Hand.h"
#include "Enmu_Right_Hand.h"
#include "Enmu_Chok.h"
#include "Enmu_Shield.h"

IMPLEMENT_SINGLETON(CEnmuBoss)

CEnmuBoss::CEnmuBoss()
{
}

HRESULT CEnmuBoss::Add_EnmuParts(CCharacters * pCharacter)
{
	if (pCharacter == nullptr)
		return E_FAIL;

	else if (pCharacter != nullptr)
		m_EnmuParts.push_back(pCharacter);

	return S_OK;
}



void CEnmuBoss::Free()
{
	
}
