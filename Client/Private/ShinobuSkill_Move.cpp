#include "stdafx.h"
#include "ShinobuSkill_Move.h"
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
#include "ShinobuSplSkrStartState.h"
using namespace Shinobu;


CSkill_MoveState::CSkill_MoveState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	m_eStateType = eType;


	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CShinobuState * CSkill_MoveState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	m_fDuration = pShinobu->Get_Model()->Get_Duration_Index(CShinobu::ANIM_SKILL_COMMON_2);
	m_fCurrentDuration = pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIM_SKILL_COMMON_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f && pShinobu->Get_SubSkill() == 0)
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

CShinobuState * CSkill_MoveState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	pShinobu->Set_EffectTime(fTimeDelta);

	if (m_eStateType == TYPE_LOOP)
	{
		Move(pShinobu, fTimeDelta);
	}



	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			//pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			return new CSkill_MoveState(STATE_TYPE::TYPE_LOOP);
			break;
			//case Client::CShinobuState::TYPE_LOOP:
			//	pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			//	return new CSkill_MoveState(STATE_TYPE::TYPE_LOOP);
			//	break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			if (pShinobu->Get_SubSkill() != 0)
			{
				pShinobu->Set_Sub(true);
				pShinobu->Set_SubSkill(0);
			}
			return new CIdleState();
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}

	return nullptr;
}

CShinobuState * CSkill_MoveState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (m_eStateType == CShinobuState::TYPE_END)
	{
		CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vLooAt.m128_f32[1] = 0.f;
		pShinobu->Get_Transform()->LookAt(vLooAt);

		m_fMove += fTimeDelta;

		if (m_fMove < 0.3f)
		{
			_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pShinobu->Get_SphereCollider();
			if (m_fMove > 0.1f && !m_bHit)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().fGuardTime <= 0.f)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-100 * pShinobu->Get_PlayerInfo().fPowerUp));
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if (pShinobu->Get_BattleTarget()->Get_GodMode() == false)
					{
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_DOWN, 0.2f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(_int(- 50 * pShinobu->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.6f, false);
						CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_Hit_MoveSkill.wav"), fEFFECT);
						pShinobu->Set_Combo(1);
						pShinobu->Set_ComboTime(0.f);
					}
					if (pShinobu->Get_BattleTarget()->Get_GodMode() == false)
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


			if (pMyCollider2->Collision(pTargetCollider))
			{
				_float fSpeed = pShinobu->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

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
		}

	}

	pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	if (m_bNextAnim == true)
	{
		//	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
		//	{
		//		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
				//pShinobu->Get_Model()->Reset_Anim(pShinobu->Get_AnimIndex());
		return new CSkill_MoveState(STATE_TYPE::TYPE_END);
		//	}
	}

	CModel* pModel = pShinobu->Get_Model();
	_uint	iIndex = pShinobu->Get_AnimIndex();
	_float fCurrentAnimTimeRatio = pModel->Get_CurrentTime_Index(iIndex) /
		pModel->Get_Duration_Index(iIndex);


	if (m_eStateType == TYPE_LOOP)
	{
		if (fCurrentAnimTimeRatio < 0.2f)	//	0.08초 / 0.4초
			pShinobu->Set_Render(false);
		else if (fCurrentAnimTimeRatio < 0.5f)	//	0.18초 / 0.4초
			pShinobu->Set_Render(true);
		else if (fCurrentAnimTimeRatio < 0.7f)	//	0.28초 / 0.4초
			pShinobu->Set_Render(false);
		else if (fCurrentAnimTimeRatio < 1.f)	//	0.38초 / 0.4초
			pShinobu->Set_Render(true);
		else									//	0.38 ~ 0.4
			pShinobu->Set_Render(false);
	}


	if (!m_bEffect &&
		m_eStateType == TYPE_START)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SHINOBU_SKL_MOVE_START, pShinobu);
		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	else if (!m_bEffect &&
		m_eStateType == TYPE_LOOP)
	{
		if (0.5f > fCurrentAnimTimeRatio)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_SHINOBU_SKL_MAIN_1R, pShinobu);
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
			m_bEffect2_ForLoopAtk = false;
		}
	}
	else if (!m_bEffect2_ForLoopAtk &&
		m_eStateType == TYPE_LOOP)
	{
		if (0.5f < fCurrentAnimTimeRatio)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_SHINOBU_SKL_MAIN_2L, pShinobu);
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect2_ForLoopAtk = true;
			m_bEffect = false;
		}
	}
	else if (!m_bEffect &&
		m_eStateType == TYPE_END)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_SHINOBU_SKL_FINAL_NONFOL, pShinobu);
		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}


	return nullptr;
}

void CSkill_MoveState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_SKILL_MOVE;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_MOVE_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_MOVE_0, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_MOVE_0);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex(), false);
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Shinobu_MoveSkill.wav"), fVOICE);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_SE_MoveSkill.wav"), fEFFECT);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_MOVE_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_MOVE_1, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_MOVE_1);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex(), true);
		m_eStateId = STATE_ID::STATE_SKILL_MOVE_ALPHA;
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_MOVE_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_MOVE_2, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_MOVE_2);
		pShinobu->Get_Model()->Set_Loop(pShinobu->Get_AnimIndex(), false);
		break;
	}



}

void CSkill_MoveState::Exit(CShinobu* pShinobu)
{
	pShinobu->Set_Render(true);

	m_pCollBox->Set_Dead();
}

void CSkill_MoveState::Move(CShinobu * pShinobu, _float fTimeDelta)
{
	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));


	if (fDistance < 5.f)
	{
		//pShinobu->Get_Transform()->Go_Straight(fTimeDelta, pShinobu->Get_NavigationCom());
		m_bNextAnim = true;
	}
	else
	{
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 4.0f, pShinobu->Get_NavigationCom());
	}

}

CShinobuState * CSkill_MoveState::CommandCheck(CShinobu * pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pShinobu->Get_Model()->Get_Duration_Index(CShinobu::ANIM_SKILL_MOVE_2);
	m_fCurrentDuration = pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIM_SKILL_MOVE_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f && pShinobu->Get_SubSkill() == 0)
	{
		switch (pShinobu->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pShinobu->Get_PlayerInfo().iUnicCount > 0)
				{
					pShinobu->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

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
			if (pGameInstance->Key_Pressing(DIK_RSHIFT))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				if (pShinobu->Get_PlayerInfo().iUnicCount > 0)
				{
					pShinobu->Set_UnicCount(-1);
					return new CSplSkrStartState(TYPE_START);

				}
			}

			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

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

