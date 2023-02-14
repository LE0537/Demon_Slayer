#include "stdafx.h"
#include "RuiSkill_ShootNet.h"
#include "RuiIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiShoot.h"
#include "RuiTargetRushState.h"
#include "RuiSkill_Shoot.h"
#include "RuiSkill_ShootNet.h"
#include "RuiSkill_Sphere.h"
#include "RuiAdvSkill_CommonState.h"
#include "RuiAdvSkill_MoveState.h"
#include "RuiJumpState.h"
#include "RuiDashState.h"
#include "RuiAtk_1_State.h"
using namespace Rui;


CSkill_ShootNetState::CSkill_ShootNetState(STATE_TYPE eType)
{
	m_eStateType = eType;

}

CRuiState * CSkill_ShootNetState::HandleInput(CRui* pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	m_fDuration = pRui->Get_Model()->Get_Duration_Index(pRui->Get_AnimIndex());
	m_fCurrentDuration = pRui->Get_Model()->Get_CurrentTime_Index(pRui->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.6f)
	{
		switch (pRui->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_W)) // ╬у
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // аб
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
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

			else if (pGameInstance->Key_Pressing(DIK_S)) // ╣з
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // аб
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л 
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


			else if (pGameInstance->Key_Pressing(DIK_A)) // аб
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ©Л
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RIGHT);
			}
			break;
		case 2:
			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_UP)) // ╬у
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
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

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ╣з
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);


				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л 
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


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // аб
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ©Л
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RIGHT);

			}
			break;
		}
	}


	return CommandCheck(pRui);
}

CRuiState * CSkill_ShootNetState::Tick(CRui* pRui, _float fTimeDelta)
{
	pRui->Get_Model()->Set_Loop(pRui->Get_AnimIndex());

	if (pRui->Get_Model()->Get_End(pRui->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiState::TYPE_START:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_ShootNetState(CRuiState::TYPE_LOOP);
			break;
		case Client::CRuiState::TYPE_LOOP:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CSkill_ShootNetState(CRuiState::TYPE_END);
			break;
		case Client::CRuiState::TYPE_END:
			pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRui->Get_Model()->Set_End(pRui->Get_AnimIndex());
	}

	return nullptr;
}

CRuiState * CSkill_ShootNetState::Late_Tick(CRui* pRui, _float fTimeDelta)
{
	CCharacters* m_pTarget = pRui->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRui->Get_Transform()->LookAt(vLooAt);

	if (m_eStateType == CRuiState::TYPE_LOOP)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		m_fMove += fTimeDelta;

		CRuiShoot::RUISHOOTINFO	tInfo;
		tInfo.pPlayer = pRui;
		tInfo.pTarget = m_pTarget;

		if (m_iHit < 1 && pRui->Get_BattleTarget()->Get_GodMode() == false)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
			tInfo.iIndex = 0;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;
			tInfo.iIndex = 1;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;
			tInfo.iIndex = 2;
			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}

		RELEASE_INSTANCE(CGameInstance);
	}
	pRui->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	if (!m_bEffect && m_eStateType == CRuiState::TYPE_LOOP)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUISKL_SHOTNET_STARTEFF, pRui);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CSkill_ShootNetState::Enter(CRui* pRui)
{
	m_eStateId = STATE_ID::STATE_SKILL_SHOOTNET;

	switch (m_eStateType)
	{
	case Client::CRuiState::TYPE_START:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOTNET_0);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOTNET_0, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOTNET_0);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Rui_ShootNet.wav"), fEFFECT);
		break;
	case Client::CRuiState::TYPE_LOOP:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOTNET_1);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOTNET_1, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOTNET_1);
		break;
	case Client::CRuiState::TYPE_END:
		pRui->Get_Model()->Set_CurrentAnimIndex(CRui::ANIM_SKILL_SHOOTNET_2);
		pRui->Get_Model()->Set_LinearTime(CRui::ANIM_SKILL_SHOOTNET_2, 0.01f);
		pRui->Set_AnimIndex(CRui::ANIM_SKILL_SHOOTNET_2);
		break;
	}

}

void CSkill_ShootNetState::Exit(CRui* pRui)
{

}

CRuiState * CSkill_ShootNetState::CommandCheck(CRui * pRui)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pRui->Get_Model()->Get_Duration_Index(CRui::ANIM_SKILL_SHOOTNET_2);
	m_fCurrentDuration = pRui->Get_Model()->Get_CurrentTime_Index(CRui::ANIM_SKILL_SHOOTNET_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f)
	{
		switch (pRui->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_I)) // ╫╨еЁ е╟ 
			{
				if (pRui->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						pRui->Set_SkillBar(-200);
						return new CSkill_SphereState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pRui->Set_SkillBar(-200);
						return new CSkill_ShootNetState(TYPE_START); // move skill
					}
					else
					{
						pRui->Set_SkillBar(-200);
						return new CSkill_ShootState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_L))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_SPACE))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			else if (pGameInstance->Key_Pressing(DIK_U))
			{
				if (pRui->Get_PlayerInfo().iFriendBar >= 500)
				{
					if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pRui->Set_FriendSkillBar(-500);
						return new CAdvSkill_CommonState();
					}
				}
			}

			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_X)) // ╫╨еЁ е╟ 
			{
				if (pRui->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						pRui->Set_SkillBar(-200);
						return new CSkill_SphereState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						pRui->Set_SkillBar(-200);
						return new CSkill_ShootNetState(TYPE_START); // move skill
					}
					else
					{
						pRui->Set_SkillBar(-200);
						return new CSkill_ShootState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_LCONTROL))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			else if (pGameInstance->Key_Pressing(DIK_V))
			{
				if (pRui->Get_PlayerInfo().iFriendBar >= 500)
				{
					if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						pRui->Set_FriendSkillBar(-500);
						return new CAdvSkill_CommonState();
					}
				}
			}
		}
	}


	return nullptr;
}

