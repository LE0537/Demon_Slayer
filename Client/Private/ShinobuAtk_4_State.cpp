#include "stdafx.h"
#include "ShinobuAtk_4_State.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "ShinobuDashState.h"
#include "ShinobuSkill_Common.h"
#include "ShinobuSkill_Move.h"
#include "ShinobuSkill_Upper.h"
#include "ShinobuTargetRushState.h"
#include "ShinobuJumpState.h"
#include "Camera_Dynamic.h"
using namespace Shinobu;


CAtk_4_State::CAtk_4_State()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CShinobuState * CAtk_4_State::HandleInput(CShinobu* pShinobu)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_fComboDelay <= 35.f)
	{
		switch (pShinobu->Get_i1P())
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
	return CommandCheck(pShinobu);
}

CShinobuState * CAtk_4_State::Tick(CShinobu* pShinobu, _float fTimeDelta)
{

	pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_ATTACK_4);
	pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_ATTACK_4, 0.01f);


	if (pShinobu->Get_Model()->Get_End(CShinobu::ANIM_ATTACK_4))
	{
		pShinobu->Get_Model()->Set_End(CShinobu::ANIM_ATTACK_4);
		return new CIdleState();
	}

	m_fComboDelay += fTimeDelta * 60.f;


	return nullptr;
}

CShinobuState * CAtk_4_State::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pShinobu->Get_Transform()->LookAt(vLooAt);
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	CCollider*	pMyCollider2 = pShinobu->Get_SphereCollider();
	m_fMove += fTimeDelta;
	m_fDelay += fTimeDelta;
	if (m_fMove < 0.5f)
	{
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.2f, pShinobu->Get_NavigationCom());

		_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
	
		if (m_fMove > 0.1f && m_iHit < 3 && m_fDelay > 0.1f)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(-30 * pShinobu->Get_PlayerInfo().fPowerUp);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					m_pTarget->Set_Hp(-pShinobu->Get_PlayerInfo().iDmg * pShinobu->Get_PlayerInfo().fPowerUp);
					m_pTarget->Set_Atk2(true);
					m_pTarget->Take_Damage(0.1f, false);
					pShinobu->Set_Combo(1);
					pShinobu->Set_ComboTime(0.f);
				}

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
				m_fDelay = 0.f;
			}

		}
	}
	else if (m_fMove > 0.9f && m_fMove < 1.2f)
	{
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.5f, pShinobu->Get_NavigationCom());

		_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pShinobu->Get_SphereCollider();
		if (!m_bHit&& m_fDelay > 0.1f)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(-50 * pShinobu->Get_PlayerInfo().fPowerUp);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(-pShinobu->Get_PlayerInfo().iDmg * 3 * pShinobu->Get_PlayerInfo().fPowerUp);
					m_pTarget->Set_Atk2(false);
					m_pTarget->Take_Damage(0.6f, false);
					pShinobu->Set_Combo(1);
					pShinobu->Set_ComboTime(0.f);
				}

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
				m_fDelay = 0.f;
			}

		}
	}
	if (pMyCollider2->Collision(pTargetCollider))
	{
		_float fSpeed = pShinobu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;

		_vector vPow = XMVector3Dot(pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
		vPos.m128_f32[1] = 0.f;
		_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
			m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		else
			pShinobu->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pShinobu->Get_NavigationCom());
	}


	pShinobu->Get_Model()->Play_Animation(fTimeDelta);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	//	pEffectManger->Create_Effect(CEffect_Manager::EFF_TANATTACK4, pShinobu);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CAtk_4_State::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_ATK_4;

	pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIMID::ANIM_ATTACK_4);
	pShinobu->Set_AnimIndex(CShinobu::ANIM_ATTACK_4);

	pShinobu->Get_Model()->Set_FrameNum(pShinobu->Get_AnimIndex(), 100);
	
	_uint iRand = rand() % 3;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_Attack4_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_Attack4_2.wav"), fEFFECT);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_Attack4_3.wav"), fEFFECT);

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("KnifeEff1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("KnifeEff2.wav"), fEFFECT);
}

void CAtk_4_State::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead(); //추가
}

CShinobuState * CAtk_4_State::CommandCheck(CShinobu * pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	//m_fDuration = pShinobu->Get_Model()->Get_Duration_Index();
	//m_fCurrentDuration = pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIM_SKILL_COMMON_2);

	//_float fRatio = m_fCurrentDuration / m_fDuration;

	//if (fRatio >= 0.7f)
	{
		switch (pShinobu->Get_i1P())
		{
		case 1:
	

			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pShinobu->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pShinobu->Set_SkillBar(-200);
						return new CSkill_UpperState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pShinobu->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pShinobu->Set_SkillBar(-200);
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
				if (pShinobu->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pShinobu->Set_SkillBar(-200);
						return new CSkill_UpperState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);

						pShinobu->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill

					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pShinobu->Set_SkillBar(-200);
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

	}

	return nullptr;
}

