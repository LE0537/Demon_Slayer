#include "stdafx.h"
#include "TanjiroKaguraAtk_1_State.h"
#include "TanjiroKaguraAtk_2_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
#include "GameObj.h"
#include "TanjiroDashState.h"

using namespace Tanjiro;


CAtk_1_KaguraState::CAtk_1_KaguraState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);

}

CTanjiroState * CAtk_1_KaguraState::HandleInput(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_J) && m_fComboDelay <= 43.f)
			m_bAtkCombo = true;
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_Z) && m_fComboDelay <= 43.f)
			m_bAtkCombo = true;
		break;
	default:
		break;
	}
	
	if (m_fComboDelay <= 35.f)
	{
		switch (pTanjiro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_W)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ��
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ��
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RF);
				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_STRAIGHT);
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_S)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ��
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // �� 
				{

					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RB);

				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_BACK);
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_A)) // ��
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RIGHT);
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_UP)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RF);


				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_STRAIGHT);
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);


				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // �� 
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RB);

				}
				else
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_BACK);

				}
			}


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RIGHT);

			}
			break;
		}
	}
	return nullptr;
}

CTanjiroState * CAtk_1_KaguraState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_KAGURA_ATTACK_1);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_KAGURA_ATTACK_1, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;

	if (m_bAtkCombo == true && m_fTime >= 33.f)
		return new CAtk_2_KaguraState();
	


	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_KAGURA_ATTACK_1))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_KAGURA_ATTACK_1);
		return new CIdleState();
	}

	return nullptr;
}

CTanjiroState * CAtk_1_KaguraState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	


	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CAtk_1_KaguraState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_1;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_KAGURA_ATTACK_1);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_KAGURA_ATTACK_1);

}

void CAtk_1_KaguraState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead(); //�߰�
}

