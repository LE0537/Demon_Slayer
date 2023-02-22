#include "stdafx.h"
#include "AkazaSkill_Punch.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "AkazaDashState.h"
#include "AkazaAdvSkill_Common.h"
#include "AkazaAdvSkill_Move.h"
#include "AkazaSkill_Destroy.h"
#include "AkazaSkill_Punch.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaTargetRushState.h"
#include "AkazaJumpState.h"
#include "AkazaAtk_1_State.h"
#include "Camera_Dynamic.h"
using namespace Akaza;


CSkill_PunchState::CSkill_PunchState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CAkazaState * CSkill_PunchState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	m_fDuration = pAkaza->Get_Model()->Get_Duration_Index(CAkaza::ANIM_SKILL_PUNCH_2);
	m_fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIM_SKILL_PUNCH_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.6f)
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

CAkazaState * CSkill_PunchState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CSkill_PunchState(CAkazaState::TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CSkill_PunchState(CAkazaState::TYPE_END);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	return nullptr;
}

CAkazaState * CSkill_PunchState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	if (m_eStateType == CAkazaState::TYPE_START)
	{
		if (!m_bLook)
		{
			_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			XMStoreFloat4(&m_vLook, vLooAt);
			pAkaza->Get_Transform()->Set_PlayerLookAt(vLooAt);
			m_bLook = true;
		}
		pAkaza->Get_Transform()->Go_Straight(fTimeDelta * 2.5f, pAkaza->Get_NavigationCom());
	
		CCollider*	pMyCollider = pAkaza->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pAkaza->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 2.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
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
	else if (m_eStateType == CAkazaState::TYPE_LOOP)
	{
	
		m_fDelay += fTimeDelta;
		m_fMove += fTimeDelta;

		if (m_fMove < 1.f)
		{
			if (m_fDelay > 0.05f  && m_iHit < 4)
			{
				_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //�߰�
				_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //�߰�
				vCollPos += XMVector3Normalize(vCollLook) * 4.f; //�߰�
				vCollPos.m128_f32[1] = 1.f; //�߰�
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //�߰�
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //�߰�
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
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
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(_int(-pAkaza->Get_PlayerInfo().iDmg * pAkaza->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.1f, false);
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
						m_fDelay = 0.f;
						++m_iHit;
					}
				}
			}


		}
		
	}

	else if (m_eStateType == CAkazaState::TYPE_END)
	{
		m_fMove += fTimeDelta;
		if (m_fMove > 0.3f && m_fMove < 1.f)
		{
			if (!m_bHit)
			{
				_vector vCollPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //�߰�
				_vector vCollLook = pAkaza->Get_Transform()->Get_State(CTransform::STATE_LOOK); //�߰�
				vCollPos += XMVector3Normalize(vCollLook) * 4.f; //�߰�
				vCollPos.m128_f32[1] = 1.f; //�߰�
				m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //�߰�
				m_pCollBox->Get_Transform()->Set_PlayerLookAt(m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
				CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //�߰�
				CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-50 * pAkaza->Get_PlayerInfo().fPowerUp));
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if (pAkaza->Get_BattleTarget()->Get_GodMode() == false)
					{
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(_int(-30 * pAkaza->Get_PlayerInfo().fPowerUp));
						m_pTarget->Player_UpperDown(CCharacters::HIT_TYPE::HIT_KNOCKBACK, 20.f, 30.f, 5.f);
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

						m_bHit = true;
					}
				}
			}
		}
	}
	pAkaza->Get_Model()->Play_Animation(fTimeDelta);
	if (!m_bEffect && m_eStateType == TYPE_START)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_MOVE_STARTFOLLOWPLC, pAkaza);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_MOVE_RUNSMOKE, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	else if (!m_bEffect && m_eStateType == TYPE_LOOP)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_MOVE_MAIN, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	else if (!m_bEffect && m_eStateType == TYPE_END)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_AKZSKL_MOVE_FINAL, pAkaza);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}


	return nullptr;
}

void CSkill_PunchState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_SKILL_PUNCH;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_PUNCH_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_PUNCH_0, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_PUNCH_0);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Akaza_Punch.wav"), fVOICE);
		if (pAkaza->Get_BattleTarget()->Get_TargetState() == STATE_HIT)
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_Hit_MoveAttack.wav"), fEFFECT);
		else
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_SE_MoveAttack.wav"), fEFFECT);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_PUNCH_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_PUNCH_1, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_PUNCH_1);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_PUNCH_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_PUNCH_2, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_PUNCH_2);
		break;
	}

}

void CSkill_PunchState::Exit(CAkaza* pAkaza)
{
	m_pCollBox->Set_Dead();
}

CAkazaState * CSkill_PunchState::CommandCheck(CAkaza * pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pAkaza->Get_Model()->Get_Duration_Index(CAkaza::ANIM_SKILL_PUNCH_2);
	m_fCurrentDuration = pAkaza->Get_Model()->Get_CurrentTime_Index(CAkaza::ANIM_SKILL_PUNCH_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f)
	{
		switch (pAkaza->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();


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
			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

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
	}


	return nullptr;
}

