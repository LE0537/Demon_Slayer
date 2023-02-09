#include "stdafx.h"
#include "RuiDadAtk_1_State.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "RuiDadDashState.h"

#include "AiState.h"


using namespace RuiDad;


CAtk_1_State::CAtk_1_State()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiDadState * CAtk_1_State::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();



	return nullptr;
}

CRuiDadState * CAtk_1_State::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{


	if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIM_ATTACK_1))
	{
		pRuiDad->Get_Model()->Set_End(CRuiDad::ANIM_ATTACK_1);
		return new CIdleState();
	}





	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_1);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_1, 0.01f);

	m_fTime += fTimeDelta * 60;
	m_fComboDelay += fTimeDelta * 60;


	//if (m_bAtkCombo == true && m_fTime >= 40.f)
	//	return new CAtk_2_State();


	if (m_bAtkCombo == false)
	{
		if (pRuiDad->Get_Model()->Get_End(CRuiDad::ANIM_ATTACK_1))
		{
			pRuiDad->Get_Model()->Set_End(CRuiDad::ANIM_ATTACK_1);
			return new CIdleState();
		}
	}


	return nullptr;
}

CRuiDadState * CAtk_1_State::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pRuiDad->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pRuiDad->Get_Transform()->LookAt(vLooAt);

	m_fMove += fTimeDelta;

	if (m_fMove >= 1.f && m_fMove < 1.3f)
		pRuiDad->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pRuiDad->Get_NavigationCom());

	if (m_fMove < 0.5f)
	{

		_vector vCollPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
		_vector vCollLook = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
		vCollPos += XMVector3Normalize(vCollLook) * 4.f; //추가
		vCollPos.m128_f32[1] = 1.f; //추가
		m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
		CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pRuiDad->Get_SphereCollider();
		if (m_fMove > 0.1f && m_iHit == 0)
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
					m_pTarget->Set_GuardHp(-30);
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else
				{
					m_pTarget->Set_Hp(-pRuiDad->Get_PlayerInfo().iDmg);
					m_pTarget->Take_Damage(0.3f, false);
					pRuiDad->Set_Combo(1);
					pRuiDad->Set_ComboTime(0.f);
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

	RELEASE_INSTANCE(CGameInstance);

	pRuiDad->Get_Model()->Play_Animation(fTimeDelta * 1.2f);
	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIATK1, pRuiDad);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

void CAtk_1_State::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_ATK_1;
	pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_1);
	pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIMID::ANIM_ATTACK_1);
	pRuiDad->Set_AnimIndex(CRuiDad::ANIM_ATTACK_1);
	pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_ATTACK_1, false);
	pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_ATTACK_1, 0.01f);
}

void CAtk_1_State::Exit(CRuiDad* pRuiDad)
{
	m_pCollBox->Set_Dead(); //추가
	pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIMID::ANIM_ATTACK_1);
}

