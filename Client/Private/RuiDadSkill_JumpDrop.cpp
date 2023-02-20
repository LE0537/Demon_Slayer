#include "stdafx.h"
#include "RuiDadSkill_JumpDrop.h"
#include "RuiDadIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "Camera_Dynamic.h"
#include "TargetCircle.h"

using namespace RuiDad;


CSkill_JumpDropState::CSkill_JumpDropState(STATE_TYPE eType)
{
	m_eStateType = eType;
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_RuiDadJump"), LEVEL_GAMEPLAY, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CRuiDadState * CSkill_JumpDropState::HandleInput(CRuiDad* pRuiDad)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	

	


	return nullptr;
}

CRuiDadState * CSkill_JumpDropState::Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{


	if (pRuiDad->Get_Model()->Get_End(pRuiDad->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CRuiDadState::TYPE_START:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());

			//if(m_bNextAnim == true)
			//	return new CSkill_JumpDropState(CRuiDadState::TYPE_LOOP);
			break;
		case Client::CRuiDadState::TYPE_LOOP:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());


			//return new CSkill_JumpDropState(CRuiDadState::TYPE_END);
			break;
		case Client::CRuiDadState::TYPE_END:
			pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pRuiDad->Get_Model()->Set_End(pRuiDad->Get_AnimIndex());
	}

	CCharacters* m_pTarget = pRuiDad->Get_BattleTarget();
	_float fRatio = 0.f;
	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		m_fDuration = pRuiDad->Get_Model()->Get_Duration_Index(CRuiDad::ANIM_SKILL3_0);
		m_fCurrentDuration = pRuiDad->Get_Model()->Get_CurrentTime_Index(CRuiDad::ANIM_SKILL3_0);

		 fRatio = m_fCurrentDuration / m_fDuration;

		 if (fRatio >= 0.5f)
			 m_bIncreaseHeight = true;

		 if (m_bIncreaseHeight == true)
		 {
			 pRuiDad->Set_ShadowAlphaIncrease(true);
			 Increase_Height(pRuiDad, fTimeDelta);
		 }

		 if(m_bNextAnim == true)
			 return new CSkill_JumpDropState(CRuiDadState::TYPE_LOOP);

		 if (!m_bEffect)
		 {
			 CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			 pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_JUMP_PLAYERUP, pRuiDad);
			 pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_JUMP_UP, pRuiDad);

			 RELEASE_INSTANCE(CEffect_Manager);
			 m_bEffect = true;
		 }
		break;
	case Client::CRuiDadState::TYPE_LOOP:

		if (m_bCreateTargetCircle == true)
			m_fFallTime += fTimeDelta;

		if (m_fFallTime >= 1.5f && m_bCreateTargetCircle == true)
		{
			_vector vPosition = XMVectorSetW(XMLoadFloat3(&m_vTempPosition), 1.f);
			pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

			m_bCreateTargetCircle = false;
		}

		if (m_fFallTime >= 1.5f)
		{
			pRuiDad->Set_ShadowAlphaIncrease(false);
			pRuiDad->Set_ShadowAlphaDecrease(true);

			m_bIncreaseHeight = false;
			Fall_Height(pRuiDad, fTimeDelta);
		}
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_JUMP_PLAYERDOWN, pRuiDad);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		if (m_bNextAnim == true)
			return new CSkill_JumpDropState(STATE_TYPE::TYPE_END);
		break;
	case Client::CRuiDadState::TYPE_END:
		m_fMove += fTimeDelta;
		
		if (m_fMove < 0.4f)
		{
			_vector vCollPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
			CCollider*	pMyCollider2 = pRuiDad->Get_SphereCollider();
			if (m_iHit < 1)
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
						m_pTarget->Set_GuardHp(_int(-150 * pRuiDad->Get_PlayerInfo().fPowerUp));
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
						m_pTarget->Set_Hp(_int(-150 * pRuiDad->Get_PlayerInfo().fPowerUp));
						m_pTarget->Player_UpperDown(CCharacters::HIT_KNOCKBACK, 20.f, 30.f, 5.f);
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

			pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_DASH_ENDGROUND, pRuiDad);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
		break;
	case Client::CRuiDadState::TYPE_DEFAULT:
		break;
	case Client::CRuiDadState::TYPE_CHANGE:
		break;
	default:
		break;
	}

	return nullptr;
}

CRuiDadState * CSkill_JumpDropState::Late_Tick(CRuiDad* pRuiDad, _float fTimeDelta)
{
	
	if (m_bIncreaseHeight == false)
		pRuiDad->Get_Model()->Play_Animation(fTimeDelta);


	/*else if (m_bIncreaseHeight == true)
		pRuiDad->Get_Model()->Reset_Anim(CRuiDad::ANIM_SKILL3_0);*/
	return nullptr;
}

void CSkill_JumpDropState::Enter(CRuiDad* pRuiDad)
{
	m_eStateId = STATE_ID::STATE_SKILL_FALLCUT;

	switch (m_eStateType)
	{
	case Client::CRuiDadState::TYPE_START:
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL3_0);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL3_0, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL3_0);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL3_0);
	//	m_fOriginPosY = pRuiDad->Get_BattleTarget()->Get_NavigationHeight().y;
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 30.f;
		m_vVelocity.z = 0.f;
		m_vPosition.x = XMVectorGetX(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiDadState::TYPE_LOOP:
		pRuiDad->Get_Transform()->Set_PlayerLookAt(pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL3_1);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL3_1, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL3_1);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL3_1, true);
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
		Create_TargetCircle(pRuiDad, 0.f);
		m_fOriginPosY = 0.f;
		m_vPosition.x = XMVectorGetX(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		break;
	case Client::CRuiDadState::TYPE_END:

		pRuiDad->Get_Model()->Set_CurrentAnimIndex(CRuiDad::ANIM_SKILL3_2);
		pRuiDad->Get_Model()->Set_LinearTime(CRuiDad::ANIM_SKILL3_2, 0.01f);
		pRuiDad->Set_AnimIndex(CRuiDad::ANIM_SKILL3_2);
		pRuiDad->Get_Model()->Set_Loop(CRuiDad::ANIM_SKILL3_2);


		CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.4f);
		dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pRuiDad->Get_Renderer());
		RELEASE_INSTANCE(CGameInstance);

		break;
	}

}

void CSkill_JumpDropState::Exit(CRuiDad* pRuiDad)
{
	m_pCollBox->Set_Dead();
}

CRuiDadState * CSkill_JumpDropState::Increase_Height(CRuiDad * pRuiDad, _float fTimeDelta)
{
	//pRuiDad->Set_NavigationHeight(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pRuiDad->Get_NavigationHeight().y;
	pRuiDad->Get_Transform()->Set_Jump(true);

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 200.f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 20.f)
	{
		pRuiDad->Set_Render(false);
	//	pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		
		m_bNextAnim = true;
	}
	else
		pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	return nullptr;
}

CRuiDadState * CSkill_JumpDropState::Fall_Height(CRuiDad * pRuiDad, _float fTimeDelta)
{
	pRuiDad->Set_Render(true);
	static _float fGravity = -200.f;
	static _float fVelocity = 0.f;


	m_vPosition.x = XMVectorGetX(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pRuiDad->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	m_vPosition.y += fVelocity * fTimeDelta;




	_vector vPosition = XMVectorSet(m_vPosition.x + 0.1f, m_vPosition.y, m_vPosition.z + 0.1f, 1.f);

	if (m_vPosition.y <= m_fOriginPosY)
	{
		m_vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		

		pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		pRuiDad->Get_Transform()->Set_Jump(false);
		m_bNextAnim = true;
	}
	else
		pRuiDad->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}

void CSkill_JumpDropState::Create_TargetCircle(CRuiDad* pRuiDad, _float fTimeDelta)
{
	CTargetCircle::tagRangeCircleDesc tInfo{};
	ZeroMemory(&tInfo, sizeof(tInfo));

	_float3 vTargetPosition; XMStoreFloat3(&vTargetPosition, pRuiDad->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	vTargetPosition.y = pRuiDad->Get_BattleTarget()->Get_NavigationHeight().y + 0.1f;

	tInfo.vPosition = vTargetPosition;

	m_bCreateTargetCircle = true;

	vTargetPosition.y += 20.f;

	m_vTempPosition = vTargetPosition;

	CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

	pEffectManger->Create_Effect(CEffect_Manager::EFF_RUIDAD_JUMP_GROUNDMARK, pRuiDad->Get_BattleTarget());

	RELEASE_INSTANCE(CEffect_Manager);

}

