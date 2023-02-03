#include "stdafx.h"
#include "TanjiroSkill_WaterMill.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "TanjiroWeapon.h"
#include "Layer.h"
#include "Kyoujuro.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CSkill_WaterMillState::CSkill_WaterMillState(STATE_TYPE eType)
{
	m_eStateType = eType;
	if (m_eStateType != Client::CTanjiroState::TYPE_START)
	{
		CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_WaterMill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
			return;

		RELEASE_INSTANCE(CGameInstance);
		m_fHitTime = 0.1f;
		
	}
}

CTanjiroState * CSkill_WaterMillState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();


	return nullptr;
}

CTanjiroState * CSkill_WaterMillState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CSkill_WaterMillState(CTanjiroState::TYPE_LOOP);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CSkill_WaterMillState(CTanjiroState::TYPE_END);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
	}
	if (m_eStateType == CTanjiroState::TYPE_START && !m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_WATER8_1, pTanjiro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}
	return nullptr;
}

CTanjiroState * CSkill_WaterMillState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
	if (!m_bLook)
	{
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vLook, vLooAt);
		pTanjiro->Get_Transform()->Set_PlayerLookAt(vLooAt);
		m_bLook = true;
	}
	_int iHit = pTanjiro->Get_WaterMillHit();
	m_fTime += fTimeDelta; 
	m_fHitTime += fTimeDelta;
	if (m_fTime < 0.35f)
	{
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta * 1.5f, pTanjiro->Get_NavigationCom());
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		CCollider*	pMyCollider2 = pTanjiro->Get_SphereCollider();
		if (m_eStateType != Client::CTanjiroState::TYPE_START)
		{
			_vector vCollPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			//m_pCollBox->Get_Transform()->LookAt(vLooAt);
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			
			if (m_fHitTime > 0.1f && iHit < 3)
			{
				if (nullptr == pTargetCollider)
					return nullptr;

				if (pMyCollider->Collision(pTargetCollider))
				{
					_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
					m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

					if (m_pTarget->Get_PlayerInfo().bGuard)
					{
						m_pTarget->Get_GuardHit(0);
					}
					else
					{
						m_pTarget->Set_Hp(-30);
						m_pTarget->Take_Damage(0.3f,false);
					}

					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

					pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);

					RELEASE_INSTANCE(CEffect_Manager);

					pTanjiro->Set_WaterMillHit();
					m_fHitTime = 0.f;
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
			_vector vPlayerPosY = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
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

	RELEASE_INSTANCE(CGameInstance);

	pTanjiro->Get_Model()->Play_Animation2(fTimeDelta);

	return nullptr;
}

void CSkill_WaterMillState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_WATERMILL;

	switch (m_eStateType)
	{
	case Client::CTanjiroState::TYPE_START:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_0);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_WATERMILL_0, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_0);
		break;
	case Client::CTanjiroState::TYPE_LOOP:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_1);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_WATERMILL_1, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_1);
		break;
	case Client::CTanjiroState::TYPE_END:
		pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_2);
		pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_WATERMILL_2, 0.01f);
		pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_WATERMILL_2);
		break;
	}

}

void CSkill_WaterMillState::Exit(CTanjiro * pTanjiro)
{
	pTanjiro->Get_Model()->Reset_Anim(pTanjiro->Get_AnimIndex());
	if (m_eStateType != Client::CTanjiroState::TYPE_START)
		m_pCollBox->Set_Dead();
	if (m_eStateType == Client::CTanjiroState::TYPE_END)
	{
		_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vPos.m128_f32[1] = 0.f;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
		pTanjiro->Reset_WaterMillHit();
	}
}

