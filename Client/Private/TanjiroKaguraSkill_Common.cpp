#include "stdafx.h"
#include "TanjiroKaguraSkill_Common.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"

#include "TanjiroJumpState.h"
#include "TanjiroKaguraSkill_Common.h"
#include "TanjiroKaguraSkill_Move.h"
#include "TanjiroKaguraSkill_Sphere.h"
#include "TanjiroDashState.h"
#include "TanjiroTargetRushState.h"
#include "TanjiroJumpState.h"
using namespace Tanjiro;


CKaguraSkill_CommonState::CKaguraSkill_CommonState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CTanjiroState * CKaguraSkill_CommonState::HandleInput(CTanjiro * pTanjiro)
{

	return CommandCheck(pTanjiro);
}

CTanjiroState * CKaguraSkill_CommonState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		return new CIdleState();
	}


	return nullptr;
}

CTanjiroState * CKaguraSkill_CommonState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	if (!m_bLook)
	{
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vLook, vLooAt);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);
		m_bLook = true;
	}
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
	m_fMove += fTimeDelta;
	if (m_fMove < 0.6f)
	{
		if(m_fMove <= 0.4f)
			pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
		if (m_iHit < 1 && m_fMove > 0.4f)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가


			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(-80);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					m_pTarget->Set_Hp(-40);
					m_pTarget->Take_Damage(0.3f, false);
					pTanjiro->Set_Combo(1);
					pTanjiro->Set_ComboTime(0.f);
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
			}
		}
	}
	else if (m_fMove < 1.1f && m_fMove > 0.8f)
	{
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pTanjiro->Get_NavigationCom());
		if (!m_bHit)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
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
					m_pTarget->Set_GuardHp(-80);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					m_pTarget->Set_Hp(-40);
					m_pTarget->Take_Damage(0.5f, false);
					pTanjiro->Set_Combo(1);
					pTanjiro->Set_ComboTime(0.f);
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
			}
		}
	}

	if (pMyCollider2->Collision(pTargetCollider))
	{
		_float fSpeed = pTanjiro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

		_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
		_vector vMyLook = vTargetLook * -1.f;

		_vector vPow = XMVector3Dot(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

		_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

		vPos += vMyLook * (fSpeed - fSpeed * fPow);
		vTargetPos += vTargetLook * fSpeed * fPow;
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



	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CKaguraSkill_CommonState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_KAGURA_COMMON;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_COMMON);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_KAGURA_COMMON);

}

void CKaguraSkill_CommonState::Exit(CTanjiro * pTanjiro)
{
	m_pCollBox->Set_Dead();
}

CTanjiroState * CKaguraSkill_CommonState::CommandCheck(CTanjiro * pTanjiro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pTanjiro->Get_Model()->Get_Duration_Index(CTanjiro::ANIM_SKILL_KAGURA_COMMON);
	m_fCurrentDuration = pTanjiro->Get_Model()->Get_CurrentTime_Index(CTanjiro::ANIM_SKILL_KAGURA_COMMON);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	//printf_s("Ratio : %f \n", fRatio);
	//printf_s("Duration : %f \n", m_fDuration);
	//printf_s("current : %f \n", m_fCurrentDuration);

	if (fRatio >= 0.2f)
	{
		switch (pTanjiro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_SphereState();
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{


						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_MoveState(); // move skill

					}

					else
					{
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
			if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
			{
				if (pTanjiro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_SphereState();
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{


						pTanjiro->Set_SkillBar(-200);
						return new CKaguraSkill_MoveState(); // move skill

					}

					else
					{
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

