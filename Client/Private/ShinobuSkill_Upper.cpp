#include "stdafx.h"
#include "ShinobuSkill_Upper.h"
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


CSkill_UpperState::CSkill_UpperState(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	m_eStateType = eType;
	

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
	m_fDelay = 0.2f;
}

CShinobuState * CSkill_UpperState::HandleInput(CShinobu* pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	m_fDuration = pShinobu->Get_Model()->Get_Duration_Index(CShinobu::ANIM_SKILL_COMMON_2);
	m_fCurrentDuration = pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIM_SKILL_COMMON_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f)
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

CShinobuState * CSkill_UpperState::Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (pShinobu->Get_Model()->Get_End(pShinobu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CShinobuState::TYPE_START:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());

			if (m_bNextAnim == true)
			{
				return new CSkill_UpperState(STATE_TYPE::TYPE_LOOP);
			}
			break;
		case Client::CShinobuState::TYPE_END:
			pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pShinobu->Get_Model()->Set_End(pShinobu->Get_AnimIndex());
	}



	


	return nullptr;
}

CShinobuState * CSkill_UpperState::Late_Tick(CShinobu* pShinobu, _float fTimeDelta)
{
	if (m_eStateType == CShinobuState::TYPE_START)
	{
		CCharacters* m_pTarget = pShinobu->Get_BattleTarget();
	
		if (m_fTime > 0.4f)
			m_fMove += fTimeDelta;

		if (m_fMove < 0.3f)
		{
			//pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pShinobu->Get_NavigationCom());

			_vector vCollPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pShinobu->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.5f; //추가
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

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(-140 * pShinobu->Get_PlayerInfo().fPowerUp);
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if(pShinobu->Get_BattleTarget()->Get_GodMode() == false)
					{
						CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
						RELEASE_INSTANCE(CGameInstance);
						m_pTarget->Set_Hp(-70 * pShinobu->Get_PlayerInfo().fPowerUp);
						m_pTarget->Player_UpperDown(CCharacters::HIT_TYPE::HIT_UPPER_2, 20.f, 30.f, 2.f);
						CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
						dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
						RELEASE_INSTANCE(CGameInstance);

						//m_pTarget->Take_Damage(0.6f, true);
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
	pShinobu->Get_Model()->Play_Animation(fTimeDelta);

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		if (m_fTime <= 0.2f)
			Move(pShinobu, fTimeDelta);
		else
			Increase_Height(pShinobu, fTimeDelta);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		Fall_Height(pShinobu, fTimeDelta);
		if (m_bNextAnim == true)
		{
			return new CSkill_UpperState(TYPE_END);
		}
		break;
	}

	

	return nullptr;
}

void CSkill_UpperState::Enter(CShinobu* pShinobu)
{
	m_eStateId = STATE_ID::STATE_SKILL_UPPER;

	switch (m_eStateType)
	{
	case Client::CShinobuState::TYPE_START:
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_SKILL_UPPER_0);
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_SKILL_UPPER_1);
		pShinobu->Get_Model()->Reset_Anim(CShinobu::ANIM_SKILL_UPPER_2);
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_UPPER_0);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_UPPER_0, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_UPPER_0);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SKILL_UPPER_0, false);
		m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 10.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		pShinobu->Get_Transform()->Set_PlayerLookAt(pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Shinobu_Upper.wav"), fEFFECT);
		break;
	case Client::CShinobuState::TYPE_LOOP:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_UPPER_1);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_UPPER_1, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_UPPER_1);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SKILL_UPPER_1, true);
		m_vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		m_fGravity = 0.f;
		pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_fCurrentPosY = pShinobu->Get_NavigationHeight().y;
		break;
	case Client::CShinobuState::TYPE_END:
		pShinobu->Get_Model()->Set_CurrentAnimIndex(CShinobu::ANIM_SKILL_UPPER_2);
		pShinobu->Get_Model()->Set_LinearTime(CShinobu::ANIM_SKILL_UPPER_2, 0.01f);
		pShinobu->Set_AnimIndex(CShinobu::ANIM_SKILL_UPPER_2);
		pShinobu->Get_Model()->Set_Loop(CShinobu::ANIM_SKILL_UPPER_2, false);
		break;
	}


	

}

void CSkill_UpperState::Exit(CShinobu* pShinobu)
{
	m_pCollBox->Set_Dead();
}

void CSkill_UpperState::Move(CShinobu * pShinobu, _float fTimeDelta)
{
	
	_vector vMyPosition = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pShinobu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vMyPosition - vTargetPosition));


	if (fDistance < 3.f)
	{
		//pShinobu->Get_Transform()->Go_Straight(fTimeDelta, pShinobu->Get_NavigationCom());
		m_bNextAnim = true;



	}
	else
	{
		pShinobu->Get_Transform()->Go_Straight(fTimeDelta * 1.2f, pShinobu->Get_NavigationCom());
	}
}

CShinobuState * CSkill_UpperState::CommandCheck(CShinobu * pShinobu)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pShinobu->Get_Model()->Get_Duration_Index(CShinobu::ANIM_SKILL_UPPER_2);
	m_fCurrentDuration = pShinobu->Get_Model()->Get_CurrentTime_Index(CShinobu::ANIM_SKILL_UPPER_2);

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (fRatio >= 0.7f)
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


CShinobuState * CSkill_UpperState::Increase_Height(CShinobu * pShinobu, _float fTimeDelta)
{
	static _float fJump_Velocity = 10.f;
	static _float fGravity = 100.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(XMVectorGetX(vCurrentPos), m_vPosition.y, XMVectorGetZ(vCurrentPos), 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f)
	{

		//pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;
	}
	else
	{
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vCurrentPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
		


	return nullptr;
}

CShinobuState * CSkill_UpperState::Fall_Height(CShinobu * pShinobu, _float fTimeDelta)
{
	pShinobu->Set_NavigationHeight(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pShinobu->Get_NavigationHeight().y;
	pShinobu->Get_Transform()->Set_Jump(true);
	static _float fGravity = -80.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	fVelocity += fGravity * fTimeDelta;

	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vecPos = XMVectorSetX(vecPos, vPosition.x);
	vecPos = XMVectorSetY(vecPos, vPosition.y);
	vecPos = XMVectorSetZ(vecPos, vPosition.z);

	if (vPosition.y <= m_fCurrentPosY)// m_fCurrentPosY)
	{
		vPosition.y = m_fCurrentPosY;
		fVelocity = m_fCurrentPosY;
		pShinobu->Get_Transform()->Set_Jump(false);
		_vector vecPos = pShinobu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		
		vecPos = XMVectorSetY(vecPos, vPosition.y);
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vecPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
		
		m_bNextAnim = true;
	}
	else
	{
		if (pShinobu->Get_NavigationCom()->Cheak_Cell(vecPos))
			pShinobu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}

	return nullptr;
}

