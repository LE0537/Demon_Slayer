#include "stdafx.h"
#include "TanjiroAtk_2_State.h"
#include "TanjiroIdleState.h"
#include "TanjiroAtk_3_State.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
#include "TanjiroDashState.h"
#include "UI_Manager.h"

#include "TanjiroJumpState.h"
#include "TanjiroSkill_Common.h"
#include "TanjiroSkill_WaterMill.h"
#include "TanjiroSkill_WindMill.h"
#include "TanjiroDashState.h"
#include "TanjiroTargetRushState.h"
#include "TanjiroJumpState.h"
using namespace Tanjiro;


CAtk_2_State::CAtk_2_State()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CTanjiroState * CAtk_2_State::HandleInput(CTanjiro * pTanjiro)
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
			if (pGameInstance->Key_Pressing(DIK_W)) // 앞
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // 좌
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 우
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

			else if (pGameInstance->Key_Pressing(DIK_S)) // 뒤
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // 좌
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LB);
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 우 
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


			else if (pGameInstance->Key_Pressing(DIK_A)) // 좌
			{

				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // 우
			{
				if (pGameInstance->Key_Pressing(DIK_L))
					return new CDashState(DIR_RIGHT);
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_UP)) // 앞
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LF);
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
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

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 뒤
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
				{

					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LB);


				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우 
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


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_LEFT);

			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
			{
				if (pGameInstance->Key_Pressing(DIK_LSHIFT))
					return new CDashState(DIR_RIGHT);

			}
			break;
		}
	}

	return CommandCheck(pTanjiro);
}

CTanjiroState * CAtk_2_State::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_ATTACK_2);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_ATTACK_2, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fComboDelay);


	if (m_bAtkCombo == true && m_fTime >= 20.f)
		return new CAtk_3_State();



	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_ATTACK_2))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_ATTACK_2);
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CAtk_2_State::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);

	m_fMove += fTimeDelta;
	if (m_fMove < 0.3f)
	{
		if (m_iHit < 1)
		{

			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] += 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-30 * pTanjiro->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						  pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if (pTanjiro->Get_BattleTarget()->Get_GodMode() == false)
				{
					m_pTarget->Set_Hp(_int(-pTanjiro->Get_PlayerInfo().iDmg * pTanjiro->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.3f, false);
					pTanjiro->Set_Combo(1);
					pTanjiro->Set_ComboTime(0.f);
				}

				if (pTanjiro->Get_BattleTarget()->Get_GodMode() == false)
				{
					_int iDest = rand() % 5;
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					switch (iDest)
					{
					case 0:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
						break;
					case 1:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTarget);
						break;
					case 2:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTarget);
						break;
					case 3:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTarget);
						break;
					case 4:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTarget);
						break;
					default:
						break;
					}


					RELEASE_INSTANCE(CEffect_Manager);

					++m_iHit;
				}
			}
		}
	}
	else if (m_fMove < 0.45f && m_fMove >= 0.3f)
	{
		if (!m_bHit)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] += 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-30 * pTanjiro->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						  pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if (pTanjiro->Get_BattleTarget()->Get_GodMode() == false)
				{
					m_pTarget->Set_Hp(_int(-pTanjiro->Get_PlayerInfo().iDmg * pTanjiro->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.f, false);
					pTanjiro->Set_Combo(1);
					pTanjiro->Set_ComboTime(0.f);
				}

				if (pTanjiro->Get_BattleTarget()->Get_GodMode() == false)
				{
					_int iDest = rand() % 5;
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					switch (iDest)
					{
					case 0:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
						break;
					case 1:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTarget);
						break;
					case 2:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTarget);
						break;
					case 3:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTarget);
						break;
					case 4:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTarget);
						break;
					default:
						break;
					}


					RELEASE_INSTANCE(CEffect_Manager);

					m_bHit = true;
				}
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 0.7f);

	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		
		pEffectManger->Create_Effect(CEffect_Manager::EFF_TANATTACK2_1, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_TANATTACK2_2, pTanjiro);
		if (g_iLevel != LEVEL_BATTLEENMU)
			pEffectManger->Create_Effect(CEffect_Manager::EFF_TANATTACK2_GROUND, pTanjiro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}

	return nullptr;
}

void CAtk_2_State::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_2;

	_uint iRand = rand() % 3;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_ATTACK_2);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_ATTACK_2);

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Attack2_1.wav"), fVOICE);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Attack2_2.wav"), fVOICE);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Attack2_3.wav"), fVOICE);

	if (pTanjiro->Get_BattleTarget()->Get_TargetState() == STATE_HIT)
	{
		_uint iRand2 = rand() % 5;

		if (iRand2 == 0)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Sword_Hit_0.wav"), fEFFECT);
		else if (iRand2 == 1)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Sword_Hit_1.wav"), fEFFECT);
		else if (iRand2 == 2)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Sword_Hit_2.wav"), fEFFECT);
		else if (iRand2 == 3)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Sword_Hit_3.wav"), fEFFECT);
		else if (iRand2 == 4)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Sword_Hit_4.wav"), fEFFECT);
	}
	else
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_SE_Attack_2.wav"), fEFFECT);
}

void CAtk_2_State::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead(); //추가
}

CTanjiroState * CAtk_2_State::CommandCheck(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pTanjiro->Get_Model()->Get_Duration();
	m_fCurrentDuration = pTanjiro->Get_Model()->Get_CurrentTime();

	_float fRatio = m_fCurrentDuration / m_fDuration;


	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
		{
			if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_O))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					return new CSkill_WindMillState(TYPE_START);
				}
				else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					return new CSkill_WaterMillState(TYPE_START); // move skill
				}

				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					return new CSkill_CommonState();
				}
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_L))
		{
			return new CTargetRushState(TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_SPACE))
		{
			return new CJumpstate(TYPE_START, 0.f, 0.f);
		}
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
		{
			if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_C))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					return new CSkill_WindMillState(TYPE_START);
				}
				else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					return new CSkill_WaterMillState(TYPE_START); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pTanjiro->Set_SkillBar(-200);
					return new CSkill_CommonState();
				}
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
		{
			return new CTargetRushState(TYPE_START);
		}
		else if (pGameInstance->Key_Pressing(DIK_LCONTROL))
		{
			return new CJumpstate(TYPE_START, 0.f, 0.f);
		}
		break;
	}



	return nullptr;
}

