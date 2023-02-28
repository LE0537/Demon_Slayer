#include "stdafx.h"
#include "KyoujuroAtk_1_State.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroAtk_2_State.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"
#include "KyoujuroDashState.h"

#include "KyoujuroSkill_Common.h"
#include "KyoujuroSkill_DashSlash.h"
#include "KyoujuroSkill_DoubleUpper.h"
#include "KyoujuroJumpState.h"
#include "KyoujuroTargetRushState.h"
#include "KyoujuroSplSkrStartState.h"

using namespace Kyoujuro;


CAtk_1_State::CAtk_1_State()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CAtk_1_State::HandleInput(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pKyoujuro->Get_i1P())
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
		switch (pKyoujuro->Get_i1P())
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


	return CommandCheck(pKyoujuro);
}

CKyoujuroState * CAtk_1_State::Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Set_EffectTime(fTimeDelta);
	pKyoujuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_ATTACK_1);
	pKyoujuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIM_ATTACK_1, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fTime);


	if (m_bAtkCombo == true && m_fTime >= 33.f)
		return new CAtk_2_State();



	if (pKyoujuro->Get_Model()->Get_End(CKyoujuro::ANIM_ATTACK_1))
	{
		pKyoujuro->Get_Model()->Set_End(CKyoujuro::ANIM_ATTACK_1);
		return new CIdleState();
	}


	return nullptr;
}

CKyoujuroState * CAtk_1_State::Late_Tick(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pKyoujuro->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;

	if (m_fMove < 0.3f)
	{
		pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pKyoujuro->Get_NavigationCom());

		_vector vCollPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pKyoujuro->Get_SphereCollider();
		if (m_fMove > 0.1f && m_iHit == 0)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);
		
				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-30 * pKyoujuro->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if (pKyoujuro->Get_BattleTarget()->Get_GodMode() == false)
				{
					m_pTarget->Set_Hp(_int(-pKyoujuro->Get_PlayerInfo().iDmg * pKyoujuro->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.0f,false);
					pKyoujuro->Set_Combo(1);
					pKyoujuro->Set_ComboTime(0.f);
				}


				if (pKyoujuro->Get_BattleTarget()->Get_GodMode() == false)
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
				CSoundMgr::Get_Instance()->Effect_Stop(SOUND_EFFECT);
				CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_SE_Hit_Attack_0.wav"), fEFFECT);
			}

		}


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pKyoujuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pKyoujuro->Get_NavigationCom()->Cheak_Cell(vPos))
				pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pKyoujuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyoujuro->Get_NavigationCom());
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pKyoujuro->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKATK1, pKyoujuro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CAtk_1_State::Enter(CKyoujuro * pKyoujuro)
{
	m_eStateId = STATE_ID::STATE_ATK_1;

	pKyoujuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_ATTACK_1);
	pKyoujuro->Set_AnimIndex(CKyoujuro::ANIM_ATTACK_1);

	_uint iRand = rand() % 3;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_Attack1_1.wav"), fVOICE);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_Attack1_2.wav"), fVOICE);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_Attack1_3.wav"), fVOICE);

		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Nezuko_SE_Attack_0.wav"), fEFFECT);
}

void CAtk_1_State::Exit(CKyoujuro * pKyoujuro)
{
	m_pCollBox->Set_Dead(); //추가
}

CKyoujuroState * CAtk_1_State::CommandCheck(CKyoujuro * pKyoujuro)
{



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pKyoujuro->Get_Model()->Get_Duration();
	m_fCurrentDuration = pKyoujuro->Get_Model()->Get_CurrentTime();

	_float fRatio = m_fCurrentDuration / m_fDuration;

	switch (pKyoujuro->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_E))
		{
			//	pTanjiro->Get_BattleTarget()->Play_Scene();
			return new CSplSkrStartState(TYPE_START);
		}

		if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
		{
			if (pKyoujuro->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_O))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DoubleUpperState();
				}
				else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DashSlashState(); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
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
			return new CJumpState(TYPE_START, 0.f, 0.f);
		}
		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_RSHIFT))
		{
			//	pTanjiro->Get_BattleTarget()->Play_Scene();
			return new CSplSkrStartState(TYPE_START);
		}

		if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
		{
			if (pKyoujuro->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_C))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DoubleUpperState();
				}
				else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
					return new CSkill_DashSlashState(); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pKyoujuro->Set_SkillBar(-200);
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
			return new CJumpState(TYPE_START, 0.f, 0.f);
		}
		break;
	}



	return nullptr;
}

