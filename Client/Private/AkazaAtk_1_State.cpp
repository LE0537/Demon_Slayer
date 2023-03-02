#include "stdafx.h"
#include "AkazaAtk_1_State.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "AkazaDashState.h"
#include "AkazaAtk_2_State.h"
#include "AkazaAdvSkill_Common.h"
#include "AkazaAdvSkill_Move.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaSkill_Punch.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaTargetRushState.h"
#include "AkazaJumpState.h"
#include "AkazaSplSkrStartState.h"
using namespace Akaza;


CAtk_1_State::CAtk_1_State()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CAkazaState * CAtk_1_State::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pAkaza->Get_i1P())
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
		switch (pAkaza->Get_i1P())
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


	return CommandCheck(pAkaza);
}

CAkazaState * CAtk_1_State::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_EffectTime(fTimeDelta);

	pAkaza->Get_Model()->Set_Loop(CAkaza::ANIM_ATTACK_1);
	pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_ATTACK_1, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;
	//printf_s("AttackTime : %f \n", (_float)m_fTime);


	if (m_bAtkCombo == true && m_fTime >= 40.f)
		return new CAtk_2_State();



	if (pAkaza->Get_Model()->Get_End(CAkaza::ANIM_ATTACK_1))
	{
		pAkaza->Get_Model()->Set_End(CAkaza::ANIM_ATTACK_1);
		return new CIdleState();
	}


	return nullptr;
}

CAkazaState * CAtk_1_State::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pAkaza->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;
	if (m_fMove < 0.5f && m_fMove > 0.3f)
		pAkaza->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pAkaza->Get_NavigationCom());

	if (m_fMove < 0.5f)
	{
		_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //�߰�
		_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //�߰�
		vCollPos += XMVector3Normalize(vCollLook) * 2.f; //�߰�
		vCollPos.m128_f32[1] = 1.f; //�߰�
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //�߰�
		m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //�߰�
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pAkaza->Get_SphereCollider();
		if (m_fMove > 0.3f && m_iHit == 0)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);
		
				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-30 * pAkaza->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if(pAkaza->Get_BattleTarget()->Get_GodMode() == false)
				{
					m_pTarget->Set_Hp(_int(-pAkaza->Get_PlayerInfo().iDmg * pAkaza->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.f,false);
					pAkaza->Set_Combo(1);
					pAkaza->Set_ComboTime(0.f);
				}
				if (pAkaza->Get_BattleTarget()->Get_GodMode() == false)
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


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pAkaza->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPos))
				pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pAkaza->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pAkaza->Get_NavigationCom());
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZATK_1, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}

	return nullptr;
}

void CAtk_1_State::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_ATK_1;

	pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_ATTACK_1);
	pAkaza->Set_AnimIndex(CAkaza::ANIM_ATTACK_1);

	_uint iRand = rand() % 4;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_Attack1_1.wav"), g_fVoice);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_Attack1_2.wav"), g_fVoice);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_Attack1_3.wav"), g_fVoice);
	else if (iRand == 3)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_Attack1_4.wav"), g_fVoice);

	if (pAkaza->Get_BattleTarget()->Get_TargetState() == STATE_HIT)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_Hit_Attack_0.wav"), g_fEffect);
	else
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_Attack_0.wav"), g_fEffect);
}

void CAtk_1_State::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead(); //�߰�
}

CAkazaState * CAtk_1_State::CommandCheck(CAkaza * pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pAkaza->Get_Model()->Get_Duration_Index(pAkaza->Get_AnimIndex());
	m_fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(pAkaza->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;


	switch (pAkaza->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Pressing(DIK_E))
		{
			//	pTanjiro->Get_BattleTarget()->Play_Scene();
			if (pAkaza->Get_PlayerInfo().iUnicCount > 0)
			{
				pAkaza->Set_UnicCount(-1);
				return new CSplSkrStartState(TYPE_START);

			}
		}

		if (pGameInstance->Key_Pressing(DIK_I)) // ��ų Ű 
		{
			if (pAkaza->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_O))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
					return new CSkill_DestoryState(TYPE_START);
				}
				else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
					return new CSkill_PunchState(TYPE_START); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 0);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
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
			if (pAkaza->Get_PlayerInfo().iFriendBar >= 500)
			{
				if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
				{
					pAkaza->Set_FriendSkillBar(-500);
					return new CAdvSkill_MoveState(TYPE_START);
				}
				else
				{
					pAkaza->Set_FriendSkillBar(-500);
					return new CAdvSkill_CommmonState(TYPE_START);
				}
			}
		}

		break;
	case 2:
		if (pGameInstance->Key_Pressing(DIK_RSHIFT))
		{
			//	pTanjiro->Get_BattleTarget()->Play_Scene();
			if (pAkaza->Get_PlayerInfo().iUnicCount > 0)
			{
				pAkaza->Set_UnicCount(-1);
				return new CSplSkrStartState(TYPE_START);

			}
		}

		if (pGameInstance->Key_Pressing(DIK_X)) // ��ų Ű 
		{
			if (pAkaza->Get_PlayerInfo().iSkBar >= 200)
			{
				if (pGameInstance->Key_Pressing(DIK_C))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
					return new CSkill_DestoryState(TYPE_START);
				}
				else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
					return new CSkill_PunchState(TYPE_START); // move skill
				}
				else
				{
					CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
					pUI_Manager->Set_UseSkillCount(1, 1);
					RELEASE_INSTANCE(CUI_Manager);
					pAkaza->Set_SkillBar(-200);
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
			if (pAkaza->Get_PlayerInfo().iFriendBar >= 500)
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
				{
					pAkaza->Set_FriendSkillBar(-500);
					return new CAdvSkill_MoveState(TYPE_START);
				}
				else
				{
					pAkaza->Set_FriendSkillBar(-500);
					return new CAdvSkill_CommmonState(TYPE_START);
				}
			}
		}
	}



	return nullptr;
}

