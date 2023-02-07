#include "stdafx.h"
#include "ShinobuSkill_Common.h"
#include "ShinobuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"


#include "ShinobuSkill_Common.h"
#include "ShinobuSkill_Move.h"
#include "ShinobuSkill_Upper.h"
#include "ShinobuTargetRushState.h"
#include "ShinobuAtk_1_State.h"
#include "ShinobuDashState.h"
#include "ShinobuJumpState.h"
#include "Camera_Dynamic.h"
using namespace Shinobu;


CSkill_CommonState::CSkill_CommonState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CShinobuState * CSkill_CommonState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	m_fDuration = pShinobu->Get_Model()->Get_Duration_Index(CShinobu::ANIM_SKILL_COMMON_2);
	m_fCurrentDuration = pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIM_SKILL_COMMON_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.85f)
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

CShinobuState * CSkill_CommonState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex());

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
		return new CIdleState();

	}

	if (pShinobu->Get_Model()->Get_CurrentTime() <= 31.f)
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.1f, pShinobu->Get_NavigationCom());

	if(pShinobu->Get_Model()->Get_CurrentTime() >= 40.f && pShinobu->Get_Model()->Get_CurrentTime() <= 50.f)
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.1f , pShinobu->Get_NavigationCom());


	return nullptr;
}

CShinobuState * CSkill_CommonState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pShinobu->Get_Transform()->LookAt(vLooAt);
	CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
	CCollider*	pMyCollider2 = pShinobu->Get_SphereCollider();
	m_fMove += fTimeDelta;
	
	if (m_fMove > 0.4f && m_fMove < 1.f)
	{
		m_fHitTime += fTimeDelta;
		_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		
		if (m_fHitTime > 0.1f && m_iHit < 5)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.1f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(-15);
					m_pTarget->Take_Damage(0.1f, false);
					pShinobu->Set_Combo(1);
					pShinobu->Set_ComboTime(0.f);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);
				m_fHitTime = 0.f;
				++m_iHit;
			}

		}
	}
	if (m_fMove > 1.5f && m_fMove < 1.9f)
	{
		_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가

		if (!m_bHit)
		{
			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard)
				{
					m_pTarget->Get_GuardHit(0);
				}
				else
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_DOWN, 0.2f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(-30);
					m_pTarget->Take_Damage(0.7f, false);
					pShinobu->Set_Combo(1);
					pShinobu->Set_ComboTime(0.f);
				}

				CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

				pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

				RELEASE_INSTANCE(CEffect_Manager);

				m_bHit = true;
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

	return nullptr;
}

void CSkill_CommonState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_SKILL_COMMON;

	pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_COMMON_2);
	pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_COMMON_2, 0.01f);
	pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_COMMON_2);
	pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CSkill_CommonState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead();
}

CShinobuState * CSkill_CommonState::CommandCheck(CShinobu * pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pShinobu->Get_Model()->Get_Duration_Index(CShinobu::ANIM_SKILL_COMMON_2);
	m_fCurrentDuration = pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIM_SKILL_COMMON_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.85f)
	{
		switch (pShinobu->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pShinobu->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_UpperState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill
					}
					else
					{
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
			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
			{
				if (pShinobu->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						pShinobu->Set_SkillBar(-200);
						return new CSkill_UpperState(TYPE_START);
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{


						pShinobu->Set_SkillBar(-200);
						return new CSkill_MoveState(TYPE_START); // move skill

					}
					else
					{
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

