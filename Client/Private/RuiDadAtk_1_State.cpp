#include "stdafx.h"
#include "RuiDadAtk_1_State.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiDadDashState.h"


using namespace RuiDad;


CAtk_1_State::CAtk_1_State(STATE_TYPE eType)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);

	m_eStateType = eType;

	
}

CRuiDadState * CAtk_1_State::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CRuiDadState * CAtk_1_State::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START: // 1타
			if(m_bNextAnim)
				return new CIdleState();
				//return new CAtk_1_State(TYPE_LOOP);
			break;
		case Client::CRuiDadState::TYPE_LOOP: // 2타
			if (m_bNextAnim)
				return new CIdleState();
				//return new CAtk_1_State(TYPE_END);
			break;
		case Client::CRuiDadState::TYPE_END: // 3타
			if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIMID::ANIM_ATTACK_2))
			{
				pRuiDad->Get_Model()->Set_End(CRuiDad::ANIMID::ANIM_ATTACK_2);
				return new CIdleState();
			}
			break;
		}


	m_fDuration = pRuiDad->Get_Model()->Get_Duration_Index(pRuiDad->Get_AnimIndex());
	m_fCurrentDuration = pRuiDad->Get_Model()->Get_CurrentTime_Index(pRuiDad->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;


	if (fRatio >= 0.7f)
		m_bNextAnim = true;


	////////////////////////////// 콜박스 여기서 하면될듯
	CCharacters* m_pTarget = pRuiDad->Get_BattleTarget();

	m_fMove += fTimeDelta;
	m_fDelay += fTimeDelta;
	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START: // 1타
		if (m_fMove < 0.7f)
		{
			if(m_fMove < 0.2f)
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());
			else if (m_fMove > 0.4f && m_fMove < 0.6f)
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());

			_vector vCollPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pRuiDad->Get_SphereCollider();
			if (m_fDelay > 0.3f && m_iHit < 2)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-30 * pRuiDad->Get_PlayerInfo().fPowerUp));
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
					{
						m_pTarget->Set_Hp(_int(-pRuiDad->Get_PlayerInfo().iDmg * pRuiDad->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.3f, false);
						pRuiDad->Set_Combo(1);
						pRuiDad->Set_ComboTime(0.f);
					}


					if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
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
						m_fDelay = 0.f;
					}
				}
				
			}

			if (pMyCollider2->Collision(pTargetCollider))
			{
				_float fSpeed = pRuiDad->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				vPos.m128_f32[1] = 0.f;
				_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
				if (pRuiDad->Get_NavigationCom()->Cheak_Cell(vPos))
					pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
				if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
				else
					pRuiDad->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRuiDad->Get_NavigationCom());
			}
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_ATK1_1, pRuiDad);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_ATK1_2, pRuiDad);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::CRuiDadState::TYPE_LOOP: // 2타
		if (m_fMove < 1.f)
		{
			if (m_fMove < 0.7f)
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 1.3f, pRuiDad->Get_NavigationCom());

			_vector vCollPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pRuiDad->Get_SphereCollider();
			if (m_fMove > 0.8f && m_iHit == 0)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-50 * pRuiDad->Get_PlayerInfo().fPowerUp));
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
					{
						m_pTarget->Set_Hp(_int(-20 * pRuiDad->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.3f, false);
						pRuiDad->Set_Combo(1);
						pRuiDad->Set_ComboTime(0.f);
					}


					if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
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
				_float fSpeed = pRuiDad->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				vPos.m128_f32[1] = 0.f;
				_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
				if (pRuiDad->Get_NavigationCom()->Cheak_Cell(vPos))
					pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
				if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
				else
					pRuiDad->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRuiDad->Get_NavigationCom());
			}
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_ATK2_GROUND, pRuiDad);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_ATK2_SLASH, pRuiDad);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::CRuiDadState::TYPE_END: // 3타

		if (m_fMove < 1.f)
		{
			if (m_fMove < 0.4f)
				pRuiDad->Get_Transform()->Go_Straight(fTimeDelta, pRuiDad->Get_NavigationCom());

			_vector vCollPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 3.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pRuiDad->Get_SphereCollider();
			if (m_fMove > 0.6f && m_iHit == 0)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
					{
						m_pTarget->Get_GuardHit(0);
						m_pTarget->Set_GuardHp(_int(-30 * pRuiDad->Get_PlayerInfo().fPowerUp));
						if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
						{
							CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
							pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, m_pTarget);
							RELEASE_INSTANCE(CEffect_Manager);
							m_pTarget->Set_ResetGuardHp();
							m_pTarget->Set_GuardTime(2.f);
						}
					}
					else if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
					{
						m_pTarget->Set_Hp(_int(-pRuiDad->Get_PlayerInfo().iDmg * pRuiDad->Get_PlayerInfo().fPowerUp));
						m_pTarget->Take_Damage(0.3f, false);
						pRuiDad->Set_Combo(1);
						pRuiDad->Set_ComboTime(0.f);
					}


					if (pRuiDad->Get_BattleTarget()->Get_GodMode() == false)
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
				_float fSpeed = pRuiDad->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

				_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				_vector vPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

				_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
				_vector vMyLook = vTargetLook * -1.f;

				_vector vPow = XMVector3Dot(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

				_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

				vPos += vMyLook * (fSpeed - fSpeed * fPow);
				vTargetPos += vTargetLook * fSpeed * fPow;
				vPos.m128_f32[1] = 0.f;
				_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
				if (pRuiDad->Get_NavigationCom()->Cheak_Cell(vPos))
					pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
				if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
				else
					pRuiDad->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pRuiDad->Get_NavigationCom());
			}
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_ATK3_END, pRuiDad);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_ATK3_SLASH, pRuiDad);
			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_ATK3_START, pRuiDad);
			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	}
	//////////////////////////////


	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;


	//if (m_bAtkCombo == true && m_fTime >= 40.f)
	//	return new CAtk_2_State();


	//if (m_bAtkCombo == false)
	//{
	//	if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIM_ATTACK_1))
	//	{
	//		pRuiDad->Get_Model()->Set_End(CRuiDad::ANIM_ATTACK_1);
	//		return new CIdleState();
	//	}
	//}


	return nullptr;
}

CRuiDadState * CAtk_1_State::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	

	pRuiDad->Get_Model()->Play_Animation(fTimeDelta);
	
	return nullptr;
}

void CAtk_1_State::Enter(CRuiDad* pRuiDad)
{
	 

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		m_eStateId = STATE_ID::STATE_ATK_1;
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_0);
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_ATTACK_0);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_ATTACK_0);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_0, false);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_0, 0.01f);
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		m_eStateId = STATE_ID::STATE_ATK_2;
		pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_1);
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_ATTACK_1);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_ATTACK_1);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_1, false);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_1, 0.01f);
		break;
	case Client::CRuiDadState::TYPE_END:
		m_eStateId = STATE_ID::STATE_ATK_3;
		pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_2);
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_ATTACK_2);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_ATTACK_2);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_2, false);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_2, 0.01f);
		break;

	default:
		break;
	}



}

void CAtk_1_State::Exit(CRuiDad* pRuiDad)
{
	m_pCollBox->Set_Dead(); //추가
	//pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_1);
}

