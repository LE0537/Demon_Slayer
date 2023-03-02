#include "stdafx.h"
#include "TanjiroKaguraAtk_3_State.h"
#include "TanjiroKaguraAtk_4_State.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"
#include "GameObj.h"
#include "TanjiroDashState.h"
#include "TanjiroJumpState.h"
#include "TanjiroKaguraSkill_Common.h"
#include "TanjiroKaguraSkill_Move.h"
#include "TanjiroKaguraSkill_Sphere.h"
#include "TanjiroDashState.h"
#include "TanjiroTargetRushState.h"
#include "TanjiroJumpState.h"
#include "TanjiroSplSkrStartState.h"
using namespace Tanjiro;


CAtk_3_KaguraState::CAtk_3_KaguraState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);

}

CTanjiroState * CAtk_3_KaguraState::HandleInput(CTanjiro * pTanjiro)
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
	return CommandCheck(pTanjiro);
}

CTanjiroState * CAtk_3_KaguraState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_EffectTime(fTimeDelta);

	pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_KAGURA_ATTACK_3);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_KAGURA_ATTACK_3, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;

	if (m_bAtkCombo == true && m_fTime >= 35.f)
		return new CAtk_4_KaguraState();
	


	if (pTanjiro->Get_Model()->Get_End(CTanjiro::ANIM_KAGURA_ATTACK_3))
	{
		pTanjiro->Get_Model()->Set_End(CTanjiro::ANIM_KAGURA_ATTACK_3);
		return new CIdleState();
	}

	return nullptr;
}

CTanjiroState * CAtk_3_KaguraState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	
	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);

	m_fMove += fTimeDelta;
	m_fDelay += fTimeDelta;
	if (m_fMove < 0.6f)
	{
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pTanjiro->Get_NavigationCom());

		_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //�߰�
		_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //�߰�
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //�߰�
		vCollPos.m128_f32[1] += 1.f; //�߰�
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //�߰�
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //�߰�
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
		if (m_fDelay > 0.2f && m_iHit < 2)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-40 * pTanjiro->Get_PlayerInfo().fPowerUp));
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
					m_pTarget->Set_Hp(_int(-15 * pTanjiro->Get_PlayerInfo().fPowerUp));
					m_pTarget->Take_Damage(0.3f, false);
					pTanjiro->Set_Combo(1);
					pTanjiro->Set_ComboTime(0.f);
				}
				if (pTanjiro->Get_BattleTarget()->Get_GodMode() == false)
				{
					CSoundMgr::Get_Instance()->PlayEffect(TEXT("Hinokami_SE_Hit_Attack_2_1.wav"), g_fEffect);
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


		if (pMyCollider2->Collision(pTargetCollider))
		{
			_float fSpeed = pTanjiro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			if(g_iLevel != LEVEL_BATTLEENMU)
				vPos.m128_f32[1] = 0.f;
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos))
				pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pTanjiro->Get_NavigationCom());
		}
	}

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOATT3_1, pTanjiro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_HINOATT3_2, pTanjiro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CAtk_3_KaguraState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_ATK_3;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_KAGURA_ATTACK_3);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_KAGURA_ATTACK_3);
	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Hinokami_SE_Attack_2.wav"), g_fEffect);
}

void CAtk_3_KaguraState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead(); //�߰�
}

CTanjiroState * CAtk_3_KaguraState::CommandCheck(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pTanjiro->Get_Model()->Get_Duration_Index(CTanjiro::ANIMID::ANIM_KAGURA_ATTACK_3);
	m_fCurrentDuration = pTanjiro->Get_Model()->Get_CurrentTime_Index(CTanjiro::ANIMID::ANIM_KAGURA_ATTACK_3);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	//printf_s("Ratio : %f \n", fRatio);
	//printf_s("Duration : %f \n", m_fDuration);
	//printf_s("current : %f \n", m_fCurrentDuration);

	//if (fRatio >= 0.7f)
	{
		switch (pTanjiro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E) && !pTanjiro->Get_StoryKey())
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pTanjiro->Get_PlayerInfo().iUnicCount > 0)
				{
					pTanjiro->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Pressing(DIK_I)) // ��ų Ű 
			{
				if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_SphereState();
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_MoveState(); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_CommonState();
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
			if (pGameInstance->Key_Pressing(DIK_RSHIFT) && !pTanjiro->Get_StoryKey())
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pTanjiro->Get_PlayerInfo().iUnicCount > 0)
				{
					pTanjiro->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Pressing(DIK_X)) // ��ų Ű 
			{
				if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_SphereState();
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_MoveState(); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_CommonState();
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

